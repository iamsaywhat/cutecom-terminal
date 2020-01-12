#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "SerialForGUI.h"

class ConsoleWindow : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleWindow(QObject*        parent,
                           SerialForGUI*   Serial,
                           QPlainTextEdit* Workspace,
                           QLineEdit *     Message,
                           QPushButton*    SendButton,
                           QPushButton*    ClearButton);
    ~ConsoleWindow();
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

#endif // CONSOLEWINDOW_H
