#include "logger.h"
#include "converter.h"
#include <QApplication>

Logger::Logger(QObject *parent, SerialGui *port) : QObject(parent){
    _port = port;
    setPath(QApplication::applicationDirPath()+"/logs");  // По-умолчанию путь находится в директории приложения
}
Logger::~Logger(){
    if(file!=nullptr && file->isOpen())
        file->close();
    if(file != nullptr)
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
    if(!dir.exists()){            // Проверим существует ли директория файла
        if(!dir.mkpath(path)){    // Если не существует, то пробуем создать
            return;               // Если создать не получилось, остальное
        }                         // неважно, выходим
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
        disconnect(_port, &SerialGui::open, this, &Logger::openFile);
        disconnect(_port, &SerialGui::close, this, &Logger::closeFile);
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
    // Собираем название файла
    name.append("/");
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
    name.append(".log");                       // Формат файла
    name.prepend(path());                      // В начало добавляем путь файла
    file = new QFile(name);                    // С директорией  все впорядке
    file->open(QIODevice::WriteOnly |          // Открываем на запись
               QIODevice::Text);               // Текстовый формат
}
void Logger::closeFile(void){
    if(file!=nullptr && file->isOpen())
        file->close();
    delete file;
    file = nullptr;
}
void Logger::write(DirectionType direction, QByteArray& data){
    QStringList ascii;
    QStringList hex;
    QTextStream textStream(file);
    int cursorPosition = 0;
    const int timestampColumnSize = 13;
    const int directionColumnSize = 8;
    int hexColumnSize = 3*bytesInRow()-1;
    /* Здесь определяем позиции столбцов текста */
    int directionColumnPosition  = timestampColumnSize+columnSpace();
    int hexColumnPosition        = directionColumnPosition+directionColumnSize+columnSpace();
    int asciiColumnPosition      = hexColumnPosition+hexColumnSize+columnSpace();
    /* Здесь байтовый массив нужно сконверировать в нужные форматы
       и нарезать их в строки по bytesInRow байт */
    toHexStrings(hex, data, bytesInRow());            // Байтовый массив в список нарезанных строк hex кодов
    replaceSymbols(data, '.');                        // Перед конвертацией в ascii, нужно все непечаные символы
    toAsciiStrings(ascii, data,  bytesInRow());       // заменить чем-то, а только после байтовый массив сконверитровать
    textStream.setCodec(Converter::currentCodec());   // Печатать будет через TextStream, чтобы можно было настроить кодировку
    /* Здесь начинаем писать строки в файл,
     * строго соблюдая форматирование */
    for(int i = 0; i < ascii.count(); i++){
        QString text;
        text = QTime::currentTime().toString("hh:mm:ss:ms");
        textStream << text;                               // Столбец временной метки
        cursorPosition += text.count();
        while(cursorPosition < directionColumnPosition){  // Заполняем пробелами место до следующего столбца
            cursorPosition++;
            textStream << " ";
        }
        if(direction == INCOMING)                          // Столбец направления
            text = tr("incoming");
        else
            text = tr("outgoing");
        textStream << text;
        cursorPosition += text.count();
        while(cursorPosition < hexColumnPosition){         // Заполняем пробелами место до следующего столбца
            cursorPosition++;
            textStream << " ";
        }
        textStream << hex.at(i);                           // Столбец HEX-кодов
        cursorPosition += hex.at(i).count();
        while(cursorPosition < asciiColumnPosition){       // Заполняем пробелами место до следующего столбца
            cursorPosition++;
            textStream << " ";
        }
        textStream << ascii.at(i);                         // Столбец ASCII-кодов
        cursorPosition += hex.at(i).count();
        textStream << "\n";                                // Здесь нужно перейти на следующую строку
        cursorPosition = 0;                                // позицию курсора поэтому тоже сбрасываем
    }
}
void Logger::incoming(QByteArray data){
    if(file!=nullptr && file->isOpen() && enabled())
        write(INCOMING, data);
}
void Logger::outgoing(QByteArray data){
    if(file!=nullptr && file->isOpen() && enabled())
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
