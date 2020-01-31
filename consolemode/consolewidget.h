#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "SerialForGUI.h"

class ConsoleWidget : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QObject*        parent,
                           SerialForGUI*   Serial,
                           QPlainTextEdit* Workspace,
                           QLineEdit *     Message,
                           QPushButton*    SendButton,
                           QPushButton*    ClearButton);
    ~ConsoleWidget();
private:
    SerialForGUI*   Serial;
    QPlainTextEdit* Workspace;
    QLineEdit *     Message;
    QPushButton*    SendButton;
    QPushButton*    ClearButton;

public slots:

private slots:
    void send(void);
    void clear(void);
    void receive(void);
};

#endif // CONSOLEWIDGET_H
