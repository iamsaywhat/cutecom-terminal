#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>


class Serial : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int _captureInterval READ captureInterval WRITE setCaptureInterval NOTIFY captureIntervalChanged)
    Q_PROPERTY(qint64 _capturePacketSize READ capturePacketSize WRITE setCapturePacketSize NOTIFY capturePacketSizeChanged)

public:
    explicit Serial(QObject *parent = nullptr);
    ~Serial(void);
    struct Settings {
        QString                  name;
        QSerialPort::BaudRate    baudrate;
        QSerialPort::Parity      parity;
        QSerialPort::DataBits    dataBits;
        QSerialPort::StopBits    stopBits;
        QSerialPort::FlowControl flowControl;
    };    
private:
    QSerialPort *port;
    QTimer *timer;
    bool portSettingsIsSet = false;
    bool transmitLocked = false;
    int _captureInterval = 0;
    qint64 _capturePacketSize = 0;
    void lockTransmit(void);
    bool isLocked(void);
    int captureInterval(void);
    qint64 capturePacketSize(void);

signals:
    void receivedData(QByteArray);
    void isConnected(bool);
    void finished(void);
    void errorInfo(QString);
    void captureIntervalChanged(int interval);
    void capturePacketSizeChanged(qint64 size);

public slots:
    void open(void);
    void close(void);
    void process(void);
    void setSettings(Settings);
    void sendData(QByteArray);
    void setCaptureInterval(int interval);
    void setCapturePacketSize(qint64 size);
    
private slots:
    void receiveData(void);
    void errorHandler(QSerialPort::SerialPortError);
    void unlockTransmit(void);
};

Q_DECLARE_METATYPE(Serial::Settings)

#endif // SERIAL_H
