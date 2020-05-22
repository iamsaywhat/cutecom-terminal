#include "terminal.h"

#include <QString>
#include <QByteArray>
#include <QTime>
#include <QDebug>
#include "source/converter.h"
#include <QScrollBar>

Terminal::Terminal(QObject*        parent,
                   SerialGui*      serial,
                   QPlainTextEdit* console,
                   QLineEdit *     input,
                   QPushButton*    sendButton,
                   QPushButton*    clearButton) : QObject(parent)
{
    /* Сохраняем указатели на формы и прочие классы внутри */
    _serial      = serial;
    _console     = console;
    _input       = input;
    _sendButton  = sendButton;
    _clearButton = clearButton;
    /* Настраиваем свойства и внешний вид рабочих областей */
    _console->setReadOnly(true);               // Запрещаем пользователю редактировать консоль
    _sendButton->setText(tr("Send"));          // Разместим текст на кнопках
    _clearButton->setText(tr("Clear"));
    /* Выполняем функциональные подключения */
    connect(_sendButton,  &QPushButton::clicked, this, QOverload<>::of(&Terminal::send));
    connect(_clearButton, &QPushButton::clicked, this, &Terminal::clear);
    connect(_serial,      &SerialGui::received,  this, &Terminal::received);

    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &Terminal::cyclicTimeout);
    setEchoMode(true);
}

Terminal::~Terminal(){
    delete timer;
}

void Terminal::send(void){
    QString data(_input->text());                                            // Берем текст с поля ввода
    if(data != "" && _serial->getConnectionStatus() != SerialGui::CLOSED) {  // Только если поле ввода что-то содержит
        _serial->send(Converter::convertFromUnicode(data).toLatin1());       // и порт открыт, посылаем данные в порт
        _input->clear();                                                     // и очищаем поле ввода
        qDebug() << "\nConsoleWidget: sending data: " << data;
    }
}
void Terminal::send(QString data){
    if(data != "" && _serial->getConnectionStatus() != SerialGui::CLOSED){  // Только если поле ввода что-то содержит
        _serial->send(Converter::convertFromUnicode(data).toLatin1());       // и порт открыт, посылаем данные в порт
        qDebug() << "\nConsoleWidget: sending cyclic data: " << data;
    }
}
void Terminal::sended(QByteArray data){
    replaceSymbols(data, '.');                         // Необходимо непечатные символы заменить
    QString text = Converter::convertToUnicode(data);  // Необходимо, массив байт представить в текущей кодировке
    _console->moveCursor(QTextCursor::End);            // Смещаем курсор текста гарантированно в конец
    _console->textCursor().insertText(">> ");          // Признак исходящих данных
    _console->textCursor().insertText(text);           // Печатаем то, что пришло
    _console->textCursor().insertText("\n");           // Переходим на следующую строку
    _console->verticalScrollBar()->setSliderPosition(_console->verticalScrollBar()->maximum());
    qDebug() << "\nConsoleWidget: sended data: " << data;
}
void Terminal::received(QByteArray data){
    replaceSymbols(data, '.');                        // Необходимо непечатные символы заменить
    QString text = Converter::convertToUnicode(data); // Необходимо, массив байт представить в текущей кодировке
    _console->moveCursor(QTextCursor::End);           // Смещаем курсор текста гарантированно в конец
    _console->textCursor().insertText(text);          // Печатаем то, что пришло
    _console->verticalScrollBar()->setSliderPosition(_console->verticalScrollBar()->maximum());
    qDebug() << "\nConsoleWidget: received data: " << data;
}
void Terminal::clear (void){
    _console->clear();
    qDebug() << "\nConsoleWidget: text is cleared";
}
bool Terminal::echoMode(void){
    return _echo;
}
bool Terminal::cyclicMode(void){
    return _cyclic;
}
int Terminal::cyclicInterval(void){
    return _cyclicInterval;
}
QString& Terminal::bindData(void){
    return _bindData;
}
void Terminal::setEchoMode(bool state){
    if(state && !_echo)                                                        // Режим эхо, не просто маскирует посылаемые
        connect(_serial, &SerialGui::send, this, &Terminal::sended);      // данные, а фактически подписывает/отписывает
    else if (!state && _echo)                                                  // нас на исходящие данные порта
        disconnect(_serial, &SerialGui::send, this, &Terminal::sended);   //
    _echo = state;                                                             // Фиксируем состояние
    emit echoModeChanged(state);                                               // Уведомляем о изменении
    qDebug() << "\nConsoleWidget: echo mode changed: " << state;
}
void Terminal::setCyclicMode(bool mode){
    _cyclic = mode;
    if(_cyclic == false)
        timer->stop();
    emit cyclicModeChanged(mode);
    qDebug() << "\nConsoleWidget: cyclic mode changed: " << mode;
}
void Terminal::setCyclicInterval(int interval){
    if(interval > 0){
        _cyclicInterval = interval;
        emit cyclicIntervalChanged(interval);
        qDebug() << "\nConsoleWidget: cyclic interval changed: " << interval;
    }
}
void Terminal::setBindData(QString data){
    _bindData = data;
    emit bindDataChanged(data);
    qDebug() << "\nConsoleWidget: cyclic data changed" << data;
}
void Terminal::retranslate(void){
    _sendButton->setText(tr("Send"));
    _clearButton->setText(tr("Clear"));
}
void Terminal::replaceSymbols(QByteArray &data, const char symbol){
    for(int i = 0; i < data.count(); i++){
        if(((data[i] >= char(0x00) && data[i] < char(0x20))
           && data[i] != char(0x09)
           && data[i] != char(0x0A)
           && data[i] != char(0x0D))
           || data[i] == char(0x7F))
            data[i] = symbol;
    }
}
void Terminal::cyclicTimeout(void){
   send(_bindData);
}
void Terminal::startCyclicSending(void){
    send(_bindData);
    if(cyclicMode()){
        timer->start(cyclicInterval());
        emit cyclicIsRunning();
        qDebug() << "\nConsoleWidget: cyclic sending data started!";
    }
    else
        emit cyclicStopped();
}
void Terminal::stopCyclicSending(void){
    timer->stop();
    emit cyclicStopped();
    qDebug() << "\nConsoleWidget: cyclic sending data stopped!";
}
