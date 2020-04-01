#include "serial.h"

#include <QDebug>

Serial::Serial(QObject *parent) : QObject(parent){
    port = new QSerialPort(this);
    timer = new QTimer(this);
    connect(port, &QSerialPort::readyRead, this, &Serial::receiveData);
    connect(port, &QSerialPort::errorOccurred, this, &Serial::errorHandler);
    qRegisterMetaType<Settings>("Settings");
    connect(timer, &QTimer::timeout, this, &Serial::unlockTransmit);
}

Serial::~Serial(void){
    emit finished();
    if(port->isOpen())
        port->close();
    delete port;
}
void Serial::process(){
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
}
void Serial::receiveData (void){
    if(!isLocked()){
        emit receivedData(port->readAll());
        lockTransmit();
    }
}
void Serial::sendData(QByteArray data){
    port->write(data);
}
void Serial::open(void){
    if(portSettingsIsSet && port->open(QSerialPort::ReadWrite))
        emit isConnected(true);
    else
        emit isConnected(false);
}
void Serial::close(void){
    if(port->isOpen()) {
        port->close();
        emit isConnected(false);
    }
}
void Serial::errorHandler(QSerialPort::SerialPortError error){
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
int Serial::captureInterval(void){
    return _captureInterval;
}
qint64 Serial::capturePacketSize(void){
    return _capturePacketSize;
}
void Serial::setCapturePacketSize(qint64 size){
    if(size >= 0){  
        _capturePacketSize = size;
        qDebug() << "capture size setted in " << size;
        emit capturePacketSizeChanged(size);
    }
}
void Serial::setCaptureInterval(int interval){
    if(interval >= 0){
        _captureInterval = interval;
        qDebug() << "capture Interval setted in " << _captureInterval;
        emit captureIntervalChanged(interval);
    }
}
bool Serial::isLocked(void){
    if(!transmitLocked || ((capturePacketSize() > 0) &&
                          (port->bytesAvailable() > capturePacketSize())))
        return false;
    else
        return true;
}
