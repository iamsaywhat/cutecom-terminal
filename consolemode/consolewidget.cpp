#include "consolewidget.h"

#include <QString>
#include <QByteArray>
#include <QTime>

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
    _sendButton->setText("Send");              // Разместим текст на кнопках
    _clearButton->setText("Clear");
    /* Выполняем функциональные подключения */
    connect(_sendButton, &QPushButton::clicked, this, &ConsoleWidget::send);
    connect(_clearButton,&QPushButton::clicked, this, &ConsoleWidget::clear);
    connect(_serial,     &SerialGui::received,  this, &ConsoleWidget::receive);
}

ConsoleWidget::~ConsoleWidget()
{
}

void ConsoleWidget::send (void){
    QString data(_input->text());
    if(data == "" ||
       _serial->getConnectionStatus() == SerialGui::CLOSED)   // Если в поле ввода пусто,
        return;                                               // или порт закрыт, ничего не делаем

    _serial->send(data.toLocal8Bit());             // Отправляем данные преобразованные в QByteArray
    _input->clear();                               // Поле ввода очищаем
    _console->moveCursor(QTextCursor::End);        // Смещаем курсор текста гарантированно в конец
    _console->textCursor().insertText(">> ");
    _console->textCursor().insertText(data);
    _console->textCursor().insertText("\n");
}

void ConsoleWidget::clear (void){
    _console->clear();   // Очищаем окно терминала
}

void ConsoleWidget::receive(QByteArray data){
    _console->moveCursor(QTextCursor::End);            // Смещаем курсор текста гарантированно в конец
    _console->textCursor().insertText(QString::fromLatin1(data));        // Печатаем то, что пришло
    _console->textCursor().insertText("\n");           // Переведем курсор на следующую строку
}
