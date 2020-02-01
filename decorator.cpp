#include "decorator.h"

Decorator::Decorator()
{

}

QString Decorator::getMainWidgetStyleSheet() {
    return "QWidget { "
                "background-color: #33363d; "
//                "border: 1px solid black; "
           "}";
}



QString Decorator::getCloseButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/close.png);"
                "background-color: #292929; "
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 1px solid #292929; "
           "}"
           "QToolButton:hover {"
                "image: url(:/buttons/close.png); "
           "}"
           "QToolButton:pressed { "
                 "image: url(:/buttons/close.png);"
                 "background-color: #F34723; "
           "}";
}

QString Decorator::getMaximizeButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/maximize.png);"
                "background-color: #292929;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 1px solid #292929; "
           "}"
           "QToolButton:hover {"
                "image: url(:/buttons/window-maximize.png); "
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-maximize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getRestoreButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/buttons/window-restore-gray.png);"
                "background-color: #292929;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 1px solid #292929; "
           "}"
           "QToolButton:hover {"
                "image: url(:/buttons/window-restore.png); "
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-restore.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getMinimizeButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/minimize.png);"
                "background-color: #292929;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 1px solid #292929; "
           "}"
           "QToolButton:hover { "
                "image: url(:/buttons/window-minimize.png); "
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-minimize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getConnectionButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/connect.png);"
                "background-color: #292929;"
                "icon-size: 20px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 1px solid #292929; "
           "}"
           "QToolButton:hover { "
                "image: url(:/buttons/window-minimize.png); "
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-minimize.png);"
                "background-color: #de8e37; "
           "}";
}


QString Decorator::getSettingsButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/settings.png);"
                "background-color: #292929;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 1px solid #292929; "
           "}"
           "QToolButton:hover { "
                "image: url(:/buttons/window-minimize.png); "
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-minimize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getConsoleStyleSheet() {
    return "QPlainTextEdit { "
                "background: #000000; "
                "color: #aaaaaa; "
                "selection-background-color: #606060;"
                "selection-color: #ffffff;"
                "border-style:solid;"
//                "border-width: 3px;"
//                "border-radius: 10px;"
//                "border-color: #2D2DF0;"
           "}";

}
