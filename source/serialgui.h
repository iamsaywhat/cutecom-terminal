#ifndef SERIALGUI_H
#define SERIALGUI_H

#include <QObject>
#include <QComboBox>
#include <QPushButton>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "serial.h"

class SerialGui : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int captureTime READ captureTime WRITE setCaptureTime NOTIFY captureTimeChanged)
    Q_PROPERTY(qint64 captureSize READ captureSize WRITE setCaptureSize NOTIFY captureSizeChanged)

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
        OPEN,
    };
    // Обновить список доступных портов
    static void updatePortsList    (QComboBox *comboBox);
    static void fillBaudrateList   (QComboBox *comboBox);
    static void fillParityList     (QComboBox *comboBox);
    static void fillDataBitsList   (QComboBox *comboBox);
    static void fillStopBitsList   (QComboBox *comboBox);
    static void fillFlowControlList(QComboBox *comboBox);
    // Узнать состояние подключения
    ConnectionStatus getConnectionStatus (void);
    void setCaptureTime(int);
    void setCaptureSize(qint64);
    void retranslate (void);
    int captureTime() const;
    qint64 captureSize() const;

private:
    // Элементы GUI
    QComboBox       *_ports;                 // Указатель на ComboBox c доступными Com портами
    QComboBox       *_baudrate;              // Указатель на ComboBox с настройками скорости
    QComboBox       *_parity;                // Указатель на ComboBox с настройками паритета
    QComboBox       *_dataBits;              // Указатель на ComboBox с настройками бит данных
    QComboBox       *_stopBits;              // Указатель на ComboBox с настройками стоп-бит
    QComboBox       *_flowControl;           // Указатель на ComboBox с настройками контроля
    QPushButton     *_connectButton;         // Указатель на Кнопка подключения/отключения
    // Рабочие члены
    QIntValidator   *baudrateValidator;          // Валидатор вводимых данных в поле Custom Baudrate;
    QThread         *otherThread;                // Поток для serial
    Serial          *port;                       // Базовый класс для работы с com-портом
    ConnectionStatus connectionStatus = CLOSED;  // Статус подключения
    int             _captureTime = 20;
    qint64          _captureSize = 20;

signals:
    void setSettings(Serial::Settings);       // Отправка настроек базовому классу
    void send(QByteArray);                    // Отправка данных базовому классу
    void received(QByteArray);                // Полученные данные по подписке на сигнал
    void open();                              // Открыть порт
    void close();                             // Закрыть порт
    void error(QString);
    void captureTimeChanged(int);
    void captureSizeChanged(qint64);

private slots:
     void portStatusChanged(bool);
     void receivedData(QByteArray);
     void openOrCloseByButton(void);
     void portCaptureTimeChanged(int);
     void portCaptureSizeChanged(qint64);
};

#endif // SERIALGUI_H
