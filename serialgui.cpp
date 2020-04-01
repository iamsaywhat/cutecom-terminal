#include "serialgui.h"
#include <QLineEdit>

#include <QDebug>


SerialGui::SerialGui(QComboBox*   ports,              // ComboBox c доступными Com портами
                     QComboBox*   baudrate,           // ComboBox с настройками скорости
                     QComboBox*   parity,             // ComboBox с настройками паритета
                     QComboBox*   dataBits,           // ComboBox с настройками бит данных
                     QComboBox*   stopBits,           // ComboBox с настройками стоп-бит
                     QComboBox*   flowControl,        // ComboBox с настройками контроля
                     QPushButton* connectButton)      // Кнопка подключения/отключения
{
    // Фиксируем внутри класса указатели на элементы управления
    _ports             = ports;
    _baudrate          = baudrate;
    _parity            = parity;
    _dataBits          = dataBits;
    _stopBits          = stopBits;
    _flowControl       = flowControl;
    _connectButton     = connectButton;
    // Заполняем элементы GUI
    updatePortsList(_ports);           // Обновим список доступных портов
    fillBaudrateList(_baudrate);       // Размещаем настрокий бодрейта
    fillParityList(_parity);           // Размещаем настройки паритета
    fillDataBitsList(_dataBits);       // Размещаем настройки бит данных
    fillStopBitsList(_stopBits);       // Размещаем настройки стоп-бит
    fillFlowControlList(_flowControl); // Размещаем настройки контроля
    // Настройками по-умолчанию будем считать следующие
    _baudrate->setCurrentText("115200");
    _dataBits->setCurrentText("8");
    _stopBits->setCurrentText("1");
    _flowControl->setCurrentText("None");
    // Размещаем на ней соответствующий текст
    _connectButton->setText(tr("Connect"));
    // Запрещаем автодобавление пользовательского бодрейта по нажатию enter
    _baudrate->setInsertPolicy(QComboBox::NoInsert);
    // Создаём валидатор данных пользовательского бодрейта
    baudrateValidator = (new QIntValidator(0, 4000000, this));
    _baudrate->setEditable(true);
    _baudrate->setValidator(baudrateValidator);

    otherThread = new QThread;
    port = new Serial;
    port->moveToThread(otherThread);

    // Стандартные подключения для управления потоком
    connect(otherThread, &QThread::started, port, &Serial::process);
    connect(port, &Serial::finished, otherThread, &QThread::quit);
    connect(otherThread, &QThread::finished, port, &Serial::deleteLater);
    connect(port, &Serial::finished, otherThread, &QThread::deleteLater);
    // Для взаимодействия с базовым классом из другого потока
    connect(this, &SerialGui::send, port, &Serial::sendData);
    connect(port, &Serial::receivedData, this, &SerialGui::receivedData);
    connect(port, &Serial::isConnected, this, &SerialGui::portStatusChanged);
    connect(this, &SerialGui::open, port, &Serial::open);
    connect(this, &SerialGui::close, port, &Serial::close);
    connect(this, &SerialGui::setSettings, port, &Serial::setSettings);
    connect(port, &Serial::errorInfo, this, &SerialGui::error);
    connect(port, &Serial::captureIntervalChanged, this, &SerialGui::portCaptureTimeChanged);
    connect(port, &Serial::capturePacketSizeChanged, this, &SerialGui::portCaptureSizeChanged);
    // События от Gui
    connect(_connectButton, &QPushButton::clicked, this, &SerialGui::openOrCloseByButton);

    // Запускаем поток
    otherThread->start();

    setCaptureTime(20);
    setCaptureSize(20);
}

SerialGui::~SerialGui(){
    delete baudrateValidator;
}

void SerialGui::portStatusChanged(bool status){
    if(status){
        connectionStatus = OPEN;
        // Заменяем текст на кнопке
        _connectButton->setText(tr("Disconnect"));
        // Блокируем ComboBoxes
        _ports->setEnabled(false);
        _baudrate->setEnabled(false);
        _parity->setEnabled(false);
        _dataBits->setEnabled(false);
        _stopBits->setEnabled(false);
        _flowControl->setEnabled(false);
    }
    else {
        connectionStatus = CLOSED;
        // Заменяем текст на кнопке
        _connectButton->setText(tr("Connect"));
        // Разблокируем ComboBoxes
        _ports->setEnabled(true);
        _baudrate->setEnabled(true);
        _parity->setEnabled(true);
        _dataBits->setEnabled(true);
        _stopBits->setEnabled(true);
        _flowControl->setEnabled(true);
    }
}

SerialGui::ConnectionStatus SerialGui::getConnectionStatus (void){
    return connectionStatus;
}

void SerialGui::receivedData(QByteArray data){
   received(data);
}

void SerialGui::openOrCloseByButton(void){
    if(!(connectionStatus == OPEN)){
        Serial::Settings settings;
        settings.name        = _ports->currentText();
        settings.baudrate    = static_cast<QSerialPort::BaudRate>
                                    (_baudrate->currentText().toInt());
        settings.parity      = static_cast<QSerialPort::Parity>
                                    (_parity->itemData(_parity->currentIndex()).toInt());
        settings.dataBits    = static_cast<QSerialPort::DataBits>
                                    (_dataBits->itemData(_dataBits->currentIndex()).toInt());
        settings.stopBits    = static_cast<QSerialPort::StopBits>
                                    (_stopBits->itemData(_stopBits->currentIndex()).toInt());
        settings.flowControl = static_cast<QSerialPort::FlowControl>
                                    (_flowControl->itemData(_flowControl->currentIndex()).toInt());
        emit setSettings(settings);
        emit open();
    }
    else
        emit close();
}
void SerialGui::setCaptureTime(int time){
    port->setCaptureInterval(time);
}
void SerialGui::setCaptureSize(qint64 size){
    port->setCapturePacketSize(size);
}
int SerialGui::captureTime(void) const{
    return _captureTime;
}
qint64 SerialGui::captureSize(void) const{
    return _captureSize;
}
void SerialGui::portCaptureTimeChanged(int time){
    _captureTime = time;
    emit captureTimeChanged(time);
}
void SerialGui::portCaptureSizeChanged(qint64 size){
    _captureSize = size;
    emit captureSizeChanged(size);
}
void SerialGui::retranslate(void){
    if(connectionStatus == OPEN)
        _connectButton->setText(tr("Disconnect"));
    else
        _connectButton->setText(tr("Connect"));
}
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
