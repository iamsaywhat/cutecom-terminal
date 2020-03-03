#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "serialgui.h"

class ConsoleWidget : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QObject*        parent,
                           SerialGui*      serial,
                           QPlainTextEdit* console,
                           QLineEdit *     input,
                           QPushButton*    sendButton,
                           QPushButton*    clearButton);
    ~ConsoleWidget();
private:
    SerialGui*      _serial;
    QPlainTextEdit* _console;
    QLineEdit *     _input;
    QPushButton*    _sendButton;
    QPushButton*    _clearButton;

public slots:

private slots:
    void send(void);
    void clear(void);
    void receive(QByteArray);
};

#endif // CONSOLEWIDGET_H
