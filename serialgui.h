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

typedef enum{
    CLOSED,
    OPEN
}ConnectionStatusType;

class SerialGui : public QSerialPort, public QSerialPortInfo
{
    Q_OBJECT

public:
    explicit SerialGui(QComboBox*   _Ports,              // ComboBox c доступными Com портами
                          QComboBox*   _Baudrate,           // ComboBox с настройками скорости
                          QComboBox*   _Parity,             // ComboBox с настройками паритета
                          QComboBox*   _Databits,           // ComboBox с настройками бит данных
                          QComboBox*   _Stopbits,           // ComboBox с настройками стоп-бит
                          QComboBox*   _Flowcontrol,        // ComboBox с настройками контроля
                          QPushButton* _ConnectDisconnect); // Кнопка подключения/отключения
     ~SerialGui();

    /* Узнать состояние подключения */
    ConnectionStatusType getConnectionStatus (void);
    QByteArray getData(void);


private:
    QComboBox*      Ports;                 // Указатель на ComboBox c доступными Com портами
    QComboBox*      Baudrate;              // Указатель на ComboBox с настройками скорости
    QComboBox*      Parity;                // Указатель на ComboBox с настройками паритета
    QComboBox*      Databits;              // Указатель на ComboBox с настройками бит данных
    QComboBox*      Stopbits;              // Указатель на ComboBox с настройками стоп-бит
    QComboBox*      Flowcontrol;           // Указатель на ComboBox с настройками контроля
    QPushButton*    ConnectDisconnect;     // Указатель на Кнопка подключения/отключения
    QIntValidator*  Baudrate_Validator;    // Валидатор вводимых данных в поле Custom Baudrate;

    QByteArray      receiveBuffer;         /*  Буфер под хранение последних принятых данных */

    const char  BaudrateCustom_indx = 8;             /* Индекс элемента пользовательского Baudrate (Custom) */
    ConnectionStatusType connectionStatus = CLOSED;  /* Статус подключения */

    /* Подключение к порту */
    void openPort (void);
    /* Отключение от порта */
    void closePort (void);
    /* Переношу методы родителя в приватную секцию,
    чтобы пользователь мог подключаться только посредством кнопки*/
    using QSerialPort::open;
    using QSerialPort::close;

private slots:
    /* Слот обслуживания custom baudrate */
    void setCustomBaudrate (void);
    /* Слот обслуживания кнопки Connect */
    void slotConnection (void);
    /* Cлот приёма данных */
    void receiveData(void);

signals:
    /* Сигнал о получении новых данных */
    void receivedNewData(void);
};

#endif // SERIAL_GUI_H
