#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

#include "serialgui.h"

class ConsoleWidget : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)
    Q_PROPERTY(bool ciclicMode READ ciclicMode WRITE setCiclicMode NOTIFY ciclicModeChanged)
    Q_PROPERTY(int ciclicInterval READ ciclicInterval WRITE setCiclicInterval NOTIFY ciclicIntervalChanged)

public:
    explicit ConsoleWidget(QObject*        parent,
                           SerialGui*      serial,
                           QPlainTextEdit* console,
                           QLineEdit *     input,
                           QPushButton*    sendButton,
                           QPushButton*    clearButton);
    ~ConsoleWidget();
    bool echoMode (void);
    bool ciclicMode(void);
    int  ciclicInterval(void);
    void retranslate(void);
    void addBindSet(QLineEdit*, QToolButton*);

private:
    QTimer              *timer;
    int                 ciclicButtonIndex;
    QList<QLineEdit*>   bindTextFields;
    QList<QToolButton*> bindButtons;
    SerialGui*          _serial;
    QPlainTextEdit*     _console;
    QLineEdit *         _input;
    QPushButton*        _sendButton;
    QPushButton*        _clearButton;
    bool                _echo   = true;
    bool                _ciclic = true;
    int                 _ciclicInterval = 1000;
    void replaceSymbols(QByteArray &data, const char symbol);

signals:
    void echoModeChanged(bool);
    void ciclicModeChanged(bool);
    void ciclicIntervalChanged(int);

public slots:
    void setEchoMode(bool);
    void setCiclicMode(bool);
    void setCiclicInterval(int);

private slots:
    void send(void);
    void clear(void);
    void receive(QByteArray);
    void sendBind(void);
    void ciclicTimeout(void);
};

#endif // CONSOLEWIDGET_H
