#include "serial.h"

#define DEBUG

#ifdef DEBUG
#include <QDebug>
#endif

Serial::Serial(QObject *parent) : QObject(parent)
{
    port = new QSerialPort(this);
    timer = new QTimer(this);
    connect(port, &QSerialPort::readyRead, this, &Serial::receiveData);
    connect(port, &QSerialPort::errorOccurred, this, &Serial::errorHandler);
    qRegisterMetaType<Settings>("Settings");
    connect(timer, &QTimer::timeout, this, &Serial::unlockTransmit);
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
    if(!isLocked()){
        emit receivedData(port->readAll());
        lockTransmit();
    }
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
void Serial::unlockTransmit(void){
    transmitLocked = false;
    timer->stop();
}
void Serial::lockTransmit(void){
    transmitLocked = true;
    if(captureInterval() > 0) {
        timer->setInterval(captureInterval());
        timer->start();
    }
}
void Serial::setCaptureInterval(int interval){
    if(interval >= 0){
        _captureInterval = interval;
        qDebug() << "capture Interval setted in " << _captureInterval;
        emit captureIntervalChanged(interval);
    }
}
int Serial::captureInterval(void){
    return _captureInterval;
}
void Serial::setCapturePacketSize(qint64 size){
    if(size >= 0){
        _capturePacketSize = size;
        emit capturePacketSizeChanged(size);
    }
}
qint64 Serial::capturePacketSize(void){
    return _capturePacketSize;
}
bool Serial::isLocked(void){
    if(!transmitLocked || ((capturePacketSize() > 0) &&
                          (port->bytesAvailable() > capturePacketSize())))
        return false;
    else
        return true;
}
