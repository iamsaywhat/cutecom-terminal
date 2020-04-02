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
    setBasicColorsToWidget(gui->centralWidget, baseColor());
    setBasicColorsToWidget(gui->workspaceWidget, baseColor());
    gui->appName->setStyleSheet(getAppLabelStyleSheet(appIconlUrl));
    gui->closeButton->setStyleSheet(getWindowButtonStyleSheet(closeIconUrl, closeIconUrl, baseColor(), closeHoverColor(), closePressedColor()));
    gui->maximazeButton->setStyleSheet(getWindowButtonStyleSheet(maximizeIconUrl, normalizeIconUrl, baseColor(), maximizeHoverColor(), maximizePressedColor()));
    gui->minimizeButton->setStyleSheet(getWindowButtonStyleSheet(minimizeIconUrl, minimizeIconUrl, baseColor(), minimizeHoverColor(), minimizePressedColor()));
    gui->showConnectionButton->setStyleSheet(getQuickPanelButtonStyleSheet(connectionIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->switchToConsoleButton->setStyleSheet(getQuickPanelButtonStyleSheet(consoleIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->switchToTableButton->setStyleSheet(getQuickPanelButtonStyleSheet(tableIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->switchToConverterButton->setStyleSheet(getQuickPanelButtonStyleSheet(converterIconUrl, baseColor(), thirdColor(), secondColor()));
    gui->showSettingsButton->setStyleSheet(getQuickPanelButtonStyleSheet(settingsIconUrl, baseColor(), thirdColor(), secondColor()));
}
void Decorator::applyToConsole(void){
    gui->consoleField->setStyleSheet(getPlainTextStyleSheet(secondColor(), 0, 0, 10, 10));
    gui->consoleField->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->inputConsoleField->setStyleSheet(getLineEditStyleSheet(secondColor(), 5, 0, 5, 10));
    gui->sendConsoleButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 5, 20));
    gui->clearConsoleButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 5, 20));
}
void Decorator::applyToTable(void){
    gui->tableField->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->tableField->horizontalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->tableField->setStyleSheet(getTableStyleSheet(secondColor(), baseColor()));
    gui->inputTableField->setStyleSheet(getLineEditStyleSheet(secondColor(), 5, 0, 5, 10));
    gui->sendTableButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 5, 20));
    gui->clearTableButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 5, 20));
}
void Decorator::applyToConverter(void){
    setBasicColorsToWidget(gui->converterPage, baseColor());
    gui->converterSource->setStyleSheet(getPlainTextStyleSheet(secondColor(), 0, 0, 10, 10));
    gui->converterResult->setStyleSheet(getPlainTextStyleSheet(secondColor(), 0, 0, 10, 10));
    gui->converterSource->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    gui->converterResult->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(secondColor(), baseColor()));
    setComboBoxColors(gui->converterSourceBox, secondColor(), baseColor(), secondColor());
    setComboBoxColors(gui->converterResultBox, secondColor(), baseColor(), secondColor());
    gui->converterResultBox->view()->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(baseColor(), secondColor()));
    gui->converterSourceBox->view()->verticalScrollBar()->setStyleSheet(getScrollBarStyleSheet(baseColor(), secondColor()));
    gui->converterConvertButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 40));
    gui->converterSwapButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 40));
    gui->converterClearButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 40));
}
void Decorator::applyToSettings(void){
    setBasicColorsToWidget  (gui->leftPanelContents, baseColor());
    setBasicColorsToWidget  (gui->settingsPage, baseColor());
    gui->connectionContentsButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 50));
    gui->generalContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 50));
    gui->consoleContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 50));
    gui->tableContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 50));
    gui->logsContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 50));
    gui->bindsContentButton->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 5, 5, 20, 50));
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
    gui->buttonConnectDisconnect->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 2, 2, 20, 40));
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
    gui->spinBoxCaptureTime->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrowUrl, spinboxUpArrowUrl, baseColor()));
    gui->spinBoxCaptureBytes->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrowUrl, spinboxUpArrowUrl, baseColor()));
    gui->buttonGeneralApply->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 2, 2, 20, 40));
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
    gui->spinboxTableCyclicInterval->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrowUrl, spinboxUpArrowUrl, baseColor()));
    gui->lineEditTableHotKey1->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->lineEditTableHotKey2->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->lineEditTableHotKey3->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->lineEditTableHotKey4->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->buttonTableHotKey1->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
    gui->buttonTableHotKey2->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
    gui->buttonTableHotKey3->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
    gui->buttonTableHotKey4->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
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
    gui->spinboxConsoleCyclicInterval->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrowUrl, spinboxUpArrowUrl, baseColor()));
    gui->lineEditConsoleHotKey1->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->lineEditConsoleHotKey2->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->lineEditConsoleHotKey3->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->lineEditConsoleHotKey4->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->buttonConsoleHotKey1->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
    gui->buttonConsoleHotKey2->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
    gui->buttonConsoleHotKey3->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
    gui->buttonConsoleHotKey4->setStyleSheet(getRunButton(playIconUrl, stopIconUrl, baseColor()));
    // Вкладка Logs
    setBasicColorsToWidget(gui->labelSectionLog, secondColor());
    setBasicColorsToWidget(gui->labelLogEnable, secondColor());
    setBasicColorsToWidget(gui->labelLogPath, secondColor());
    setBasicColorsToWidget(gui->labelLogSpace, secondColor());
    setBasicColorsToWidget(gui->labelLogColomnSize, secondColor());
    setCheckBoxColors(gui->checkBoxLogEnable, baseColor(), thirdColor(), baseColor());
    gui->lineEditLogPath->setStyleSheet(getLineEditStyleSheet(baseColor(), 2, 0, 5, 10));
    gui->buttonLogApply->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 2, 2, 20, 40));
    gui->spinBoxLogSpace->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrowUrl, spinboxUpArrowUrl, baseColor()));
    gui->spinBoxLogColumnSize->setStyleSheet(getSpinBoxStyleSheet(spinboxDownArrowUrl, spinboxUpArrowUrl, baseColor()));
    gui->buttonLogPath->setStyleSheet(getButtonStyleSheet(baseColor(), thirdColor(), secondColor(), 2, 0, 5, 5));
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
        appIconlUrl         = ":/dark/resources/icons/dark/label.png";
        closeIconUrl        = ":/dark/resources/icons/dark/close.png";
        maximizeIconUrl     = ":/dark/resources/icons/dark/maximize.png";
        minimizeIconUrl     = ":/dark/resources/icons/dark/minimize.png";
        normalizeIconUrl    = ":/dark/resources/icons/dark/normalize.png";
        connectionIconUrl   = ":/dark/resources/icons/dark/connect.png";
        consoleIconUrl      = ":/dark/resources/icons/dark/consolemode.png";
        tableIconUrl        = ":/dark/resources/icons/dark/tablemode.png";
        converterIconUrl    = ":/dark/resources/icons/dark/converter.png";
        settingsIconUrl     = ":/dark/resources/icons/dark/settings.png";
        checkBoxCheckUrl    = ":/dark/resources/icons/dark/checkbox.png";
        comboBoxArrowUrl    = ":/dark/resources/icons/dark/combobox.png";
        spinboxDownArrowUrl = ":/dark/resources/icons/dark/down-arrow.png";
        spinboxUpArrowUrl   = ":/dark/resources/icons/dark/up-arrow.png";
        playIconUrl         = ":/dark/resources/icons/dark/play.png";
        stopIconUrl         = ":/dark/resources/icons/dark/stop.png";
    }
    else{
        appIconlUrl         = ":/light/resources/icons/light/label.png";
        closeIconUrl        = ":/light/resources/icons/light/close.png";
        maximizeIconUrl     = ":/light/resources/icons/light/maximize.png";
        minimizeIconUrl     = ":/light/resources/icons/light/minimize.png";
        normalizeIconUrl    = ":/light/resources/icons/light/normalize.png";
        connectionIconUrl   = ":/light/resources/icons/light/connect.png";
        consoleIconUrl      = ":/light/resources/icons/light/consolemode.png";
        tableIconUrl        = ":/light/resources/icons/light/tablemode.png";
        converterIconUrl    = ":/light/resources/icons/light/converter.png";
        settingsIconUrl     = ":/light/resources/icons/light/settings.png";
        checkBoxCheckUrl    = ":/light/resources/icons/light/checkbox.png";
        comboBoxArrowUrl    = ":/light/resources/icons/light/combobox.png";
        spinboxDownArrowUrl = ":/light/resources/icons/light/down-arrow.png";
        spinboxUpArrowUrl   = ":/light/resources/icons/light/up-arrow.png";
        playIconUrl         = ":/light/resources/icons/light/play.png";
        stopIconUrl         = ":/light/resources/icons/light/stop.png";
    }
}
void Decorator::setBasicColorsToWidget(QWidget *widget, Color colors){
    widget->setStyleSheet(getWidgetStyleSheet(colors));
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
                                             QString iconUrlChecked,
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
           "}"
           "QToolButton:checked { "
                "image:             url(%5);"
           "}");
    return styleSheet.arg(iconUrl)
                     .arg(colorToString(baseColor))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor))
                     .arg(iconUrlChecked);
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
                "border-radius:     5px;"
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
QString Decorator::getWidgetStyleSheet(Color color) {
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
QString Decorator::getPlainTextStyleSheet(Color color, int verticalMargin, int horizontalMargin,
                                           int verticalPadding, int horizontalPadding) {
     QString styleSheet (
                 "QPlainTextEdit { "
                     "%1;"
                     "border:                     none;"
                     "margin-top:                 %2px;"
                     "margin-bottom:              %2px;"
                     "margin-left:                %3px;"
                     "margin-right:               %3px;"
                     "padding-top:                %4px;"
                     "padding-bottom:             %4px;"
                     "padding-left:               %5px;"
                     "padding-right:              %5px;"
                     "border-radius:              5px;"

                 "}"
                 );
     return styleSheet.arg(colorToString(color))
                      .arg(verticalMargin)
                      .arg(horizontalMargin)
                      .arg(verticalPadding)
                      .arg(horizontalPadding);
}
QString Decorator::getButtonStyleSheet(Color baseColor, Color hoverColor, Color pressedColor,
                                       int verticalMargin, int horizontalMargin,
                                       int verticalPadding, int horizontalPadding) {
    QString styleSheet(
                "QPushButton, QToolButton {"
                       "%1;"
                       "border:                     none;"
                       "margin-top:                 %4px;"
                       "margin-bottom:              %4px;"
                       "margin-left:                %5px;"
                       "margin-right:               %5px;"
                       "padding-top:                %6px;"
                       "padding-bottom:             %6px;"
                       "padding-left:               %7px;"
                       "padding-right:              %7px;"
                       "border-radius:              5px;"
                  "}"
                  "QPushButton:hover, QToolButton:hover { "
                       "%2;"
                  "}"
                  "QPushButton:pressed, QToolButton:pressed { "
                       "%3; "
                  "}");
    return styleSheet.arg(colorToString(baseColor))
                     .arg(colorToString(hoverColor))
                     .arg(colorToString(pressedColor))
                     .arg(verticalMargin)
                     .arg(horizontalMargin)
                     .arg(verticalPadding)
                     .arg(horizontalPadding);
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
                    "image: url(%2);"
                "}"
                "QSpinBox::down-button {"
                    "subcontrol-origin: margin;"
                    "subcontrol-position: center right;"
                    "width: 30px;"
                    "image: url(%3);"
                "}"
                );
    return styleSheet.arg(colorToString(color))
                     .arg(upArrowUrl)
                     .arg(downArrowUrl);
}
QString Decorator::getRunButton(QString& playUrl, QString& stopUrl, Color color){

    QString styleSheet(
                "QToolButton {"
                    "padding:           5px;"
                    "border:            none;"
                    "border-radius:     5px;"
                    "image:             url(%1);"
                    "%3"
                "}"
                "QToolButton:checked {"
                    "image:             url(%2);"
                "}"
                );
    return styleSheet.arg(playUrl)
                     .arg(stopUrl)
                     .arg(colorToString(color));
}
QString Decorator::getLineEditStyleSheet (Color color, int verticalMargin, int horizontalMargin,
                                          int verticalPadding, int horizontalPadding) {
    QString styleSheet (
                "QLineEdit { "
                    "%1;"
                    "border:          none;"
                    "margin-top:      %2px;"
                    "margin-bottom:   %2px;"
                    "margin-left:     %3px;"
                    "margin-right:    %3px;"
                    "padding-top:     %4px;"
                    "padding-bottom:  %4px;"
                    "padding-left:    %5px;"
                    "padding-right:   %5px;"
                    "border-radius:   5px;"

                "}"
                );
    return styleSheet.arg(colorToString(color))
                     .arg(verticalMargin)
                     .arg(horizontalMargin)
                     .arg(verticalPadding)
                     .arg(horizontalPadding);
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
QString Decorator::urlToStyleSheet(QString& url){
    QString styleSheet(
                "image: url(%1);");
    return styleSheet.arg(url);
}
