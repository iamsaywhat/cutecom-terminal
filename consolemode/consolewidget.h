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
    Q_PROPERTY(bool cyclicMode READ cyclicMode WRITE setCyclicMode NOTIFY cyclicModeChanged)
    Q_PROPERTY(int cyclicInterval READ cyclicInterval WRITE setCyclicInterval NOTIFY cyclicIntervalChanged)

public:
    explicit ConsoleWidget(QObject*        parent,
                           SerialGui*      serial,
                           QPlainTextEdit* console,
                           QLineEdit *     input,
                           QPushButton*    sendButton,
                           QPushButton*    clearButton);
    ~ConsoleWidget();
    bool echoMode (void);
    bool cyclicMode(void);
    int  cyclicInterval(void);
    void retranslate(void);
    void addBindSet(QLineEdit*, QToolButton*);

private:
    QTimer              *timer;
    int                 cyclicButtonIndex;
    QList<QLineEdit*>   bindTextFields;
    QList<QToolButton*> bindButtons;
    SerialGui*          _serial;
    QPlainTextEdit*     _console;
    QLineEdit *         _input;
    QPushButton*        _sendButton;
    QPushButton*        _clearButton;
    bool                _echo   = true;
    bool                _cyclic = false;
    int                 _cyclicInterval = 1000;
    void replaceSymbols(QByteArray &data, const char symbol);

signals:
    void echoModeChanged(bool);
    void cyclicModeChanged(bool);
    void cyclicIntervalChanged(int);

public slots:
    void setEchoMode(bool);
    void setCyclicMode(bool);
    void setCyclicInterval(int);

private slots:
    void send(void);
    void clear(void);
    void sended(QByteArray data);
    void received(QByteArray);
    void sendBind(void);
    void cyclicTimeout(void);
};

#endif // CONSOLEWIDGET_H
