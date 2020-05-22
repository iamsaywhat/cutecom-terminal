#ifndef TERMINAL_H
#define TERMINAL_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

#include "serialgui.h"

class Terminal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)
    Q_PROPERTY(bool cyclicMode READ cyclicMode WRITE setCyclicMode NOTIFY cyclicModeChanged)
    Q_PROPERTY(int cyclicInterval READ cyclicInterval WRITE setCyclicInterval NOTIFY cyclicIntervalChanged)
    Q_PROPERTY(QString bindData READ bindData WRITE setBindData NOTIFY bindDataChanged)

public:
    explicit Terminal(QObject*        parent,
                           SerialGui*      serial,
                           QPlainTextEdit* console,
                           QLineEdit *     input,
                           QPushButton*    sendButton,
                           QPushButton*    clearButton);
    ~Terminal();
    bool echoMode (void);
    bool cyclicMode(void);
    int  cyclicInterval(void);
    QString& bindData(void);
    void retranslate(void);

private:
    QTimer              *timer;
    SerialGui*          _serial;
    QPlainTextEdit*     _console;
    QLineEdit *         _input;
    QPushButton*        _sendButton;
    QPushButton*        _clearButton;
    bool                _echo   = false;
    bool                _cyclic = false;
    int                 _cyclicInterval = 1000;
    QString             _bindData;
    void replaceSymbols(QByteArray &data, const char symbol);

signals:
    void echoModeChanged(bool);
    void cyclicModeChanged(bool);
    void cyclicIntervalChanged(int);
    void bindDataChanged(QString&);
    void cyclicIsRunning(void);
    void cyclicStopped(void);

public slots:
    void setEchoMode(bool);
    void setCyclicMode(bool);
    void setCyclicInterval(int);
    void setBindData(QString);
    void startCyclicSending(void);
    void stopCyclicSending(void);

private slots:
    void send(void);
    void send(QString);
    void clear(void);
    void sended(QByteArray data);
    void received(QByteArray);
    void cyclicTimeout(void);
};

#endif // TERMINAL_H
