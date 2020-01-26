#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "consolemode/ConsoleWindow.h"
#include "hexmode/tableconsole.h"
#include "SerialForGUI.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void setPreviousPosition(QPoint previousPosition);

signals:
    void previousPositionChanged(QPoint previousPosition);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::MainWindow       *ui;
    SettingsDialog       *settingsWindow;
    SerialForGUI         *serial;
    ConsoleWindow        *console;
    TableConsole         *tableConsole;

    enum ClickType {
        None,
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Move
     };
    /* Переменная, от которой будем отталкиваться при работе с перемещением и изменением размера окна */
    ClickType m_leftMouseButtonPressed;
    QPoint    m_previousPosition;
    QRect     previousWindowSize;
    ClickType checkResizableField(QMouseEvent *event);
};
#endif // MAINWINDOW_H
