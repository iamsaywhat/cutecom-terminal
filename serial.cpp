#include "serial.h"

//#define DEBUG

#ifdef DEBUG
#include <QDebug>
#endif

Serial::Serial(QObject *parent) : QObject(parent)
{
    port = new QSerialPort(this);
    connect(port, &QSerialPort::readyRead, this, &Serial::receiveData);
    connect(port, &QSerialPort::errorOccurred, this, &Serial::errorHandler);
    qRegisterMetaType<Settings>("Settings");
#ifdef DEBUG
    qDebug() << "From Serial::Serial";
#endif
}

Serial::~Serial(void){
#ifdef DEBUG
    qDebug("From Serial::~Serial");
#endif
    emit finished();
    if(port->isOpen())
        port->close();
    delete port;
}

void Serial::process(){
#ifdef DEBUG
    qDebug("From Serial::process");
#endif
    if(port->bytesAvailable() > 0){
        port->waitForReadyRead(100);
        emit receivedData(port->readAll());
    }
}

void Serial::setSettings(Settings settings){
    port->setPortName(settings.name);
    port->setBaudRate(settings.baudrate);
    port->setParity(settings.parity);
    port->setDataBits(settings.dataBits);
    port->setStopBits(settings.stopBits);
    port->setFlowControl(settings.flowControl);
    portSettingsIsSet = true;
#ifdef DEBUG
    qDebug("From Serial::setSettings");
#endif
}

void Serial::receiveData (void){
    //port->waitForReadyRead(10);
    emit receivedData(port->readAll());
#ifdef DEBUG
    qDebug("From Serial::receiveData");
#endif
}

void Serial::sendData(QByteArray data){
    port->write(data);
#ifdef DEBUG
    qDebug("From Serial::sendData");
#endif
}

void Serial::open(void){
#ifdef DEBUG
    qDebug("From Serial::open");
#endif
    if(portSettingsIsSet && port->open(QSerialPort::ReadWrite))
        emit isConnected(true);
    else
        emit isConnected(false);
}

void Serial::close(void){
#ifdef DEBUG
    qDebug("From Serial::close");
#endif
    if(port->isOpen()) {
        port->close();
        emit isConnected(false);
    }
}

void Serial::errorHandler(QSerialPort::SerialPortError error){
#ifdef DEBUG
    qDebug() << "From Serial::errorHandler";
#endif
    if (error != QSerialPort::NoError) {
        emit errorInfo(port->errorString());
        if(port->isOpen())
            close();
    }
}
