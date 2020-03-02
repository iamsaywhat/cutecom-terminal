#include "consolewidget.h"

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QTime>

ConsoleWidget::ConsoleWidget(QObject*        parent,
                             SerialGui*      serial,
                             QPlainTextEdit* workspace,
                             QLineEdit *     message,
                             QPushButton*    sendButton,
                             QPushButton*    clearButton) : QObject(parent)
{
    /* Сохраняем указатели на формы и прочие классы внутри */
    _serial      = serial;
    _workspace   = workspace;
    _message     = message;
    _sendButton  = sendButton;
    _clearButton = clearButton;
    /* Настраиваем свойства и внешний вид рабочих областей */
    /* Запрещаем пользователю редактировать консоль */
    _workspace->setReadOnly(true);
    /* Привязываем кнопки клавиатуры к кнопкам UI */
    _sendButton->setShortcut(Qt::Key_Return);
    /* Разместим текст на кнопках */
    _sendButton->setText("Send");
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
    QString data(_message->text());
    if(data == "" ||
       _serial->getConnectionStatus() == SerialGui::CLOSED)   // Если в поле ввода пусто,
        return;                                               // или порт закрыт, ничего не делаем

    _serial->send(data.toLocal8Bit());             // Отправляем данные преобразованные в QByteArray
    _message->clear();                             // Поле ввода очищаем
    _workspace->moveCursor(QTextCursor::End);      // Смещаем курсор текста гарантированно в конец
    _workspace->textCursor().insertText(">> ");
    _workspace->textCursor().insertText(data);
    _workspace->textCursor().insertText("\n");
}

void ConsoleWidget::clear (void){
    _workspace->clear();   // Очищаем окно терминала
}

void ConsoleWidget::receive(QByteArray data){
    QString message = QString::fromLocal8Bit(data);      // Принимаем данные и сразу преобразуем в строку
    _workspace->moveCursor(QTextCursor::End);            // Смещаем курсор текста гарантированно в конец
    _workspace->textCursor().insertText(message);        // Печатаем то, что пришло
    _workspace->textCursor().insertText("\n");           // Переведем курсор на следующую строку
}
