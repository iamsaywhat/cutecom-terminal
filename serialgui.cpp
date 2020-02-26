#include "serialgui.h"

#include <QString>
#include <QtDebug>
#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>


SerialGui::SerialGui(QComboBox*   ports,              // ComboBox c доступными Com портами
                     QComboBox*   baudrate,           // ComboBox с настройками скорости
                     QComboBox*   parity,             // ComboBox с настройками паритета
                     QComboBox*   dataBits,           // ComboBox с настройками бит данных
                     QComboBox*   stopBits,           // ComboBox с настройками стоп-бит
                     QComboBox*   flowControl,        // ComboBox с настройками контроля
                     QPushButton* connectButton)      // Кнопка подключения/отключения
{
    // Создаём валидатор данных пользовательского бодрейта
    baudrateValidator = (new QIntValidator(0, 4000000, this));

    // Фиксируем внутри класса указатели на элементы управления
    _ports             = ports;
    _baudrate          = baudrate;
    _parity            = parity;
    _dataBits          = dataBits;
    _stopBits          = stopBits;
    _flowControl       = flowControl;
    _connectButton     = connectButton;

    updatePortsList(_ports);           // Обновим список доступных портов
    fillBaudrateList(_baudrate);       // Размещаем настрокий бодрейта
    fillBaudrateList(_parity);         // Размещаем настройки паритета
    fillDataBitsList(_dataBits);       // Размещаем настройки бит данных
    fillStopBitsList(_stopBits);       // Размещаем настройки стоп-бит
    fillFlowControlList(_flowControl); // Размещаем настройки контроля

    // Настройками по-умолчанию будем считать следующие
    _baudrate->setCurrentIndex(7);      // 115200
    _dataBits->setCurrentIndex(3);      // 8 - бит
    _stopBits->setCurrentIndex(0);      // 1 - стопбит
    _flowControl->setCurrentIndex(0);   // None

    // Добавляем свойство кнопки Checkable
    _connectButton->setCheckable(true);
    // Размещаем на ней соответствующий текст
    _connectButton->setText("Connect");

    // Запрещаем автодобавление пользовательского бодрейта по нажатию enter
    _baudrate->setInsertPolicy(QComboBox::NoInsert);

    connect(_baudrate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SerialGui::setCustomBaudrate);
    connect(_connectButton, &QPushButton::clicked, this, &SerialGui::slotConnection);
    connect(this, &QSerialPort::readyRead, this, &SerialGui::receiveData);
}

SerialGui::~SerialGui ()
{
    delete baudrateValidator;
}

/*********************************************************************************
 * getStatus - Получить статус подключения
*********************************************************************************/
SerialGui::ConnectionStatus SerialGui::getConnectionStatus (void)
{
    return connectionStatus;
}

/*********************************************************************************
 * openPort - Подключение к порту
*********************************************************************************/
void SerialGui::openPort(void)
{
    // Устанавливаем настройки - Выбор порта
    setPortName(_ports->currentText());

    // Выбор Бодрейта
    if(_baudrate->currentIndex() == indexCustomBaudrate)
        // Если на момент нажания кнопки открыть выбран Custom baudrate
        setBaudRate (static_cast<QSerialPort::BaudRate>(_baudrate->currentText().toInt()));
    else
        setBaudRate (static_cast<QSerialPort::BaudRate>(_baudrate->itemData(_baudrate->currentIndex()).toInt()));

    // Прочие настройки порта
    setDataBits   (static_cast<QSerialPort::DataBits>   (_dataBits->itemData(_dataBits->currentIndex()).toInt()));
    setParity     (static_cast<QSerialPort::Parity>     (_parity->itemData(_parity->currentIndex()).toInt()));
    setStopBits   (static_cast<QSerialPort::StopBits>   (_stopBits->itemData(_stopBits->currentIndex()).toInt()));
    setFlowControl(static_cast<QSerialPort::FlowControl>(_flowControl->itemData(_flowControl->currentIndex()).toInt()));

    if(!open(QSerialPort::ReadWrite))
    {
        // Подключение не удалось, сбрасываем флаг Checked
        _connectButton->setChecked(false);
        qDebug() << "\nError: COM is not available!";
        // Сбросим флаг статуса открытия порта
        connectionStatus = CLOSED;
        QMessageBox::warning(nullptr, "Warning", "Selected COM is not availalable!");
    }
    else
    {
        // Заменяем текст на кнопке
        _connectButton->setText("Disconnect");
        // Выведем сообщение в дебагер
        qDebug() << "\nConnected to:" << QSerialPort::portName()
                 << "\nBaudrate:"     << baudRate()
                 << "\nData bit:"     << dataBits()
                 << "\nParity:"       << parity()
                 << "\nStop bits:"    << stopBits()
                 << "\nFlow control:" << flowControl();
        // Установим флаг статуса открытия порта
        connectionStatus = OPEN;

        // Блокируем ComboBoxes
        _ports->setEnabled(false);
        _baudrate->setEnabled(false);
        _parity->setEnabled(false);
        _dataBits->setEnabled(false);
        _stopBits->setEnabled(false);
        _flowControl->setEnabled(false);
    }
}

/*********************************************************************************
 * closePort - Отключение порта
**********************************************************************************/
void SerialGui::closePort(void)
{
    // Заменяем текст на кнопке
    _connectButton->setText("Connect");
    // Отключаемся
    close();
    // Выведем сообщение в дебагер
    qDebug() << "\nDisconnected from:" << QSerialPort::portName();
    // Сбросим флаг статуса открытия порта
    connectionStatus = CLOSED;

    // Разблокируем ComboBoxes
    _ports->setEnabled(true);
    _baudrate->setEnabled(true);
    _parity->setEnabled(true);
    _dataBits->setEnabled(true);
    _stopBits->setEnabled(true);
    _flowControl->setEnabled(true);
}
/*********************************************************************************
 * updatePortsList - Обновление в списка доступных портов
**********************************************************************************/
void SerialGui::updatePortsList(QComboBox *comboBox){
    /* Перед обновлением списка портов запоминаем текущий порт,
     * чтобы после того установить его же как текущий, несмотря на то,
     * что мог измениться его индекс в списке. Если порт был перехвачен
     * (отсутствует в списке), то ничего не произойдет и текущим станет
     * первый в списке порт */
    QString currentText = comboBox->currentText();
    comboBox->clear();
    // Смотрим список доступных портов и пишем его в comboBox
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        if(!info.isBusy())
            comboBox->addItem(info.portName());
    }
    comboBox->setCurrentText(currentText);
}
void SerialGui::fillBaudrateList(QComboBox *comboBox){
    comboBox->clear();
    comboBox->addItem(QStringLiteral("1200"),    QSerialPort::Baud1200);
    comboBox->addItem(QStringLiteral("2400"),    QSerialPort::Baud2400);
    comboBox->addItem(QStringLiteral("4800"),    QSerialPort::Baud4800);
    comboBox->addItem(QStringLiteral("9600"),    QSerialPort::Baud9600);
    comboBox->addItem(QStringLiteral("19200"),   QSerialPort::Baud19200);
    comboBox->addItem(QStringLiteral("38400"),   QSerialPort::Baud38400);
    comboBox->addItem(QStringLiteral("57600"),   QSerialPort::Baud57600);
    comboBox->addItem(QStringLiteral("115200"),  QSerialPort::Baud115200);
    comboBox->addItem(QStringLiteral("Custom"));
}
void SerialGui::fillParityList(QComboBox *comboBox){
    comboBox->clear();
    comboBox->addItem(QStringLiteral("None"),  QSerialPort::NoParity);
    comboBox->addItem(QStringLiteral("Even"),  QSerialPort::EvenParity);
    comboBox->addItem(QStringLiteral("Odd"),   QSerialPort::OddParity);
    comboBox->addItem(QStringLiteral("Mark"),  QSerialPort::MarkParity);
    comboBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);
}
void SerialGui::fillDataBitsList(QComboBox *comboBox){
    comboBox->clear();
    comboBox->addItem(QStringLiteral("5"), QSerialPort::QSerialPort::Data5);
    comboBox->addItem(QStringLiteral("6"), QSerialPort::QSerialPort::Data6);
    comboBox->addItem(QStringLiteral("7"), QSerialPort::QSerialPort::Data7);
    comboBox->addItem(QStringLiteral("8"), QSerialPort::QSerialPort::Data8);
}
void SerialGui::fillStopBitsList(QComboBox *comboBox){
    comboBox->clear();
    comboBox->addItem(QStringLiteral("1"),   QSerialPort::OneStop);
    comboBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
    comboBox->addItem(QStringLiteral("2"),   QSerialPort::TwoStop);
}
void SerialGui::fillFlowControlList(QComboBox *comboBox){
    comboBox->clear();
    comboBox->addItem(QStringLiteral("None"),     QSerialPort::NoFlowControl);
    comboBox->addItem(QStringLiteral("RTS/CTS"),  QSerialPort::HardwareControl);
    comboBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
}
/*********************************************************************************
* OpenOrClose - Слот обслуживания кнопки Connect
*********************************************************************************/
void SerialGui::slotConnection(void)
{
    // Если Checked не установлен, значит выполняем подключение
    if(_connectButton->isChecked())
        openPort();
    // Если Checked установлен - отключаемся
    else {
        closePort();
        updatePortsList(_ports);}
}

/*********************************************************************************
 * setCustomBaudrate - Слот обслуживания custom baudrate
*********************************************************************************/
void SerialGui::setCustomBaudrate(void)
{
    // Если выбран Custom
    if (_baudrate->currentIndex() == indexCustomBaudrate)
    {
        // Делаем изменяемым
        _baudrate->setEditable(true);
        // Убираем текст
        _baudrate->clearEditText();
        // Привязываем к полю ввода валидатор данных
        _baudrate->lineEdit()->setValidator(baudrateValidator);
    }
    // Если выбран любой другой пункт
    else
        _baudrate->setEditable(false);
}


void SerialGui::receiveData(void)
{
    // По рекомендациям втыкаем ожидание перед считыванием
    waitForReadyRead(1);
    // Принимаем данные и сразу преобразуем в строку
    receiveBuffer = readAll();
    // Испускаем сигнал о наличии новых данных
    emit receivedNewData();
}

QByteArray SerialGui::getData(void)
{
    return receiveBuffer;
}
