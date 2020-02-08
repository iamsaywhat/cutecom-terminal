#include "decorator.h"


Decorator::Decorator()
{

}

QString Decorator::getMainWidgetStyleSheet(int backgroundColor,
                                           int textColor) {
    QString styleSheet (
            "QWidget { "
                "background-color: #%1;"
                "color:            #%2;"
                "border:           none;"
           "}");
    return styleSheet.arg(QString::number(backgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(textColor&0xFFFFFF, 16));
}

QString Decorator::getWindowButtonStyleSheet(QString iconUrl,
                                             int backgroundColor,
                                             int hoverBackgroundColor,
                                             int pressedBackgroundColor) {
    QString styleSheet (
           "QToolButton { "
                "image:            url(%1);"
                "background-color: #%2;"
                "icon-size:        12px;"
                "padding-left:     10px;"
                "padding-right:    10px;"
                "padding-top:      5px;"
                "padding-bottom:   5px;"
                "border:           none;"
           "}"
           "QToolButton:hover {"
                "background-color: #%3;"
           "}"
           "QToolButton:pressed { "
                "background-color: #%4;"
           "}");
    return styleSheet.arg(iconUrl).
                      arg(QString::number(backgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(hoverBackgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(pressedBackgroundColor&0xFFFFFF, 16));
}


QString Decorator::getUsualButtonStyleSheet (int backgroundColor,
                                             int textColor,
                                             int hoverBackgroundColor,
                                             int pressedBackgroundColor) {
    QString styleSheet (
           "QPushButton { "
                "background-color: #%1;"
                "color:            #%2;"
                "padding-left:     10px;"
                "padding-right:    10px;"
                "padding-top:      5px;"
                "padding-bottom:   5px;"
                "border:           none; "
           "}"
           "QPushButton:hover { "
                "background-color: #%3;"
           "}"
           "QPushButton:pressed { "
                "background-color: #%4; "
           "}");
    return styleSheet.arg(QString::number(backgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(textColor&0xFFFFFF, 16)).
                      arg(QString::number(hoverBackgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(pressedBackgroundColor&0xFFFFFF, 16));
}

QString Decorator::getInputFieldStyleSheet (int backgroundColor,
                                            int textColor,
                                            int selectionBackgroundColor,
                                            int selectionTextColor) {
    QString styleSheet (
                "QLineEdit { "
                    "background:                 #%1;"
                    "color:                      #%2;"
                    "border:                     none;"
                    "selection-background-color: #%3;"
                    "selection-color:            #%4;"
                "}"
                );
    return styleSheet.arg(QString::number(backgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(textColor&0xFFFFFF, 16)).
                      arg(QString::number(selectionBackgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(selectionTextColor&0xFFFFFF, 16));
}

QString Decorator::getScrollBarStyleSheet (int handleColor, int pageColor) {
    QString styleSheet (
           "QScrollBar:vertical { "
                "border: none;"
                "height: 10px;"
                "width:  10px;"
                "margin: 0;"
           "}"
           "QScrollBar::handle:vertical {"
                "background: #%1;"
                "min-width:  20px;"
                "min-height: 20px;"
           "}"
           "QScrollBar::add-line:vertical {"
                "background: none;"
                "border:     none; "
           "}"
           "QScrollBar::sub-line:vertical {"
                "background: none;"
                "border:     none;"
           "}"
           "QScrollBar::add-page:vertical {"
                "background: #%2;"
                "border:     none;"
           "}"
           "QScrollBar::sub-page:vertical {"
                "background: #%2;"
                "border:     none;"
           "}"
           "QScrollBar:horizontal { "
                "border: none;"
                "height: 10px;"
                "width:  10px;"
                "margin: 0;"
           "}"
           "QScrollBar::handle:horizontal {"
                "background: #%1;"
                "min-width:  20px;"
                "min-height: 20px;"
           "}"
           "QScrollBar::add-line:horizontal {"
                "background: none;"
                "border:     none;"
           "}"
           "QScrollBar::sub-line:horizontal {"
                "background: none;"
                "border:     none;"
           "}"
           "QScrollBar::add-page:horizontal {"
                "background: #%2;"
                "border:     none;"
           "}"
           "QScrollBar::sub-page:horizontal {"
                "background: #%2;"
                "border:     none;"
           "}"
                );
    return styleSheet.arg(QString::number(handleColor&0xFFFFFF, 16)).
                      arg(QString::number(pageColor&0xFFFFFF,16));
}


QString Decorator::getConsoleStyleSheet(int backgroundColor,
                                        int textColor,
                                        int selectionBackgroundColor,
                                        int selectionTextColor) {
    QString styleSheet (
           "QPlainTextEdit { "
                "background:                 #%1;"
                "color:                      #%2;"
                "selection-background-color: #%3;"
                "selection-color:            #%4;"
                "border-style:               solid;"       // Без этого свойства бэкграунд не устанавливается(??)
                "border:                     none;"
                "border-top-left-radius:     5px;"
           "}");
    return styleSheet.arg(QString::number(backgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(textColor&0xFFFFFF, 16)).
                      arg(QString::number(selectionBackgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(selectionTextColor&0xFFFFFF, 16));
}



QString Decorator::getQuickPanelButtonStyleSheet(QString iconUrl,
                                                 int backgroundColor,
                                                 int hoverBackgroundColor,
                                                 int pressedBackgroundColor) {
    QString styleSheet (
                "QPushButton { "
                "image:              url(%1);"
                "background-color:   #%2;"
                "icon-size:          12px;"
                "qproperty-iconSize: 12px;"
                "padding-left:       10px;"
                "padding-right:      10px;"
                "padding-top:        10px;"
                "padding-bottom:     10px;"
                "border:             none;"
           "}"
           "QPushButton:hover { "
                "background-color:   #%3;"
           "}"
           "QPushButton:pressed { "
                "background-color:   #%4;"
           "}");
    return styleSheet.arg(iconUrl).
                      arg(QString::number(backgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(hoverBackgroundColor&0xFFFFFF, 16)).
                      arg(QString::number(pressedBackgroundColor&0xFFFFFF, 16));
}



QString Decorator::getAppLabelStyleSheet(QString iconUrl){
    QString styleSheet (
            "QLabel {"
                "image: url(%1);"
            "}");
    return styleSheet.arg(iconUrl);
}

QString Decorator::getTableStyleSheet(int backgroundColor,
                                      int textColor,
                                      int selectionBackgroundColor,
                                      int selectionTextColor,
                                      int headerBackgroundColor) {
    QString styleSheet (
            "QTableView {"
                "background:                 #%1;"
                "color:                      #%2;"
                "selection-background-color: #%3;"
                "selection-color:            #%4;"
                "border:                     none;"
                "border-top-left-radius:     5px;"
            "}"
//            "QTableView QTableCornerButton::section {"
//                "background:                 red;"
//                "border:                     2px outset red;"
//            "}"
//            "QTableView::indicator:unchecked {"
//                "background-color:           red;" //#d7d6d5;"
//            "}"
              /* Горизонтальный хэдер таблицы */
            "QHeaderView::section:horizontal {"
                "background-color:           #%5;"
                "color:                      #%2;"
                "padding-left:               1px;"
                "border:                     none;"
                "border-top-left-radius:     5px;"
           "}"
            );
    return styleSheet.arg(QString::number(backgroundColor&0xFFFFFF, 16))
                     .arg(QString::number(textColor&0xFFFFFF, 16))
                     .arg(QString::number(selectionBackgroundColor&0xFFFFFF, 16))
                     .arg(QString::number(selectionTextColor&0xFFFFFF, 16))
                     .arg(QString::number(headerBackgroundColor&0xFFFFFF, 16));
}







QString Decorator::getSipmleWidgetStyleSheet(int backgroundColor,
                                             int textColor) {
   QString styleSheet(
            "QWidget { "
                "border:           none;"
                "border-style:     solid;"
                "background:       #%1;"
                "color:            #%2;"
            "}");
   return styleSheet.arg(QString::number(backgroundColor&0xFFFFFF, 16))
                    .arg(QString::number(textColor&0xFFFFFF, 16));
}

QString Decorator::getSettingsMenuButtonStyleSheet(int background, int backgroundHover, int backgroundPressed){
    QString styleSheet(
                "QPushButton { "
                       "background-color:       #%1;"
                       "padding-left:           40px;"
                       "padding-right:          40px;"
                       "padding-top:            10px;"
                       "padding-bottom:         10px;"
                       "border:                 none;"
                       "border-radius:          5px;"
                       "margin:                 2px;"
                  "}"
                  "QPushButton:hover { "
                       "background-color:       #%2;"
                  "}"
                  "QPushButton:pressed { "
                       "background-color:       #%3; "
                  "}");
    return styleSheet.arg(QString::number(background&0xFFFFFF, 16)).
                      arg(QString::number(backgroundHover&0xFFFFFF,16)).
                      arg(QString::number(backgroundPressed&0xFFFFFF, 16));
}

QString Decorator::getComboBoxStyleSheet(int background,
                                         int color,
                                         int selectionBackgroundColor,
                                         int selectionColor) {
    QString styleSheet(
            "QComboBox { "
                "border:                     0px solid gray; "
                "border-style:               solid;"
                "border-radius:              3px; "
                "background:                 #2b2d33;"
                "color:                      #dcddde;"
                "selection-background-color: #3d563d;"
                "selection-color:            #dcddde;"
            "}"
            /* Стиль виджета при редактировании элемента */
            "QComboBox:editable { "
                "background:                 #2b2d33;"
            "}"
            /* Выпадающий список целиком */
            "QComboBox QAbstractItemView { "
                "background:                 #2b2d33;"
                "color:                      #dcddde;"
                "selection-background-color: #3d563d;"
                "selection-color:            #dcddde;"
                "border:                     none; "   // Внешняя граница в выпадающем списке
                "outline:                    none;"    // Граница элемента списка
            "}"
            /* Работает только если установить для Combobox специальный делегат
               combobox->setItemDelegate(new QStyledItemDelegate(combobox));
               https://switch-case.ru/33415621 */
//            "QComboBox QAbstractItemView::item {"
//                "border-style:             solid;"
//                "border:                   none;"
//                "padding-left:             5px;"
//            "}"
//            "QComboBox QAbstractItemView::item:selected {"
//                "border-style:               solid;"
//                "border: none;"
//                "background: rgb(47, 175, 178);"
//                "padding-left: 5px;"
//            "}"
            /* Применяется для неизменяемого поля, и ненажатой кнопки */
            "QComboBox:!editable, QComboBox::drop-down:editable { "
            "}"
            /* Применяется для неизменяемого поля, и нажатой кнопки */
            "QComboBox:!editable:on, QComboBox::drop-down:editable:on { "
            "}"
            /* При открытом списке верхнее поле */
            "QComboBox:on { "
               "padding-top: 3px; "   // При раскрытии списка верхнее поле смещаем
               "padding-left: 4px;"
            "}"
            /* Кнопка раскрытия списка */
            "QComboBox::drop-down { "
                "width: 15px; "
                "border-left-width: 1px; "
                "border-left-color: darkgray; "
                "border-left-style: solid; "/* только одна линия */
                "border-top-right-radius: 8px; " /* тот же радиус закругления что и у QComboBox */
                "border-bottom-right-radius: 3px;"
            "}"
            /* Стрелка при закрытом списке */
            "QComboBox::down-arrow { "
            "}"
            /* Стрелка, когда список раскрыт */
            "QComboBox::down-arrow:on { "
            "}"
            );
    return styleSheet.arg(QString::number(background&0xFFFFFF, 16)).
                      arg(QString::number(background&0xFFFFFF, 16)).
                      arg(QString::number(background&0xFFFFFF, 16)).
                      arg(QString::number(background&0xFFFFFF, 16));
}
