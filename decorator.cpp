#include "decorator.h"

/*
 * 212121 Основной фон
 * */


Decorator::Decorator()
{

}

QString Decorator::getMainWidgetStyleSheet() {
    return "QWidget { "
                "background-color: #212121; "
//                "border: 1px solid black; "
           "}";
}

QString Decorator::getCloseButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/close.png);"
                "background-color: #212121; "
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 0px solid #292929; "
           "}"
           "QToolButton:hover {"
//                "image: url(:/buttons/close.png); "
                "background-color: #23282B;"
           "}"
           "QToolButton:pressed { "
                 "image: url(:/buttons/close.png);"
                 "background-color: #292929; "
           "}";
}

QString Decorator::getMaximizeButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/maximize.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 0px solid #292929; "
           "}"
           "QToolButton:hover {"
//                "image: url(:/buttons/window-maximize.png); "
                "background-color: #23282B;"
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-maximize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getRestoreButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/buttons/window-restore-gray.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 0px solid #292929; "
           "}"
           "QToolButton:hover {"
//                "image: url(:/buttons/window-restore.png); "
                "background-color: #23282B;"
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-restore.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getMinimizeButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/minimize.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 0px solid #292929; "
           "}"
           "QToolButton:hover { "
//                "image: url(:/buttons/window-minimize.png); "
                  "background-color: #23282B;"
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-minimize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getConnectionButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/connect.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "qproperty-iconSize: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 0px solid #292929; "
           "}"
           "QToolButton:hover { "
//                "image: url(:/buttons/window-minimize.png); "
                "background-color: #23282B;"
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-minimize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getSettingsButtonStyleSheet() {
    return "QToolButton { "
                "image: url(:/icons/resources/settings.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 5px;"
                "padding-bottom: 5px;"
                "border: 0px solid #292929; "
           "}"
           "QToolButton:hover { "
//                "image: url(:/buttons/window-minimize.png); "
                "background-color: #23282B;"
           "}"
           "QToolButton:pressed { "
                "image: url(:/buttons/window-minimize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getUsualButtonStyleSheet (void) {
    return "QPushButton { "
                "background-color: #212121;"
                "padding-left:   10px;"
                "padding-right:  10px;"
                "padding-top:    10px;"
                "padding-bottom: 10px;"
                "border: 0px solid #292929; "
           "}";
//           "QPushButton:hover { "
//           "}"
//           "QPushButton:pressed { "
//           "}";
}

QString Decorator::getInputFieldStyleSheet (void) {
    return "QLineEdit { "
                "background: #000000; "
                "color:      #aaaaaa; "
                "border:     0px; "
           "}";
}

QString Decorator::getScrollBarStyleSheet (void) {
    return "QScrollBar:vertical { "
                "border: none;"
                "height: 10px;"
                "width:  10px;"
                "margin: 0;"
           "}"
           "QScrollBar::handle:vertical {"
                "background: #212121; "
                "min-width:  20px; "
                "min-height: 20px; "

            "}"
            "QScrollBar::add-line:vertical {"
                "background: none;"
                "border:     none; "
            "}"
            "QScrollBar::sub-line:vertical {"
                "background: none; "
                "border:     none; "
            "}"
            "QScrollBar::add-page:vertical {"
                "background: #000000;"
                "border:     none; "
            "}"
            "QScrollBar::sub-page:vertical {"
                "background: #000000; "
                "border:     none; "
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
