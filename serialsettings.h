/**
 * @class SerialSettings
 *
 * Класс потомок QSerialPort, позволяющий быстро связать
 * SerialPort c UI, элементами, такими как:
 * 1) ComboBox выбора порта;
 * 2) ComboBox выбора бодрейта;
 * 3) ComboBox выбора паритета;
 * 4) ComboBox выбора длины бит данных;
 * 5) ComboBox выбора количество стоп-бит;
 * 6) ComboBox выбора контроля;
 * 7) PushButton подключения/отключения;
 */
#ifndef SERIALSETTINGS_H
#define SERIALSETTINGS_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QPushButton>

class SerialSettings : public QSerialPort, public QSerialPortInfo
{
    Q_OBJECT

public:
    explicit SerialSettings(QComboBox*   _Ports,              // ComboBox c доступными Com портами
                            QComboBox*   _Baudrate,           // ComboBox с настройками скорости
                            QComboBox*   _Parity,             // ComboBox с настройками паритета
                            QComboBox*   _Databits,           // ComboBox с настройками бит данных
                            QComboBox*   _Stopbits,           // ComboBox с настройками стоп-бит
                            QComboBox*   _Flowcontrol,        // ComboBox с настройками контроля
                            QPushButton* _ConnectDisconnect); // Кнопка подключения/отключения
     ~SerialSettings();


    /**
     * @brief getStatus
     * Получить статус подключения
     * @return Статус подключения
     * true - Подключено
     * false - Отключено
     */
    bool getStatus (void);

private:
    QComboBox*   Ports;                 // Указатель на ComboBox c доступными Com портами
    QComboBox*   Baudrate;              // Указатель на ComboBox с настройками скорости
    QComboBox*   Parity;                // Указатель на ComboBox с настройками паритета
    QComboBox*   Databits;              // Указатель на ComboBox с настройками бит данных
    QComboBox*   Stopbits;              // Указатель на ComboBox с настройками стоп-бит
    QComboBox*   Flowcontrol;           // Указатель на ComboBox с настройками контроля
    QPushButton* ConnectDisconnect;     // Указатель на Кнопка подключения/отключения

    // Индекс элемента пользовательского Baudrate (Custom)
    const char  BaudrateCustom_indx = 8;

    // Статус подключения (0 закрыт, 1 открыт)
    bool Status = 0;

    // Валидатор вводимых данных в поле Custom Baudrate;
    QIntValidator        *Baudrate_Validator;

    /**
     * @brief openPort
     * Подключение к порту
     */
    void openPort (void);

    /**
     * @brief closePort
     * Отключение от порта
     */
    void closePort (void);

    /* Переношу методы родителя в приватную секцию,
    чтобы пользователь мог подключаться только посредством кнопки*/
    using QSerialPort::open;
    using QSerialPort::close;

private slots:

    /**
     * @brief setCustomBaudrate
     * Слот обслуживания custom baudrate
     */
    void setCustomBaudrate (void);

    /**
     * @brief OpenOrClose
     * Слот обслуживания кнопки Connect
     */
    void OpenOrClose (void);
};

#endif // SERIALSETTINGS_H
