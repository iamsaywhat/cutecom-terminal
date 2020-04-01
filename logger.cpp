#include "logger.h"
#include "converter.h"
#include <QApplication>

Logger::Logger(QObject *parent, SerialGui *port) : QObject(parent){
    _port = port;
    setPath(QApplication::applicationDirPath()+"/logs/");
}
Logger::~Logger(){
    file->close();
    delete file;
}
bool Logger::enabled() const{
    return _enabled;
}
QString Logger::path() const{
    return _path;
}

int Logger::bytesInRow(void) const{
    return _bytesInRow;
}

int Logger::columnSpace(void) const{
    return _columnSpace;
}

void Logger::setColumnSpace(int columnSpace){
    if(columnSpace > 1){
        _columnSpace = columnSpace;
        emit columnSpaceChanged(columnSpace);
    }
}
void Logger::setPath(QString path){
    QDir dir(path);
    if(!dir.exists()){
        if(!dir.mkpath(path)){
            qDebug("Создать не получилось");
            return;
        }
    }
    _path = path;
    emit pathChanged(path);
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
void Logger::setBytesInRow(int size){
    if(size > 0){
        _bytesInRow = size;
        emit bytesInRowChanged(size);
    }
}
void Logger::openFile(void){
    QString name;
    name.append(QString::number(QDate::currentDate().year()).rightJustified(4, '0'));
    name.append("_");
    name.append(QString::number(QDate::currentDate().month()).rightJustified(2, '0'));
    name.append("_");
    name.append(QString::number(QDate::currentDate().day()).rightJustified(2, '0'));
    name.append("_");
    name.append(QString::number(QTime::currentTime().hour()).rightJustified(2, '0'));
    name.append("_");
    name.append(QString::number(QTime::currentTime().minute()).rightJustified(2, '0'));
    name.append("_");
    name.append(QString::number(QTime::currentTime().second()).rightJustified(2, '0'));
    name.append(".log");
    name.prepend(path());
    QDir dir(path());
    if(!dir.exists()){
        qDebug("Не существует");
        if(!dir.mkpath(path())){
            qDebug("Создать не получилось");
            return;
        }
    }
    file = new QFile(name);
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}
void Logger::closeFile(void){
    file->close();
}
void Logger::write(DirectionType direction, QByteArray& data){
    QStringList ascii;
    QStringList hex;
    QTextStream textStream(file);
    int cursorPosition = 0;
    const int timestampColumnSize = 13;
    const int directionColumnSize = 8;
    int hexColumnSize = 3*bytesInRow()-1;
    int directionColumnPosition  = timestampColumnSize+columnSpace();
    int hexColumnPosition        = directionColumnPosition+directionColumnSize+columnSpace();
    int asciiColumnPosition      = hexColumnPosition+hexColumnSize+columnSpace();

    toHexStrings(hex, data, bytesInRow());
    replaceSymbols(data, '.');
    toAsciiStrings(ascii, data,  bytesInRow());
    textStream.setCodec(Converter::currentCodec());

    for(int i = 0; i < ascii.count(); i++){
        QString text = QTime::currentTime().toString("hh:mm:ss:ms");
        textStream << text;
        cursorPosition += text.count();
        while(cursorPosition < directionColumnPosition){
            cursorPosition++;
            textStream << " ";
        }
        if(direction == INCOMING)
            text = "incoming";
        else
            text = "outgoing";
        textStream << text;
        cursorPosition += text.count();
        while(cursorPosition < hexColumnPosition){
            cursorPosition++;
            textStream << " ";
        }
        textStream << hex.at(i);
        cursorPosition += hex.at(i).count();
        while(cursorPosition < asciiColumnPosition){
            cursorPosition++;
            textStream << " ";
        }
        textStream << ascii.at(i);
        cursorPosition += hex.at(i).count();
        textStream << "\n";
        cursorPosition = 0;
    }
}
void Logger::incoming(QByteArray data){
    write(INCOMING, data);
}
void Logger::outgoing(QByteArray data){
    write(OUTGOING, data);
}
void Logger::toHexStrings(QStringList& list, QByteArray& data, int length){
    QString hex;
    for(int i = 0; i < data.count(); i++){
        hex+=QString::number(static_cast<unsigned char>(data[i]),16).rightJustified(2, '0');
    }
    hex = hex.toUpper();
    int rows = hex.count()/(length*2) + (hex.count()%(length*2) != 0 ? 1 : 0);
    for(int i = 0; i < rows; i++){
        QString temp;
        if(i == rows-1){
            temp = hex;
        }
        else {
            temp = hex.left(length*2);
            hex.remove(0, length*2);
        }
        Converter::setDelimitersInHexString(temp, 2, ' ');
        list.append(temp);
    }
}
void Logger::toAsciiStrings(QStringList& list, QByteArray& data, int length){
    QString ascii = Converter::convertToUnicode(data);
    int rows = ascii.count()/length + (ascii.count()%length != 0 ? 1 : 0);
    for(int i = 0; i < rows; i++){
        if(i == rows-1){
            list.append(ascii);
        }
        else {
            list.append(ascii.left(length));
            ascii.remove(0, length);
        }
    }
}
void Logger::replaceSymbols(QByteArray &data, const char symbol){
    for(int i = 0; i < data.count(); i++){
        if(((data[i] >= char(0x00) && data[i] < char(0x20)) || data[i] == char(0x7F)))
            data[i] = symbol;
    }
}
