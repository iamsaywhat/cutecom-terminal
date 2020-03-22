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
    Q_PROPERTY(bool echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)

public:
    explicit ConsoleWidget(QObject*        parent,
                           SerialGui*      serial,
                           QPlainTextEdit* console,
                           QLineEdit *     input,
                           QPushButton*    sendButton,
                           QPushButton*    clearButton);
    ~ConsoleWidget();
    bool echoMode (void);
    void retranslate(void);

private:
    SerialGui*      _serial;
    QPlainTextEdit* _console;
    QLineEdit *     _input;
    QPushButton*    _sendButton;
    QPushButton*    _clearButton;
    bool            _echo;


signals:
    void echoModeChanged(bool);

public slots:
    void setEchoMode(bool);

private slots:
    void send(void);
    void clear(void);
    void receive(QByteArray);
};

#endif // CONSOLEWIDGET_H
