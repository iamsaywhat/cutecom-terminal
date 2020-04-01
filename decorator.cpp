#include "decorator.h"
#include <QAbstractItemView>
#include <QListView>

Ui::MainWidget* Decorator::gui = nullptr;

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
    setName("untitled");
    setIconMode(Light);

    /* Остальные цвета по-умолчанию такие */
    setCloseHoverColor({0xea4445, 0xea4445,0xea4445,0xea4445});
    setClosePressedColor({0xac4042, 0xac4042, 0xac4042, 0xac4042});
    setMaximizeHoverColor(thirdColor());
    setMaximizePressedColor(secondColor());
    setMinimizeHoverColor(thirdColor());
    setMinimizePressedColor(secondColor());
}
Decorator::Decorator(const Decorator &decorator):QObject(){
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
void Decorator::apply(void){
    applyToMainWidget();   // Основная рамка
    applyToConsole();      // Виджет консоли
    applyToTable();        // Виджет таблицы
    applyToConverter();    // Конвертер
    applyToSettings();     // Виджет настроек
}
void Decorator::applyToMainWidget(void){
    setBasicColorsToWidget  (gui->centralWidget, baseColor());
    gui->appName->setStyleSheet(getAppLabelStyleSheet(appIconlUrl));
    gui->closeButton->setStyleSheet(getWindowButtonStyleSheet(closeIconUrl, baseColor(), closeHoverColor(), closePressedColor()));
    gui->maximazeButton->setStyleSheet(getWindowButtonStyleSheet(maximizeIconUrl, baseColor(), maximizeHoverColor(), maximizePressedColor()));
    gui->minimizeButton->setStyleSheet(getWindowButtonStyleSheet(minimizeIconUrl, baseColor(), minimizeHoverColor(), minimizePressedColor()));
    gui->showConnectionButton->setStyleSheet(getQuickPanelButtonStyleSheet(connectionIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->switchToConsoleButton->setStyleSheet(getQuickPanelButtonStyleSheet(consoleIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->switchToTableButton->setStyleSheet(getQuickPanelButtonStyleSheet(tableIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->switchToConverterButton->setStyleSheet(getQuickPanelButtonStyleSheet(converterIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->showSettingsButton->setStyleSheet(getQuickPanelButtonStyleSheet(settingsIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToConsole(void){
    gui->consoleField->setStyleSheet(getConsoleStyleSheet(secondColor()));
    gui->consoleField->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->inputConsoleField->setStyleSheet(getInputFieldStyleSheet(secondColor()));
    gui->sendConsoleButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 20, 6, 0, 0, 0, 0));
    gui->clearConsoleButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 20, 6, 0, 0, 0, 0));
}
void Decorator::applyToTable(void){
    gui->tableField->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->tableField->horizontalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->tableField->setStyleSheet(getTableStyleSheet(secondColor(), baseColor()));
    gui->inputTableField->setStyleSheet(getInputFieldStyleSheet(secondColor()));
    gui->sendTableButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 20, 6, 0, 0, 0, 0));
    gui->clearTableButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 20, 6, 0, 0, 0, 0));
}
void Decorator::applyToConverter(void){
    setBasicColorsToWidget(gui->converterPage, baseColor());
    gui->converterSource->setStyleSheet(getPlainTextStyleSheet(secondColor()));
    gui->converterResult->setStyleSheet(getPlainTextStyleSheet(secondColor()));
    gui->converterSource->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->converterResult->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    setComboBoxColors(gui->converterSourceBox, secondColor(), baseColor(), secondColor());
    setComboBoxColors(gui->converterResultBox, secondColor(), baseColor(), secondColor());
    gui->converterResultBox->view()->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(baseColor(), secondColor()));
    gui->converterSourceBox->view()->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(baseColor(), secondColor()));
    gui->converterConvertButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 10, 5, 5, 5, 5));
    gui->converterSwapButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 10, 5, 5, 5, 5));
    gui->converterClearButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 10, 5, 5, 5, 5));
}
void Decorator::applyToSettings(void){
    setBasicColorsToWidget  (gui->leftPanelContents, baseColor());
    setBasicColorsToWidget  (gui->settingsPage, baseColor());
    gui->connectionContentsButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 20, 5, 5, 0, 0));
    gui->generalContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 20, 5, 5, 0, 0));
    gui->consoleContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 20, 5, 5, 0, 0));
    gui->tableContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 20, 5, 5, 0, 0));
    gui->logsContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 20, 5, 5, 0, 0));
    gui->bindsContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 40, 20, 5, 5, 0, 0));
    gui->settingsLeftPanel->horizontalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->settingsLeftPanel->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->settingsRightPanel->horizontalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->settingsRightPanel->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    setBasicColorsToWidget(gui->rightPanelContents, secondColor());
    setBasicColorsToWidget(gui->rightStackedPanel, secondColor());
    setBasicColorsToWidget(gui->labelSectionConnection, secondColor());
    setBasicColorsToWidget(gui->labelPorts, secondColor());
    setBasicColorsToWidget(gui->labelBaudrate, secondColor());
    setBasicColorsToWidget(gui->labelData, secondColor());
    setBasicColorsToWidget(gui->labelParity, secondColor());
    setBasicColorsToWidget(gui->labelStopBits, secondColor());
    setBasicColorsToWidget(gui->labelFlowControl, secondColor());
    // Вкладка Connection
    setComboBoxColors(gui->boxPorts, baseColor(), secondColor(), baseColor());
    setComboBoxColors(gui->boxData, baseColor(), secondColor(), baseColor());
    setComboBoxColors(gui->boxParity, baseColor(), secondColor(), baseColor());
    setComboBoxColors(gui->boxBaudrate, baseColor(), secondColor(), baseColor());
    setComboBoxColors(gui->boxStopBits, baseColor(), secondColor(), baseColor());
    setComboBoxColors(gui->boxFlowControl, baseColor(), secondColor(), baseColor());
    gui->buttonConnectDisconnect->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 50, 15, 5, 5, 5, 5));
    // Вкладка General
    setComboBoxColors(gui->comboBoxTheme, baseColor(), secondColor(), baseColor());
    setComboBoxColors(gui->comboBoxLanguage, baseColor(), secondColor(), baseColor());
    setComboBoxColors(gui->comboBoxCodec, baseColor(), secondColor(), baseColor());
    setBasicColorsToWidget(gui->labelTheme, secondColor());
    setBasicColorsToWidget(gui->labelCodec, secondColor());
    setBasicColorsToWidget(gui->labelLanguage, secondColor());
    setBasicColorsToWidget(gui->labelSectionGeneral, secondColor());
    setBasicColorsToWidget(gui->labelCaptureTime, secondColor());
    setBasicColorsToWidget(gui->labelCaptureBytes, secondColor());
    gui->spinBoxCaptureTime->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrow, spinboxUpArrow, baseColor()));
    gui->spinBoxCaptureBytes->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrow, spinboxUpArrow, baseColor()));
    gui->buttonGeneralApply->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 50, 15, 5, 5, 5, 5));
    // Вкладка Table
    setBasicColorsToWidget(gui->labelSectionTable, secondColor());
    setBasicColorsToWidget(gui->labelTableEcho, secondColor());
    setBasicColorsToWidget(gui->labelTableCyclicMode, secondColor());
    setBasicColorsToWidget(gui->labelTableCyclicInterval, secondColor());
    setBasicColorsToWidget(gui->labelTableHotKey1, secondColor());
    setBasicColorsToWidget(gui->labelTableHotKey2, secondColor());
    setBasicColorsToWidget(gui->labelTableHotKey3, secondColor());
    setBasicColorsToWidget(gui->labelTableHotKey4, secondColor());
    setCheckBoxColors(gui->checkboxTableEcho, baseColor(), thirdColor(), baseColor());
    setCheckBoxColors(gui->checkboxTableCyclic, baseColor(), thirdColor(), baseColor());
    gui->spinboxTableCyclicInterval->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrow, spinboxUpArrow, baseColor()));
    gui->lineEditTableHotKey1->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    gui->lineEditTableHotKey2->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    gui->lineEditTableHotKey3->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    gui->lineEditTableHotKey4->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    // Вкладка Console
    setBasicColorsToWidget(gui->labelSectionConsole, secondColor());
    setBasicColorsToWidget(gui->labelConsoleEcho, secondColor());
    setBasicColorsToWidget(gui->labelConsoleCyclic, secondColor());
    setBasicColorsToWidget(gui->labelConsoleCyclicInterval, secondColor());
    setBasicColorsToWidget(gui->labelConsoleHotKey1, secondColor());
    setBasicColorsToWidget(gui->labelConsoleHotKey2, secondColor());
    setBasicColorsToWidget(gui->labelConsoleHotKey3, secondColor());
    setBasicColorsToWidget(gui->labelConsoleHotKey4, secondColor());
    setCheckBoxColors(gui->checkboxConsoleEcho, baseColor(), thirdColor(), baseColor());
    setCheckBoxColors(gui->checkboxConsoleCyclic, baseColor(), thirdColor(), baseColor());
    gui->spinboxConsoleCyclicInterval->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrow, spinboxUpArrow, baseColor()));
    gui->lineEditConsoleHotKey1->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    gui->lineEditConsoleHotKey2->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    gui->lineEditConsoleHotKey3->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    gui->lineEditConsoleHotKey4->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    // Вкладка Logs
    setBasicColorsToWidget(gui->labelSectionLog, secondColor());
    setBasicColorsToWidget(gui->labelLogEnable, secondColor());
    setBasicColorsToWidget(gui->labelLogPath, secondColor());
    setBasicColorsToWidget(gui->labelLogSpace, secondColor());
    setBasicColorsToWidget(gui->labelLogColomnSize, secondColor());
    setCheckBoxColors(gui->checkBoxLogEnable, baseColor(), thirdColor(), baseColor());
    gui->lineEditLogPath->setStyleSheet(getInputFieldStyleSheet(baseColor()));
    gui->buttonLogApply->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 50, 15, 5, 5, 5, 5));
    gui->spinBoxLogSpace->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrow, spinboxUpArrow, baseColor()));
    gui->spinBoxLogColumnSize->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrow, spinboxUpArrow, baseColor()));
}
void Decorator::validateColor(Color &color){
    color.background&=0xffffff;
    color.text&=0xffffff;
    color.selectedText&=0xffffff;
    color.selectionColor&=0xffffff;
}
void Decorator::setTargetGui(Ui::MainWidget* target){
    gui = target;
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
        spinboxDownArrow  = ":/dark/resources/icons/dark/down-arrow.png";
        spinboxUpArrow    = ":/dark/resources/icons/dark/up-arrow.png";
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
        spinboxDownArrow  = ":/light/resources/icons/light/down-arrow.png";
        spinboxUpArrow    = ":/light/resources/icons/light/up-arrow.png";
    }
}
void Decorator::setBasicColorsToWidget(QWidget *widget, Color colors){
    widget->setStyleSheet(getSipmleWidgetStyleSheet(colors));
}
void Decorator::setComboBoxColors(QComboBox *comboBox, Color activeColor, Color disableColor, Color listColor){
    comboBox->setStyleSheet(getComboBoxStyleSheet(comboBoxArrowUrl, activeColor, disableColor, listColor));
    comboBox->view()->setStyleSheet(getComboBoxStyleSheet(comboBoxArrowUrl, activeColor, disableColor, listColor));
}
void Decorator::setCheckBoxColors(QCheckBox *checkBox, Color color, Color hoverColor, Color pressedColor){
    checkBox->setStyleSheet(getCheckBoxStyleSheet(checkBoxCheckUrl, color, hoverColor, pressedColor));
}
/* Специальные таблицы стилей для главного окна ******************************************************************/
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
QString Decorator::getAppLabelStyleSheet(QString iconUrl){
    QString styleSheet (
            "QLabel {"
                "image: url(%1);"
            "}");
    return styleSheet.arg(iconUrl);
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
                "border-radius: 5px;"
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


/* Специальные таблицы стилей  для базовых элементов gui *****************************************/
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
/* Таблицы стилей базовых элементов gui **************************************************************************/
QString Decorator::getScrollBarStyleSheet (Color handleColor, Color pageColor) {
    QString styleSheet (
           "QScrollBar {"
                 "border: none;"
                 "%2"
                 "height: 15px;"
                 "width: 15px;"
                 "margin: 0px;"  // Если ясно не указать ломает вертикальные скроллбары
           "}"
           "QScrollBar::handle {"
                 "%1"
                 "min-width: 20px;"
                 "min-height: 20px;"
                 "margin: 4px;"
           "}"
           "QScrollBar::add-line,  QScrollBar::sub-line {"
                 "background: none;"
                 "border: none;"
           "}"
           "QScrollBar::add-page, QScrollBar::sub-page {"
                 "%2"
           "}");
    return styleSheet.arg(colorToString(handleColor))
                     .arg(colorToString(pageColor));
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
QString Decorator::getComboBoxStyleSheet(QString &arrowUrl,
                                         Color activeColor,
                                         Color disableColor,
                                         Color listColor)
{
    QString styleSheet(
            "QComboBox { "
                "border:                     none; "
                "border-style:               solid;"
                "border-radius:              5px;"
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
QString Decorator::getPlainTextStyleSheet(Color color){
    QString styleSheet (
           "QPlainTextEdit { "
                "%1;"
                "border-style:      solid;"       // Без этого свойства бэкграунд не устанавливается(??)
                "border:            none;"
                "border-radius:     5px;"
           "}");
    return styleSheet.arg(colorToString(color));
}
QString Decorator::getButtonStyleSheet(Color baseColor, Color hoverColor, Color pressedColor,
                                        int horizontalPadding, int verticalPadding,
                                        int leftUpperRadius,   int leftBottomRadius,
                                        int rightUpperRadius,  int rightBottomRadius){
    QString styleSheet(
                "QPushButton { "
                       "%1;"
                       "padding-left:                  %4px;"
                       "padding-right:                 %4px;"
                       "padding-top:                   %5px;"
                       "padding-bottom:                %5px;"
                       "border:                        none;"
                       "border-bottom-left-radius:     %6px;"
                       "border-bottom-right-radius:    %7px;"
                       "border-top-right-radius:       %8px;"
                       "border-top-left-radius:        %9px;"
//                       "margin:                        2px;"
                  "}"
                  "QPushButton:hover { "
                       "%2;"
                  "}"
                  "QPushButton:pressed { "
                       "%3; "
                  "}");
    return styleSheet.arg(colorToString(baseColor))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor))
                     .arg(QString::number(horizontalPadding))
                     .arg(QString::number(verticalPadding))
                     .arg(QString::number(leftBottomRadius))
                     .arg(QString::number(rightBottomRadius))
                     .arg(QString::number(rightUpperRadius))
                     .arg(QString::number(leftUpperRadius));
}
QString Decorator::getSpinBoxStyleSheet(QString& downArrowUrl,
                                        QString& upArrowUrl,
                                        Color color){
    QString styleSheet(
                "QSpinBox {"
                    "%1"
                    "padding-top:      5px;"
                    "padding-bottom:   5px;"
                    "padding-right:    30px;"
                    "padding-left:     10px;"
                    "border-radius:    5px;"
                "}"
                "QSpinBox::up-button{"
                    "subcontrol-origin: margin;"
                    "subcontrol-position: center right;"
                    "right: 30px;"
                    "width: 30px;"
                    "image: url(%2) 1;"
                "}"
                "QSpinBox::down-button {"
                    "subcontrol-origin: margin;"
                    "subcontrol-position: center right;"
                    "width: 30px;"
                    "image: url(%3) 1;"
                "}"
                );
    return styleSheet.arg(colorToString(color))
                     .arg(upArrowUrl)
                     .arg(downArrowUrl);
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
