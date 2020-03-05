#include "decorator.h"
#include <QAbstractItemView>
#include <QListView>


Decorator::Decorator(QString name, IconMode icon, Color base, Color second, Color third)
{
    /* Устанавливаем основные цвета */
    setBaseColor(base);
    setSecondColor(second);
    setThirdColor(third);
    setName(name);
    setIconMode(icon);

    /* Остальные цвета по-умолчанию такие */
    setCloseHoverColor({0xea4445, 0xea4445,0xea4445,0xea4445});
    setClosePressedColor({0xac4042, 0xac4042, 0xac4042, 0xac4042});
    setMaximizeHoverColor(thirdColor());
    setMaximizePressedColor(secondColor());
    setMinimizeHoverColor(thirdColor());
    setMinimizePressedColor(secondColor());
}

Decorator::Decorator(void)
{
    /* Устанавливаем основные цвета */
    setBaseColor({0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF});
    setSecondColor({0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF});
    setThirdColor({0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF});
    setName("unnamed");
    setIconMode(Light);

    /* Остальные цвета по-умолчанию такие */
    setCloseHoverColor({0xea4445, 0xea4445,0xea4445,0xea4445});
    setClosePressedColor({0xac4042, 0xac4042, 0xac4042, 0xac4042});
    setMaximizeHoverColor(thirdColor());
    setMaximizePressedColor(secondColor());
    setMinimizeHoverColor(thirdColor());
    setMinimizePressedColor(secondColor());
}
Decorator::Decorator(const Decorator &decorator){
    setName(decorator.name());
    setIconMode(decorator.iconMode());
    setBaseColor(decorator.baseColor());
    setThirdColor(decorator.thirdColor());
    setSecondColor(decorator.secondColor());
    setCloseHoverColor(decorator.closeHoverColor());
    setClosePressedColor(decorator.closePressedColor());
    setMaximizeHoverColor(decorator.maximizeHoverColor());
    setMaximizePressedColor(decorator.maximizePressedColor());
    setMinimizeHoverColor(decorator.minimizeHoverColor());
    setMinimizePressedColor(decorator.minimizePressedColor());
}

Decorator::~Decorator(){

}

Decorator& Decorator::operator=(const Decorator &source){
    setName(source.name());
    setIconMode(source.iconMode());
    setBaseColor(source.baseColor());
    setThirdColor(source.thirdColor());
    setSecondColor(source.secondColor());
    setCloseHoverColor(source.closeHoverColor());
    setClosePressedColor(source.closePressedColor());
    setMaximizeHoverColor(source.maximizeHoverColor());
    setMaximizePressedColor(source.maximizePressedColor());
    setMinimizeHoverColor(source.minimizeHoverColor());
    setMinimizePressedColor(source.minimizePressedColor());
    return *this;
}

void Decorator::validateColor(Color &color){
    color.background&=0xffffff;
    color.text&=0xffffff;
    color.selectedText&=0xffffff;
    color.selectionColor&=0xffffff;
}

void Decorator::setBaseColor(Color color){
    validateColor(color);
    _baseColor = color;
}
void Decorator::setSecondColor(Color color){
    validateColor(color);
    _secondColor = color;
}
void Decorator::setThirdColor(Color color){
    validateColor(color);
    _thirdColor = color;
}
void Decorator::setCloseHoverColor(Color color){
    validateColor(color);
    _closeHoverColor = color;
}
void Decorator::setClosePressedColor(Color color){
    validateColor(color);
    _closePressedColor = color;
}
void Decorator::setMaximizeHoverColor(Color color){
    validateColor(color);
    _maximizeHoverColor = color;
}
void Decorator::setMaximizePressedColor(Color color){
    validateColor(color);
    _maximizePressedColor = color;
}
void Decorator::setMinimizeHoverColor(Color color){
    validateColor(color);
    _minimizeHoverColor = color;
}
void Decorator::setMinimizePressedColor(Color color){
    validateColor(color);
    _minimizePressedColor = color;
}
void Decorator::setName(QString text){
    _name = text;
}
void Decorator::setIconMode(IconMode mode){
    _iconMode = mode;
    if(mode == Dark){
        appIconlUrl       = ":/dark/resources/icons/dark/label.png";
        closeIconUrl      = ":/dark/resources/icons/dark/close.png";
        maximizeIconUrl   = ":/dark/resources/icons/dark/maximize.png";
        minimizeIconUrl   = ":/dark/resources/icons/dark/minimize.png";
        connectionIconUrl = ":/dark/resources/icons/dark/connect.png";
        consoleIconUrl    = ":/dark/resources/icons/dark/consolemode.png";
        tableIconUrl      = ":/dark/resources/icons/dark/tablemode.png";
        converterIconUrl  = ":/dark/resources/icons/dark/converter.png";
        settingsIconUrl   = ":/dark/resources/icons/dark/settings.png";
        checkBoxCheckUrl  = ":/dark/resources/icons/dark/checkbox.png";
        comboBoxArrowUrl  = ":/dark/resources/icons/dark/combobox.png";
    }
    else{
        appIconlUrl       = ":/light/resources/icons/light/label.png";
        closeIconUrl      = ":/light/resources/icons/light/close.png";
        maximizeIconUrl   = ":/light/resources/icons/light/maximize.png";
        minimizeIconUrl   = ":/light/resources/icons/light/minimize.png";
        connectionIconUrl = ":/light/resources/icons/light/connect.png";
        consoleIconUrl    = ":/light/resources/icons/light/consolemode.png";
        tableIconUrl      = ":/light/resources/icons/light/tablemode.png";
        converterIconUrl  = ":/light/resources/icons/light/converter.png";
        settingsIconUrl   = ":/light/resources/icons/light/settings.png";
        checkBoxCheckUrl  = ":/light/resources/icons/light/checkbox.png";
        comboBoxArrowUrl  = ":/light/resources/icons/light/combobox.png";
    }
}

void Decorator::applyToCloseButton(QToolButton *button){
    button->setStyleSheet(getWindowButtonStyleSheet(closeIconUrl, baseColor(), closeHoverColor(), closePressedColor()));
}
void Decorator::applyToMaximizeButton(QToolButton *button){
    button->setStyleSheet(getWindowButtonStyleSheet(maximizeIconUrl, baseColor(), maximizeHoverColor(), maximizePressedColor()));
}
void Decorator::applyToMinimizeButton(QToolButton *button){
    button->setStyleSheet(getWindowButtonStyleSheet(minimizeIconUrl, baseColor(), minimizeHoverColor(), minimizePressedColor()));
}
void Decorator::applyToConnectionButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(connectionIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToConsoleButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(consoleIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToTableButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(tableIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToConverterButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(converterIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToSettingsButton(QPushButton *button){
    button->setStyleSheet(getQuickPanelButtonStyleSheet(settingsIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToAppLabel(QLabel *label){
    label->setStyleSheet(getAppLabelStyleSheet(appIconlUrl));
}
void Decorator::applyToConsoleWidget(QPlainTextEdit *display, QLineEdit *input, QPushButton *sendButton, QPushButton *clearButton){
    display->setStyleSheet(getConsoleStyleSheet(secondColor()));
    display->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    input->setStyleSheet(getInputFieldStyleSheet(secondColor()));
    sendButton->setStyleSheet(getStandartButtonStyleSheet(baseColor(), thirdColor(), secondColor()));
    clearButton->setStyleSheet(getStandartButtonStyleSheet(baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToTableWidget(QTableView *table, QLineEdit *input, QPushButton *sendButton, QPushButton *clearButton){
    table->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    table->horizontalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    table->setStyleSheet(getTableStyleSheet(secondColor(), baseColor()));
    input->setStyleSheet(getInputFieldStyleSheet(secondColor()));
    sendButton->setStyleSheet(getStandartButtonStyleSheet(baseColor(), thirdColor(), secondColor()));
    clearButton->setStyleSheet(getStandartButtonStyleSheet(baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToConverterWidget(QPlainTextEdit* source, QPlainTextEdit* result, QComboBox* sourceBox,
                                       QComboBox* resultBox,   QPushButton* convert,   QPushButton* swap, QPushButton* clear){
   source->setStyleSheet(getConsoleStyleSheet(baseColor()));
   result->setStyleSheet(getConsoleStyleSheet(baseColor()));
   setScrollBarColors(source->verticalScrollBar(), secondColor(), baseColor());
   setScrollBarColors(result->verticalScrollBar(), secondColor(), baseColor());
   setComboBoxColors(sourceBox);
   setComboBoxColors(resultBox);
   setScrollBarColors(resultBox->view()->verticalScrollBar(), secondColor(), baseColor());
   setScrollBarColors(sourceBox->view()->verticalScrollBar(), secondColor(), baseColor());
   setStandartButtonColors(convert, secondColor(), thirdColor(), baseColor());
   setStandartButtonColors(swap, secondColor(), thirdColor(), baseColor());
   setStandartButtonColors(clear, secondColor(), thirdColor(), baseColor());
}
void Decorator::setBasicColorsToWidget(QWidget *widget, Color colors){
    widget->setStyleSheet(getSipmleWidgetStyleSheet(colors));
}
void Decorator::setScrollBarColors(QScrollBar *scrollBar, Color handleColor, Color pageColor){
    scrollBar->setStyleSheet(getScrollBarStyleSheet(handleColor, pageColor));
}
void Decorator::setSettingsButtonsColors(QPushButton *button, Color background, Color backgroundHover, Color backgroundPressed){
    button->setStyleSheet(getSettingsMenuButtonStyleSheet(background, backgroundHover, backgroundPressed));
}
void Decorator::setComboBoxColors(QComboBox *comboBox){
    comboBox->setStyleSheet(getComboBoxStyleSheet(comboBoxArrowUrl, baseColor(), secondColor(), baseColor()));
    comboBox->view()->setStyleSheet(getComboBoxStyleSheet(comboBoxArrowUrl, baseColor(), secondColor(), baseColor()));
    comboBox->lineEdit();//->setStyleSheet(getComboBoxStyleSheet(baseColor(), textColor(), selectionColor(), selectionTextColor()));
}
void Decorator::setStandartButtonColors(QPushButton *button, Color baseColor, Color hoverColor, Color pressedColor){
    button->setStyleSheet(getStandartButtonStyleSheet(baseColor, hoverColor, pressedColor));
}
void Decorator::setCheckBoxColors(QCheckBox *checkBox, Color color, Color hoverColor, Color pressedColor){
    checkBox->setStyleSheet(getCheckBoxStyleSheet(checkBoxCheckUrl, color, hoverColor, pressedColor));
}


QString Decorator::colorToString(Color color){
    validateColor(color);
    QString styleSheet(
                "background-color:           #%1;"
                "color:                      #%2;"
                "selection-background-color: #%3;"
                "selection-color:            #%4;");
    return styleSheet.arg(QString::number(color.background, 16).rightJustified(6,'0'))
                     .arg(QString::number(color.text, 16).rightJustified(6,'0'))
                     .arg(QString::number(color.selectionColor, 16).rightJustified(6,'0'))
                     .arg(QString::number(color.selectedText, 16).rightJustified(6,'0'));
}

QString Decorator::getMainWidgetStyleSheet(Color color) {
    QString styleSheet (
            "QWidget { "
                "%1"
                "border: none;"
           "}");
    return styleSheet.arg(colorToString(color));
}

QString Decorator::getWindowButtonStyleSheet(QString iconUrl,
                                             Color baseColor,
                                             Color hoverColor,
                                             Color pressedColor){
    QString styleSheet (
           "QToolButton { "
                "image:                      url(%1);"
                "%2;"
                "icon-size:                  12px;"
                "padding-left:               10px;"
                "padding-right:              10px;"
                "padding-top:                5px;"
                "padding-bottom:             5px;"
                "border:                     none;"
           "}"
           "QToolButton:hover {"
                "%3;"
           "}"
           "QToolButton:pressed { "
                "%4"
           "}");
    return styleSheet.arg(iconUrl)
                     .arg(colorToString(baseColor))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor));
}


QString Decorator::getStandartButtonStyleSheet (Color baseColor,
                                                Color hoverColor,
                                                Color pressedColor) {
    QString styleSheet (
           "QPushButton { "
                "%1;"
                "padding-left:     20px;"
                "padding-right:    20px;"
                "padding-top:      6px;"
                "padding-bottom:   6px;"
                "border:           none; "
           "}"
           "QPushButton:hover { "
                "%2;"
           "}"
           "QPushButton:pressed { "
                "%3; "
           "}");
    return styleSheet.arg(colorToString(baseColor))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor));
}

QString Decorator::getInputFieldStyleSheet (Color color) {
    QString styleSheet (
                "QLineEdit { "
                    "%1;"
                    "border:                     none;"
                    "padding-left:               10px;"
                    "padding-right:              5px;"
                    "padding-top:                5px;"
                    "padding-bottom:             5px;"
                "}"
                );
    return styleSheet.arg(colorToString(color));
}

QString Decorator::getScrollBarStyleSheet (Color handleColor, Color pageColor) {
    QString styleSheet (
           "QScrollBar:vertical { "
                "border: none;"
                "height: 10px;"
                "width:  10px;"
                "margin: 0;"
           "}"
           "QScrollBar::handle:vertical {"
                "%1;"
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
                "%2;"
                "border:     none;"
           "}"
           "QScrollBar::sub-page:vertical {"
                "%2;"
                "border:     none;"
           "}"
           "QScrollBar:horizontal { "
                "border: none;"
                "height: 10px;"
                "width:  10px;"
                "margin: 0;"
           "}"
           "QScrollBar::handle:horizontal {"
                "%1;"
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
                "%2;"
                "border:     none;"
           "}"
           "QScrollBar::sub-page:horizontal {"
                "%2;"
                "border:     none;"
           "}"
                );
    return styleSheet.arg(colorToString(handleColor))
                     .arg(colorToString(pageColor));
}


QString Decorator::getConsoleStyleSheet(Color color) {
    QString styleSheet (
           "QPlainTextEdit { "
                "%1;"
                "border-style:               solid;"       // Без этого свойства бэкграунд не устанавливается(??)
                "border:                     none;"
                "border-top-left-radius:     5px;"
           "}");
    return styleSheet.arg(colorToString(color));
}



QString Decorator::getQuickPanelButtonStyleSheet(QString iconUrl,
                                                 Color baseColor,
                                                 Color hoverColor,
                                                 Color pressedColor)
{
    QString styleSheet (
                "QPushButton { "
                "image:              url(%1);"
                "icon-size:          12px;"
                "qproperty-iconSize: 12px;"
                "%2;"
                "padding-left:       10px;"
                "padding-right:      10px;"
                "padding-top:        10px;"
                "padding-bottom:     10px;"
                "border:             none;"
           "}"
           "QPushButton:hover { "
                "%3;"
           "}"
           "QPushButton:pressed { "
                "%4;"
           "}");
    return styleSheet.arg(iconUrl)
                     .arg(colorToString(baseColor))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor));
}



QString Decorator::getAppLabelStyleSheet(QString iconUrl){
    QString styleSheet (
            "QLabel {"
                "image: url(%1);"
            "}");
    return styleSheet.arg(iconUrl);
}

QString Decorator::getTableStyleSheet(Color tableColor,
                                      Color headerColor) {
    QString styleSheet (
            "QTableView {"
                "%1"
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
                "%2"
                "padding-left:               1px;"
                "border:                     none;"
                "border-top-left-radius:     5px;"
           "}"
            );
    return styleSheet.arg(colorToString(tableColor))
                     .arg(colorToString(headerColor));
}







QString Decorator::getSipmleWidgetStyleSheet(Color color) {
   QString styleSheet(
            "QWidget { "
                "border:           none;"
                "border-style:     solid;"
                "%1;"
            "}");
   return styleSheet.arg(colorToString(color));
}

QString Decorator::getSettingsMenuButtonStyleSheet(Color baseColor,
                                                   Color hoverColor,
                                                   Color pressedColor)
{
    QString styleSheet(
                "QPushButton { "
                       "%1;"
                       "padding-left:           40px;"
                       "padding-right:          40px;"
                       "padding-top:            10px;"
                       "padding-bottom:         10px;"
                       "border:                 none;"
                       "border-radius:          5px;"
                       "margin:                 2px;"
                  "}"
                  "QPushButton:hover { "
                       "%2;"
                  "}"
                  "QPushButton:pressed { "
                       "%3; "
                  "}");
    return styleSheet.arg(colorToString(baseColor))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor));
}

QString Decorator::getComboBoxStyleSheet(QString &arrowUrl,
                                         Color activeColor,
                                         Color disableColor,
                                         Color listColor)
{
    QString styleSheet(

//                "QComboBox{"
//                "border:                 none;"
//                "background-color:   rgb(87, 96, 134);"
//                "color:                      rgb(255, 255, 255);"
//                "font-weight:            bold;"
//                "padding:                    5px "

//                "}"

//                "QComboBox::drop-down{"
//                   " border:                 none;"
//                    "background-color:   rgb(87, 96, 134);"
//                    "color:                      rgb(255, 255, 255);"
//                   " font-weight:            bold;"
//                    "padding:                    0px;"
//                "}"

//                "QComboBox::down-arrow{"
//                 "   image:                      url(:/icons/resources/minimize.png);"
//                  "  padding-right:          5px;"
//                "}"

//               " QListView{"
//                "    border:                 none;"
//                 "   color:                      rgb(87, 96, 134);"
//                  "  background-color:   rgb(255, 255, 255);"
//                   " font-weight:            bold;"
//                    "selection-background-color: rgb(47, 175, 178);"
//                   " show-decoration-selected: 1;"
//                   " margin-left:                -10px;"
//                   " padding-left    :           15px;"
//               " }"

//               " QListView::item:hover{"

//                "    background-color:   rgb(47, 175, 178);"
//                 "   border:                 none;"
//                "}"

//                );

            "QComboBox { "
                "border:                     none; "
                "border-style:               solid;"
                "border-radius:              3px;"
                "%1"
                "padding-top:                5px;"
                "padding-bottom:             5px;"
                "padding-right:              10px;"
                "padding-left:               10px;"
            "}"
            // Если бокс деактивирован
            "QComboBox:disabled { "
                "%2"
            "}"
            // Стиль виджета при редактировании элемента
            "QComboBox:editable { "
                "background:                 red;"//#2b2d33;"
            "}"
            // Выпадающий список целиком
            "QComboBox QAbstractItemView { "
                "%3"
                "border:                     none; "   // Внешняя граница в выпадающем списке
                "outline:                    none;"    // Граница элемента списка
            "}"
            // Применяется для неизменяемого поля, и ненажатой кнопки
            "QComboBox:!editable, QComboBox::drop-down:editable { "
            "}"
            // Применяется для неизменяемого поля, и нажатой кнопки
            "QComboBox:!editable:on, QComboBox::drop-down:editable:on { "
            "}"
            // При открытом списке верхнее поле
            "QComboBox:on { "
               "padding-top: 3px; "   // При раскрытии списка верхнее поле смещаем
               "padding-left: 4px;"
            "}"
            // Кнопка раскрытия списка
            "QComboBox::drop-down { "
                "width:                      15px;"
                "border:                     none;"
                "border-top-right-radius:    8px;"
                "border-bottom-right-radius: 3px;"
                "padding-right:              4px;"
            "}"
            // Стрелка при закрытом списке
            "QComboBox::down-arrow { "
                "image: url(%4);"
                "width: 14px;"
                "height: 14px;"
            "}"
            // Стрелка, когда список раскрыт
            "QComboBox::down-arrow:on { "
                "top: 1px;"
                "left: 1px;"
            "}"



//            // Работает только если установить для Combobox специальный делегат
//            //   combobox->setItemDelegate(new QStyledItemDelegate(combobox));
//            //   https://switch-case.ru/33415621
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
           );
    return styleSheet.arg(colorToString(activeColor))
                     .arg(colorToString(disableColor))
                     .arg(colorToString(listColor))
                     .arg(arrowUrl);
}
QString Decorator::getCheckBoxStyleSheet(QString &checkUrl, Color color, Color hoverColor, Color pressedColor){
    QString styleSheet(
                "QCheckBox {"
                "}"
                "QCheckBox::indicator {"
                    "width:           15px;"
                    "height:          15px;"
                    "border-radius:   4px;"
                    "%1"
                "}"
                "QCheckBox::indicator:unchecked {"
                "}"
                "QCheckBox::indicator:unchecked:hover {"
                    "%2"
                "}"
                "QCheckBox::indicator:unchecked:pressed {"
                    "%3"
                "}"
                "QCheckBox::indicator:checked {"
                    "image: url(%4);"
                "}"
                "QCheckBox::indicator:checked:hover {"
                "}"
                "QCheckBox::indicator:checked:pressed {"
                "}"
                "QCheckBox::indicator:indeterminate:hover {"
                "}"
                "QCheckBox::indicator:indeterminate:pressed {"
                "}"
                );
    return styleSheet.arg(colorToString(color))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor))
                     .arg(checkUrl);
}
