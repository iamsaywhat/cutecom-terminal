#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>


class Serial : public QObject
{
    Q_OBJECT
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
    bool portSettingsIsSet = false;

signals:
    void receivedData(QByteArray);
    void isConnected(bool);
    void finished(void);
    void errorInfo(QString);

public slots:
    void open(void);
    void close(void);
    void process(void);
    void setSettings(Settings);
    void sendData(QByteArray);
    
private slots:
    void receiveData(void);
    void errorHandler(QSerialPort::SerialPortError);
};

Q_DECLARE_METATYPE(Serial::Settings)

#endif // SERIAL_H
