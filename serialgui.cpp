#include "serialgui.h"

#include <QString>
#include <QtDebug>
#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>

SerialGui::SerialGui(QComboBox*   _Ports,              // ComboBox c доступными Com портами
                           QComboBox*   _Baudrate,           // ComboBox с настройками скорости
                           QComboBox*   _Parity,             // ComboBox с настройками паритета
                           QComboBox*   _Databits,           // ComboBox с настройками бит данных
                           QComboBox*   _Stopbits,           // ComboBox с настройками стоп-бит
                           QComboBox*   _Flowcontrol,        // ComboBox с настройками контроля
                           QPushButton* _ConnectDisconnect)  // Кнопка подключения/отключения
{
    // Создаём валидатор данных пользовательского бодрейта
    Baudrate_Validator = (new QIntValidator(0, 4000000, this));

    // Фиксируем внутри класса указатели на элементы управления
    Ports             = _Ports;
    Baudrate          = _Baudrate;
    Parity            = _Parity;
    Databits          = _Databits;
    Stopbits          = _Stopbits;
    Flowcontrol       = _Flowcontrol;
    ConnectDisconnect = _ConnectDisconnect;

    // Посмотрим список имеющихся COM портов и разместим их список в соответствующем боксе
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        Ports->addItem(info.portName());

    // Размещаем в селекторе доступный бодрейт
    Baudrate->addItem(QStringLiteral("1200"),    QSerialPort::Baud1200);
    Baudrate->addItem(QStringLiteral("2400"),    QSerialPort::Baud2400);
    Baudrate->addItem(QStringLiteral("4800"),    QSerialPort::Baud4800);
    Baudrate->addItem(QStringLiteral("9600"),    QSerialPort::Baud9600);
    Baudrate->addItem(QStringLiteral("19200"),   QSerialPort::Baud19200);
    Baudrate->addItem(QStringLiteral("38400"),   QSerialPort::Baud38400);
    Baudrate->addItem(QStringLiteral("57600"),   QSerialPort::Baud57600);
    Baudrate->addItem(QStringLiteral("115200"),  QSerialPort::Baud115200);
    Baudrate->addItem(QStringLiteral("Custom"));

    // Размещаем настройки паритета
    Parity->addItem(QStringLiteral("None"),  QSerialPort::NoParity);
    Parity->addItem(QStringLiteral("Even"),  QSerialPort::EvenParity);
    Parity->addItem(QStringLiteral("Odd"),   QSerialPort::OddParity);
    Parity->addItem(QStringLiteral("Mark"),  QSerialPort::MarkParity);
    Parity->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

    // Размещаем настройки бит данных
    Databits->addItem(QStringLiteral("5"), QSerialPort::QSerialPort::Data5);
    Databits->addItem(QStringLiteral("6"), QSerialPort::QSerialPort::Data6);
    Databits->addItem(QStringLiteral("7"), QSerialPort::QSerialPort::Data7);
    Databits->addItem(QStringLiteral("8"), QSerialPort::QSerialPort::Data8);

    // Размещаем настройки стоп-бит
    Stopbits->addItem(QStringLiteral("1"),   QSerialPort::OneStop);
#ifdef Q_OS_WIN
    Stopbits->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    Stopbits->addItem(QStringLiteral("2"),   QSerialPort::TwoStop);

    // Размещаем настройки контроля
    Flowcontrol->addItem(QStringLiteral("None"),     QSerialPort::NoFlowControl);
    Flowcontrol->addItem(QStringLiteral("RTS/CTS"),  QSerialPort::HardwareControl);
    Flowcontrol->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);

    // Настройками по-умолчанию будем считать следующие
    Baudrate->setCurrentIndex(7);      // 115200
    Databits->setCurrentIndex(3);      // 8 - бит
    Stopbits->setCurrentIndex(0);      // 1 - стопбит
    Flowcontrol->setCurrentIndex(0);   // None

    // Добавляем свойство кнопки Checkable
    ConnectDisconnect->setCheckable(true);
    // Размещаем на ней соответствующий текст
    ConnectDisconnect->setText("Connect");

    // Запрещаем автодобавление пользовательского бодрейта по нажатию enter
    Baudrate->setInsertPolicy(QComboBox::NoInsert);

    // Подключаем сигнал при изменении селектора бодрейта на слот работы с custom значением
    connect(Baudrate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SerialGui::setCustomBaudrate);

    // Подключаем клик по кнопке ConnectDisconnect на слот OpenOrClose
    connect(ConnectDisconnect, &QPushButton::clicked, this, &SerialGui::slotConnection);
    /* */
    connect(this, &QSerialPort::readyRead, this, &SerialGui::receiveData);
}

SerialGui::~SerialGui ()
{
    delete Baudrate_Validator;
}

/*********************************************************************************
 * getStatus - Получить статус подключения
*********************************************************************************/
ConnectionStatusType SerialGui::getConnectionStatus (void)
{
    return connectionStatus;
}

/*********************************************************************************
 * openPort - Подключение к порту
*********************************************************************************/
void SerialGui::openPort(void)
{
    // Устанавливаем настройки - Выбор порта
    setPortName(Ports->currentText());

    // Выбор Бодрейта
    if(Baudrate->currentIndex() == BaudrateCustom_indx)
    {
        // Если на момент нажания кнопки открыть выбран Custom baudrate
        setBaudRate (static_cast<QSerialPort::BaudRate>(Baudrate->currentText().toInt()));
    }
    else
    {
        setBaudRate (static_cast<QSerialPort::BaudRate>(Baudrate->itemData(Baudrate->currentIndex()).toInt()));
    }

    // Прочие настройки порта
    setDataBits   (static_cast<QSerialPort::DataBits>   (Databits->itemData(Databits->currentIndex()).toInt()));
    setParity     (static_cast<QSerialPort::Parity>     (Parity->itemData(Parity->currentIndex()).toInt()));
    setStopBits   (static_cast<QSerialPort::StopBits>   (Stopbits->itemData(Stopbits->currentIndex()).toInt()));
    setFlowControl(static_cast<QSerialPort::FlowControl>(Flowcontrol->itemData(Flowcontrol->currentIndex()).toInt()));

    if(!open(QSerialPort::ReadWrite))
    {
        // Подключение не удалось, сбрасываем флаг Checked
        ConnectDisconnect->setChecked(false);
        qDebug() << "\nError: COM is not available!";
        // Сбросим флаг статуса открытия порта
        connectionStatus = CLOSED;
        QMessageBox::warning(nullptr, "Warning", "Selected COM is not availalable!");
    }
    else
    {
        // Заменяем текст на кнопке
        ConnectDisconnect->setText("Disconnect");
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
        Ports->setEnabled(false);
        Baudrate->setEnabled(false);
        Parity->setEnabled(false);
        Databits->setEnabled(false);
        Stopbits->setEnabled(false);
        Flowcontrol->setEnabled(false);
    }
}

/*********************************************************************************
 * closePort - Отключение порта
**********************************************************************************/
void SerialGui::closePort(void)
{
    // Заменяем текст на кнопке
    ConnectDisconnect->setText("Connect");
    // Отключаемся
    close();
    // Выведем сообщение в дебагер
    qDebug() << "\nDisconnected from:" << QSerialPort::portName();
    // Сбросим флаг статуса открытия порта
    connectionStatus = CLOSED;

    // Разблокируем ComboBoxes
    Ports->setEnabled(true);
    Baudrate->setEnabled(true);
    Parity->setEnabled(true);
    Databits->setEnabled(true);
    Stopbits->setEnabled(true);
    Flowcontrol->setEnabled(true);
}


/*********************************************************************************
* OpenOrClose - Слот обслуживания кнопки Connect
*********************************************************************************/
void SerialGui::slotConnection(void)
{
    // Если Checked не установлен, значит выполняем подключение
    if(ConnectDisconnect->isChecked())
        openPort();
    // Если Checked установлен - отключаемся
    else
        closePort();
}

/*********************************************************************************
 * setCustomBaudrate - Слот обслуживания custom baudrate
*********************************************************************************/
void SerialGui::setCustomBaudrate(void)
{
    // Если выбран Custom
    if (Baudrate->currentIndex() == BaudrateCustom_indx)
    {
        // Делаем изменяемым
        Baudrate->setEditable(true);
        // Убираем текст
        Baudrate->clearEditText();
        // Привязываем к полю ввода валидатор данных
        QLineEdit *edit = Baudrate->lineEdit();
        edit->setValidator(Baudrate_Validator);
    }
    // Если выбран любой другой пункт
    else
        Baudrate->setEditable(false);
}


void SerialGui::receiveData(void)
{
    /* По рекомендациям втыкаем ожидание перед считыванием */
    waitForReadyRead(1);
    /* Принимаем данные и сразу преобразуем в строку */
    receiveBuffer = readAll();
    /* Испускаем сигнал о наличии новых данных */
    emit receivedNewData();
}

QByteArray SerialGui::getData(void)
{
    return receiveBuffer;
}
