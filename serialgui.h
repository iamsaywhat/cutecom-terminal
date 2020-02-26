/**
 * @class SerialGui
 *
 * Класс потомок QSerialPort и QSerialPortInfo, позволяющий быстро связать
 * SerialPort c GUI, элементами, такими как:
 *
 * 1) ComboBox выбора порта;
 * 2) ComboBox выбора бодрейта;
 * 3) ComboBox выбора паритета;
 * 4) ComboBox выбора длины бит данных;
 * 5) ComboBox выбора количество стоп-бит;
 * 6) ComboBox выбора контроля;
 * 7) PushButton подключения/отключения;
 */
#ifndef SERIAL_GUI_H
#define SERIAL_GUI_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QPushButton>

class SerialGui : public QSerialPort, public QSerialPortInfo
{
    Q_OBJECT

public:
    explicit SerialGui(QComboBox*   ports,              // ComboBox c доступными Com портами
                       QComboBox*   baudrate,           // ComboBox с настройками скорости
                       QComboBox*   parity,             // ComboBox с настройками паритета
                       QComboBox*   dataBits,           // ComboBox с настройками бит данных
                       QComboBox*   stopBits,           // ComboBox с настройками стоп-бит
                       QComboBox*   flowControl,        // ComboBox с настройками контроля
                       QPushButton* connectButton);     // Кнопка подключения/отключения
     ~SerialGui();
    enum ConnectionStatus{
        CLOSED,
        OPEN
    };

    // Подключение к порту
    void openPort (void);
    // Отключение от порта
    void closePort (void);
    // Обновить список доступных портов
    void updatePortsList(QComboBox *comboBox);
    void fillBaudrateList(QComboBox *comboBox);
    void fillParityList(QComboBox *comboBox);
    void fillDataBitsList(QComboBox *comboBox);
    void fillStopBitsList(QComboBox *comboBox);
    void fillFlowControlList(QComboBox *comboBox);
    // Узнать состояние подключения
    ConnectionStatus getConnectionStatus (void);
    QByteArray       getData(void);

private:
    QComboBox*      _ports;                 // Указатель на ComboBox c доступными Com портами
    QComboBox*      _baudrate;              // Указатель на ComboBox с настройками скорости
    QComboBox*      _parity;                // Указатель на ComboBox с настройками паритета
    QComboBox*      _dataBits;              // Указатель на ComboBox с настройками бит данных
    QComboBox*      _stopBits;              // Указатель на ComboBox с настройками стоп-бит
    QComboBox*      _flowControl;           // Указатель на ComboBox с настройками контроля
    QPushButton*    _connectButton;         // Указатель на Кнопка подключения/отключения
    QIntValidator*  baudrateValidator;      // Валидатор вводимых данных в поле Custom Baudrate;
    QByteArray      receiveBuffer;          // Буфер под хранение последних принятых данных

    const char  indexCustomBaudrate = 8;             // Индекс элемента пользовательского Baudrate (Custom)
    ConnectionStatus connectionStatus = CLOSED;      // Статус подключения



    /* Переношу методы родителя в приватную секцию,
    чтобы пользователь мог подключаться только посредством кнопки */
    using QSerialPort::open;
    using QSerialPort::close;
private slots:
    // Слот обслуживания custom baudrate
    void setCustomBaudrate (void);
    // Слот обслуживания кнопки Connect
    void slotConnection (void);
    // Cлот приёма данных */
    void receiveData(void);

signals:
    // Сигнал о получении новых данных
    void receivedNewData(void);
};

#endif // SERIAL_GUI_H
