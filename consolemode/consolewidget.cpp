#include "consolewidget.h"

#include <QString>
#include <QByteArray>
#include <QTime>
#include <QDebug>
#include "converter.h"

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
    _sendButton->setShortcut(Qt::Key_Return);  // Привязываем кнопки клавиатуры к кнопкам UI
    _sendButton->setText(tr("Send"));              // Разместим текст на кнопках
    _clearButton->setText(tr("Clear"));
    setEchoMode(true);
    /* Выполняем функциональные подключения */
    connect(_sendButton, &QPushButton::clicked, this, QOverload<>::of(&ConsoleWidget::send));
    connect(_clearButton,&QPushButton::clicked, this, &ConsoleWidget::clear);
    connect(_serial,     &SerialGui::received,  this, &ConsoleWidget::receive);

    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &ConsoleWidget::ciclicTimeout);
}

ConsoleWidget::~ConsoleWidget()
{
    delete timer;
}

void ConsoleWidget::send (void){
    QString data(_input->text());
    if(data == "" ||
       _serial->getConnectionStatus() == SerialGui::CLOSED)   // Если в поле ввода пусто,
        return;                                               // или порт закрыт, ничего не делаем

    _serial->send(Converter::convertFromUnicode(data).toLatin1());                // Отправляем данные преобразованные в QByteArray
    //_serial->send(data.toLocal8Bit());           // Отправляем данные преобразованные в QByteArray
    _input->clear();                               // Поле ввода очищаем
    _console->moveCursor(QTextCursor::End);        // Смещаем курсор текста гарантированно в конец
    if(_echo) {
        _console->textCursor().insertText(">> ");
        _console->textCursor().insertText(data);
        _console->textCursor().insertText("\n");
    }
}
void ConsoleWidget::clear (void){
    _console->clear();   // Очищаем окно терминала
}

void ConsoleWidget::receive(QByteArray data){
    replaceSymbols(data, '.');
    QString temp = Converter::convertToUnicode(data);
    _console->moveCursor(QTextCursor::End);            // Смещаем курсор текста гарантированно в конец
    _console->textCursor().insertText(temp);        // Печатаем то, что пришло
    //_console->textCursor().insertText(QString::fromLatin1(data));        // Печатаем то, что пришло
    //_console->textCursor().insertText("\n");           // Переведем курсор на следующую строку
}
bool ConsoleWidget::echoMode(void){
    return _echo;
}
void ConsoleWidget::setEchoMode(bool state){
    _echo = state;
    emit echoModeChanged(state);
}
bool ConsoleWidget::ciclicMode(void){
    return _ciclic;
}
void ConsoleWidget::setCiclicMode(bool mode){
    _ciclic = mode;
    if(_ciclic == false)
        timer->stop();
    emit ciclicModeChanged(mode);
}
int ConsoleWidget::ciclicInterval(void){
    return _ciclicInterval;
}
void ConsoleWidget::setCiclicInterval(int interval){
    _ciclicInterval = interval;
    emit ciclicIntervalChanged(interval);
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
void ConsoleWidget::addBindSet(QLineEdit* textField, QToolButton* button){
    bindTextFields.append(textField);
    bindButtons.append(button);
    connect(button, &QToolButton::clicked, this, &ConsoleWidget::sendBind);
}
void ConsoleWidget::sendBind(void){
    int index = bindButtons.indexOf(static_cast<QToolButton*>(QObject::sender()));   // Определяем отправителя
    if(index == -1 ||                                                                // Если отпраитель не был найден в списке
       _serial->getConnectionStatus() == SerialGui::CLOSED ||                        // или если порт закрыт
       bindTextFields.at(index)->text().isEmpty())                                   // или если строка пуска
        return;                                                                      // Выходим

    QString data = bindTextFields.at(index)->text();

    _serial->send(Converter::convertFromUnicode(data).toLatin1());   // Отправляем данные преобразованные в QByteArray
    _console->moveCursor(QTextCursor::End);                          // Смещаем курсор текста гарантированно в конец

    if(echoMode()) {
        _console->textCursor().insertText(">> ");
        _console->textCursor().insertText(data);
        _console->textCursor().insertText("\n");
    }
    if(ciclicMode()) {
        ciclicButtonIndex = index;
        timer->setInterval(ciclicInterval());
        timer->start();
    }
}
void ConsoleWidget::ciclicTimeout(void){
    emit bindButtons.at(ciclicButtonIndex)->clicked();
}
