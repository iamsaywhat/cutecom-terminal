#include "ConsoleWindow.h"

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QTime>

ConsoleWindow::ConsoleWindow(SerialForGUI*   Serial,
                             QPlainTextEdit* Workspace,
                             QLineEdit *     Message,
                             QPushButton*    SendButton,
                             QPushButton*    ClearButton)
{
    ConsoleWindow::Serial      = Serial;
    ConsoleWindow::Workspace   = Workspace;
    ConsoleWindow::Message     = Message;
    ConsoleWindow::SendButton  = SendButton;
    ConsoleWindow::ClearButton = ClearButton;

    // Настраиваем свойства и внешний вид рабочих областей
    // Запрещаем пользователю редактировать консоль
    Workspace->setReadOnly(true);
    // Внешний вид
    Workspace->setFont(QFont("System", 8, QFont::Normal));
    Workspace->setStyleSheet("QPlainTextEdit{color: #aaaaaa; background-color: #000000;"
                              " selection-background-color: #606060; selection-color: #ffffff;}");

    // Привязываем кнопки клавиатуры к кнопкам UI
    SendButton->setShortcut(Qt::Key_Return);

    //
    connect(SendButton,  &QPushButton::clicked,   this, &ConsoleWindow::sendMessage);
    connect(ClearButton, &QPushButton::clicked,   this, &ConsoleWindow::clearWorkspace);
    connect(Serial,      &QSerialPort::readyRead, this, &ConsoleWindow::readMessage);
}


void ConsoleWindow::sendMessage (void)
{
    QString Msg(Message->text());
    // Если в поле ввода пусто, или порт не открыт, ничего не делаем
    if(Msg == "" || Serial->getConnectionStatus() == CLOSED)
        return;

    qDebug() << "Send: " << Msg;
    Serial->write(Msg.toLocal8Bit());
    Serial->waitForBytesWritten(100);
    Message->clear();

    // Смещаем курсор текста гарантированно в конец
    Workspace->moveCursor(QTextCursor::End);
    Workspace->textCursor().insertText(">> ");
    Workspace->textCursor().insertText(Msg);
    Workspace->textCursor().insertText("\n");
}

void ConsoleWindow::clearWorkspace (void)
{
    Workspace->clear();
}

void ConsoleWindow::readMessage(void)
{
    // По рекомендациям втыкаем ожидание перед считыванием
    //Serial->waitForReadyRead(1);
    // Принимаем данные и сразу преобразуем в строку
    QString message = QString::fromLocal8Bit(Serial->readAll());
    // Смещаем курсор текста гарантированно в конец
    Workspace->moveCursor(QTextCursor::End);
    // Печатаем то, что пришло
    Workspace->textCursor().insertText(message);
    // Переведем курсор на следующую строку
    Workspace->textCursor().insertText("\n");
    // Выведем пришедшее сообщение
    qDebug() << "Comming data:" << message;
}
