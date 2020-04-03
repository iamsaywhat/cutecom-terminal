#include "mainwidget.h"
#include "framelesswindow.h"

#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QWidget>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   
    FramelessWindow frameless;
    frameless.setRememberFrameProperties(true);
    MainWidget widget(&frameless);
    frameless.setCentralWidget(&widget);
    frameless.show();
    return a.exec();
}
