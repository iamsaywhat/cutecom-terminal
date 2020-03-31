#include "logger.h"
#include "converter.h"


Logger::Logger(QObject *parent, SerialGui *port) : QObject(parent){
    _port = port;
}
Logger::~Logger(){
    file->close();
   delete file;
}
bool Logger::enabled(){
    return _enabled;
}

QString Logger::path() const{
    return _path;
}

void Logger::setPath(const QString &path){
    _path = path;
}
void Logger::setEnabled(bool state){
    _enabled = state;
    if(state) {
        connect(_port, &SerialGui::send, this, &Logger::outgoing);
        connect(_port, &SerialGui::received, this, &Logger::incoming);
        connect(_port, &SerialGui::open, this, &Logger::openFile);
        connect(_port, &SerialGui::close, this, &Logger::closeFile);
        if(_port->getConnectionStatus() == SerialGui::OPEN)
            openFile();
    }
    else {
        disconnect(_port, &SerialGui::send, this, &Logger::outgoing);
        disconnect(_port, &SerialGui::received, this, &Logger::incoming);
        connect(_port, &SerialGui::open, this, &Logger::openFile);
        connect(_port, &SerialGui::close, this, &Logger::closeFile);
        closeFile();
    }
}
void Logger::openFile(void){
    QString name;
    name.append(QString::number(QDate::currentDate().day()));
    name.append("-");
    name.append(QString::number(QDate::currentDate().month()));
    name.append("-");
    name.append(QString::number(QDate::currentDate().year()));
    name.append(".log");
    file = new QFile(name);
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}
void Logger::closeFile(void){
    file->close();
}
void Logger::incoming(QByteArray data){
    file->write(data);
}

void Logger::outgoing(QByteArray data){
    file->write("ХУайппhrtyjhy\n");
    file->write(data);
}
