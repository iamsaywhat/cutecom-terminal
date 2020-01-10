/*
 * SerialSettingsField - Класс для конфигурирования/открытия/закрытия COM порта.
 * В коструктор требуется передать указатели на следующие элементы GUI:
 *  1) Указатель на QSerialPort экземпляр класса COM порта
 *  2) Указатель на QComboBox куда класс отобразит доступные порты
 *  3) Указатель на QComboBox куда класс отобразит доступные настройки бодрейта
 *  4) Указатель на QComboBox куда класс отобразит доступные настройки паритета
 *  5) Указатель на QComboBox куда класс отобразит доступные настройки бит поля
 *  6) Указатель на QComboBox куда класс отобразит доступные настройки стоп бит
 *  7) Указатель на QComboBox куда класс отобразит доступные настройки контроля
 *  8) Указатель на QPushButton - кнопку подключения/отключения
 * */
#ifndef SERIALSETTINGSFIELD_H
#define SERIALSETTINGSFIELD_H

#include <QComboBox>
#include <QSerialPort>
#include <QPushButton>


class SerialSettingsField : public QObject // Наследуемся от QObject чтобы иметь доступ к сигналам и слотам
{
   //Q_OBJECT

private:

    QSerialPort* SerialPort;            // Указатель на SerialPort экземпляр к которому привяжем настройки
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

    /*********************************************************************************
     * open - Подключение к порту
    *********************************************************************************/
    void open (void);
    /*********************************************************************************
     * close - Отключение порта
    **********************************************************************************/
    void close (void);

public:

    explicit SerialSettingsField( QSerialPort* _SerialPort,         // SerialPort с которым будем работать
                                  QComboBox*   _Ports,              // ComboBox c доступными Com портами
                                  QComboBox*   _Baudrate,           // ComboBox с настройками скорости
                                  QComboBox*   _Parity,             // ComboBox с настройками паритета
                                  QComboBox*   _Databits,           // ComboBox с настройками бит данных
                                  QComboBox*   _Stopbits,           // ComboBox с настройками стоп-бит
                                  QComboBox*   _Flowcontrol,        // ComboBox с настройками контроля
                                  QPushButton* _ConnectDisconnect); // Кнопка подключения/отключения
    ~SerialSettingsField();

    /*********************************************************************************
     * getStatus - Получить статус подключения
    *********************************************************************************/
    bool getStatus (void);

private slots:

    /*********************************************************************************
     * setCustomBaudrate - Слот обслуживания custom baudrate
    *********************************************************************************/
    void setCustomBaudrate (void);

    /*********************************************************************************
     * OpenOrClose - Слот обслуживания кнопки Connect
    *********************************************************************************/
    void OpenOrClose (void);
};

#endif // SERIALSETTINGSFIELD_H
