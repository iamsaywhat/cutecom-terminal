#include "consolewidget.h"

#include <QString>
#include <QByteArray>
#include <QTime>
#include <QDebug>
#include "converter.h"
#include <QScrollBar>

ConsoleWidget::ConsoleWidget(QObject*        parent,
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
    connect(_sendButton,  &QPushButton::clicked, this, QOverload<>::of(&ConsoleWidget::send));
    connect(_clearButton, &QPushButton::clicked, this, &ConsoleWidget::clear);
    connect(_serial,      &SerialGui::received,  this, &ConsoleWidget::received);

    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &ConsoleWidget::cyclicTimeout);
    setEchoMode(true);
}

ConsoleWidget::~ConsoleWidget(){
    delete timer;
}

void ConsoleWidget::send(void){
    QString data(_input->text());                                            // Берем текст с поля ввода
    if(data != "" && _serial->getConnectionStatus() != SerialGui::CLOSED) {  // Только если поле ввода что-то содержит
        _serial->send(Converter::convertFromUnicode(data).toLatin1());       // и порт открыт, посылаем данные в порт
        _input->clear();                                                     // и очищаем поле ввода
    }
}
void ConsoleWidget::send(QString data){
    if(data != "" && _serial->getConnectionStatus() != SerialGui::CLOSED)  // Только если поле ввода что-то содержит
        _serial->send(Converter::convertFromUnicode(data).toLatin1());       // и порт открыт, посылаем данные в порт
}
void ConsoleWidget::sended(QByteArray data){
    replaceSymbols(data, '.');                         // Необходимо непечатные символы заменить
    QString text = Converter::convertToUnicode(data);  // Необходимо, массив байт представить в текущей кодировке
    _console->moveCursor(QTextCursor::End);            // Смещаем курсор текста гарантированно в конец
    _console->textCursor().insertText(">> ");          // Признак исходящих данных
    _console->textCursor().insertText(text);           // Печатаем то, что пришло
    _console->textCursor().insertText("\n");           // Переходим на следующую строку
    _console->verticalScrollBar()->setSliderPosition(_console->verticalScrollBar()->maximum());
}
void ConsoleWidget::received(QByteArray data){
    replaceSymbols(data, '.');                        // Необходимо непечатные символы заменить
    QString text = Converter::convertToUnicode(data); // Необходимо, массив байт представить в текущей кодировке
    _console->moveCursor(QTextCursor::End);           // Смещаем курсор текста гарантированно в конец
    _console->textCursor().insertText(text);          // Печатаем то, что пришло
    _console->verticalScrollBar()->setSliderPosition(_console->verticalScrollBar()->maximum());
}
void ConsoleWidget::clear (void){
    _console->clear();
}
bool ConsoleWidget::echoMode(void){
    return _echo;
}
bool ConsoleWidget::cyclicMode(void){
    return _cyclic;
}
int ConsoleWidget::cyclicInterval(void){
    return _cyclicInterval;
}
QString& ConsoleWidget::bindData(void){
    return _bindData;
}
void ConsoleWidget::setEchoMode(bool state){
    if(state && !_echo)                                                        // Режим эхо, не просто маскирует посылаемые
        connect(_serial, &SerialGui::send, this, &ConsoleWidget::sended);      // данные, а фактически подписывает/отписывает
    else if (!state && _echo)                                                  // нас на исходящие данные порта
        disconnect(_serial, &SerialGui::send, this, &ConsoleWidget::sended);   //
    _echo = state;                                                             // Фиксируем состояние
    emit echoModeChanged(state);                                               // Уведомляем о изменении
}
void ConsoleWidget::setCyclicMode(bool mode){
    _cyclic = mode;
    if(_cyclic == false)
        timer->stop();
    emit cyclicModeChanged(mode);
}
void ConsoleWidget::setCyclicInterval(int interval){
    if(interval > 0){
        _cyclicInterval = interval;
        emit cyclicIntervalChanged(interval);
    }
}
void ConsoleWidget::setBindData(QString data){
    _bindData = data;
    emit bindDataChanged(data);
}
void ConsoleWidget::retranslate(void){
    _sendButton->setText(tr("Send"));
    _clearButton->setText(tr("Clear"));
}
void ConsoleWidget::replaceSymbols(QByteArray &data, const char symbol){
    for(int i = 0; i < data.count(); i++){
        if(((data[i] >= char(0x00) && data[i] < char(0x20))
           && data[i] != char(0x09)
           && data[i] != char(0x0A)
           && data[i] != char(0x0D))
           || data[i] == char(0x7F))
            data[i] = symbol;
    }
}
void ConsoleWidget::cyclicTimeout(void){
   send(_bindData);
}
void ConsoleWidget::startCyclicSending(void){
    send(_bindData);
    if(cyclicMode()){
        timer->start(cyclicInterval());
        emit cyclicIsRunning();
    }
    else
        emit cyclicStopped();
}
void ConsoleWidget::stopCyclicSending(void){
    timer->stop();
    emit cyclicStopped();
}
