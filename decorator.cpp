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


QString Decorator::getUsualButtonStyleSheet (void) {
    return "QPushButton { "
                "background-color: #212121;"
                "color:            #aaaaaa;"
                "padding-left:     10px;"
                "padding-right:    10px;"
                "padding-top:      5px;"
                "padding-bottom:   5px;"
                "border:           none; "
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
                "border:     none; "
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
           "}"
           "QScrollBar:horizontal { "
                "border: none;"
                "height: 10px;"
                "width:  10px;"
                "margin: 0;"
           "}"
           "QScrollBar::handle:horizontal {"
                "background: #212121; "
                "min-width:  20px; "
                "min-height: 20px; "
           "}"
           "QScrollBar::add-line:horizontal {"
                "background: none;"
                "border:     none; "
           "}"
           "QScrollBar::sub-line:horizontal {"
                "background: none; "
                "border:     none; "
           "}"
           "QScrollBar::add-page:horizontal {"
                "background: #000000;"
                "border:     none; "
           "}"
           "QScrollBar::sub-page:horizontal {"
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

QString Decorator::getComboBoxStyleSheet(void) {
    return
            "QComboBox { "
                "border: 1px solid gray; "
                "border-radius: 3px; "
                "padding: 1px 18px 1px 3px;"
                "min-width: 6em; "
            "}"
            "QComboBox:editable { "
                "background: black;"
            "}"
            "QComboBox QAbstractItemView { "
                "border: 2px solid darkgray; "
                "background:                 #212121;"
                "color:                      #aaaaaa;"
                "selection-background-color: #161A1E;"
                "selection-color:            #aaaaaa;"
            "}"
            "QComboBox:!editable, QComboBox::drop-down:editable { "
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                            "stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,"
                                            "stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
            "}"
            "QComboBox:!editable:on, QComboBox::drop-down:editable:on { "
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                            "stop: 0 #D3D3D3, stop: 0.4 #D8D8D8,"
                                            "stop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1);"
            "}"
            "QComboBox:on { "
                "padding-top: 3px; "
                "padding-left: 4px;"
            "}"
            /* */
            "QComboBox::drop-down { "
                 "subcontrol-origin: padding; "
                 "subcontrol-position: top right;"
                 "width: 15px; "
                 "border-left-width: 1px; "
                 "border-left-color: darkgray; "
                 "border-left-style: solid; "/* только одна линия */
                 "border-top-right-radius: 8px; " /* тот же радиус закругления что и у QComboBox */
                 "border-bottom-right-radius: 3px;"
            "}"
            /* Стрелка, при закрытом списке */
            "QComboBox::down-arrow { "
                 "image: url(/usr/share/icons/crystalsvg/16x16/actions/1downarrow.png);"
            "}"
            /* Стрелка, когда список раскрыт */
            "QComboBox::down-arrow:on { "
                "top: 1px;"
                "left: 1px;"
            "}"
            "QComboBox QListView:editable {"
                "background: black;"
            "}"
            ;
}


QString Decorator::getConnectionButtonStyleSheet(void) {
    return "QPushButton { "
                "image: url(:/icons/resources/connect.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "qproperty-iconSize: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 10px;"
                "padding-bottom: 10px;"
                "border: 0px solid #292929; "
           "}"
           "QPushButton:hover { "
//                "image: url(:/buttons/window-minimize.png); "
                "background-color: #23282B;"
           "}"
           "QPushButton:pressed { "
//                "image: url(:/buttons/window-minimize.png);"
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getSettingsButtonStyleSheet(void) {
    return "QPushButton { "
                "image: url(:/icons/resources/settings.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "qproperty-iconSize: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 10px;"
                "padding-bottom: 10px;"
                "border: 0px solid #292929; "
           "}"
           "QPushButton:hover { "
                "background-color: #23282B;"
           "}"
           "QPushButton:pressed { "
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getConsoleModeButtonStyleSheet(void) {
    return "QPushButton { "
                "image: url(:/icons/resources/consolemode.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "qproperty-iconSize: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 10px;"
                "padding-bottom: 10px;"
                "border: 0px solid #292929; "
           "}"
           "QPushButton:hover { "
                "background-color: #23282B;"
           "}"
           "QPushButton:pressed { "
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getTableModeButtonStyleSheet(void) {
    return "QPushButton { "
                "image: url(:/icons/resources/tablemode.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "qproperty-iconSize: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 10px;"
                "padding-bottom: 10px;"
                "border: 0px solid #292929; "
           "}"
           "QPushButton:hover { "
                "background-color: #23282B;"
           "}"
           "QPushButton:pressed { "
                "background-color: #de8e37; "
           "}";
}
QString Decorator::getConverterModeButtonStyleSheet(void) {
    return "QPushButton { "
                "image: url(:/icons/resources/converter.png);"
                "background-color: #212121;"
                "icon-size: 12px;"
                "qproperty-iconSize: 12px;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "padding-top: 10px;"
                "padding-bottom: 10px;"
                //"border: 0px solid #292929; "
            //"border: none;"
            "border-width: 1px;"
            "border-style: solid;"
            "border-radius: 20px;"
           "}"
           "QPushButton:hover { "
                "background-color: #23282B;"
           "}"
           "QPushButton:pressed { "
                "background-color: #de8e37; "
           "}";
}

QString Decorator::getAppLabelStyleSheet(void){
    return "QLabel {"
                "image: url(:/icons/resources/label.png);"
            "}";
}

QString Decorator::getTableStyleSheet(void) {
    return "QTableView {"
                "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5,"
                                        "stop: 0 #FF92BB, stop: 1 white);"
            "}"
            "QTableView QTableCornerButton::section {"
                "background: red;"
                "border: 2px outset red;"
            "}"
            "QTableView::indicator:unchecked {"
                "background-color: #d7d6d5;"
            "}"
            "QHeaderView::section:horizontal {"
                "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                                  "stop:0 #616161, stop: 0.5 #505050,"
                                                  "stop: 0.6 #434343, stop:1 #656565);"
                "color: white;"
                "padding-left: 4px;"
                "border: 1px solid #6c6c6c;"
            "}";
}
