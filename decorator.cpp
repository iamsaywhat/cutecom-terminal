#include "decorator.h"


Decorator::Decorator(int base, int second, int third, int text, int selection)
{
    /* Пути к иконкам */
    appIconlUrl       = new QString(":/icons/resources/label.png");
    closeIconUrl      = new QString(":/icons/resources/close.png");
    maximizeIconUrl   = new QString(":/icons/resources/maximize.png");
    minimizeIconUrl   = new QString(":/icons/resources/minimize.png");
    connectionIconUrl = new QString(":/icons/resources/connect.png");
    consoleIconUrl    = new QString(":/icons/resources/consolemode.png");
    tableIconUrl      = new QString(":/icons/resources/tablemode.png");
    converterIconUrl  = new QString(":/icons/resources/converter.png");
    settingsIconUrl   = new QString(":/icons/resources/settings.png");

    /* Устанавливаем основные цвета */
    setBaseColor(base);
    setSecondColor(second);
    setThirdColor(third);
    setTextColor(text);
    setSelectionColor(selection);

    /* Остальные цвета по-умолчанию такие */
    setSelectionTextColor(textColor());
    setCloseHoverColor(0xea4445);
    setClosePressedColor(0xac4042);
    setMaximizeHoverColor(thirdColor());
    setMaximizePressedColor(secondColor());
    setMinimizeHoverColor(thirdColor());
    setMinimizePressedColor(secondColor());
}

Decorator::~Decorator(){
    delete appIconlUrl;
    delete closeIconUrl;
    delete maximizeIconUrl;
    delete minimizeIconUrl;
    delete connectionIconUrl;
    delete consoleIconUrl;
    delete tableIconUrl;
    delete converterIconUrl;
    delete settingsIconUrl;
}


void Decorator::applyToCloseButton(QToolButton *button){
    button->setStyleSheet(getWindowButtonStyleSheet(*closeIconUrl, baseColor(), closeHoverColor(), closePressedColor()));
}
void Decorator::applyToMaximizeButton(QToolButton *button){
    button->setStyleSheet(getWindowButtonStyleSheet(*maximizeIconUrl, baseColor(), maximizeHoverColor(), maximizePressedColor()));
}
void Decorator::applyToMinimizeButton(QToolButton *button){
    button->setStyleSheet(getWindowButtonStyleSheet(*minimizeIconUrl, baseColor(), minimizeHoverColor(), minimizePressedColor()));
}
void Decorator::applyToConnectionButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(*connectionIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToConsoleButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(*consoleIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToTableButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(*tableIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToConverterButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(*converterIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToSettingsButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(*settingsIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToAppLabel(QLabel *label){
    label->setStyleSheet(getAppLabelStyleSheet(*appIconlUrl));
}
void Decorator::applyToConsoleWidget(QPlainTextEdit *display, QLineEdit *input, QPushButton *sendButton, QPushButton *clearButton){
    display->setStyleSheet(getConsoleStyleSheet(secondColor(), textColor(), selectionColor(), selectionTextColor()));
    display->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    input->setStyleSheet(getInputFieldStyleSheet(secondColor(), textColor(), selectionColor(), selectionTextColor()));
    sendButton->setStyleSheet(getUsualButtonStyleSheet(baseColor(), textColor(), thirdColor(), secondColor()));
    clearButton->setStyleSheet(getUsualButtonStyleSheet(baseColor(), textColor(), thirdColor(), secondColor()));
}
void Decorator::applyToTableWidget(QTableView *table, QLineEdit *input, QPushButton *sendButton, QPushButton *clearButton){
    table->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    table->horizontalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    table->setStyleSheet(getTableStyleSheet(secondColor(), textColor(), selectionColor(), selectionTextColor(), baseColor()));
    input->setStyleSheet(getInputFieldStyleSheet(secondColor(), textColor(), selectionColor(), selectionTextColor()));
    sendButton->setStyleSheet(getUsualButtonStyleSheet(baseColor(), textColor(), thirdColor(), secondColor()));
    clearButton->setStyleSheet(getUsualButtonStyleSheet(baseColor(), textColor(), thirdColor(), secondColor()));
}
void Decorator::setBasicColorsToWidget(QWidget *widget, int backgroundColor, int textColor){
    widget->setStyleSheet(getSipmleWidgetStyleSheet(backgroundColor, textColor));
}
void Decorator::setScrollBarColors(QScrollBar *scrollBar, int handleColor, int pageColor){
    scrollBar->setStyleSheet(getScrollBarStyleSheet(handleColor, pageColor));
}
void Decorator::setSettingsButtonsColors(QPushButton *button, int background, int backgroundHover, int backgroundPressed){
    button->setStyleSheet(getSettingsMenuButtonStyleSheet(background, backgroundHover, backgroundPressed));
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
