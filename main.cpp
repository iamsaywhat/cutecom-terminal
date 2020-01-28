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
    FramelessWindow f;
    MainWidget w(&f);
    f.show();
    f.setCentralWidget(&w);

    return a.exec();
}
