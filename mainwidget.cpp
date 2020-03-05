#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QScrollBar>
#include <QString>
#include <QThread>
#include <QTextCodec>


MainWidget::MainWidget(FramelessWindow *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // Выполняем связку класса работы с портом окном настроек
    serial = new SerialGui(ui->boxPorts,                   // ComboBox c доступными Com портами
                           ui->boxBaudrate,                // ComboBox с настройками скорости
                           ui->boxParity,                  // ComboBox с настройками паритета
                           ui->boxData,                    // ComboBox с настройками бит данных
                           ui->boxStopBits,                // ComboBox с настройками стоп-бит
                           ui->boxFlowControl,             // ComboBox с настройками контроля
                           ui->buttonConnectDisconnect);   // Кнопка подключения/отключения
    // Выполняем связку класса работы с консолью с формами
    console = new ConsoleWidget(this,
                                serial,                    // Указатель на SerialSettings экземпляр
                                ui->consoleField,          // Указатель на QPlainTextEdit форму
                                ui->inputConsoleField,     // Указатель на QLineEdit форму
                                ui->sendConsoleButton,     // Указатель на QPushButton форму
                                ui->clearConsoleButton);   // Указатель на QPushButton форму
    // Создаём и связываем табличную консоль с формой
    tableConsole = new TableConsole(this,
                                    serial,                // Указатель на SerialSettings экземпляр
                                    ui->tableField,        // Указатель на QTableView форму
                                    ui->inputTableField,   // Указатель на QLineEdit форму
                                    ui->sendTableButton,   // Указатель на QPushButton форму
                                    ui->clearTableButton); // Указатель на QPushButton форму
    converter = new Converter(this,
                              ui->converterSource,         // QPlainText для ввода
                              ui->converterResult,         // QplainText для вывода результата
                              ui->converterConvertButton,  // Кнопка выполнения конвертации
                              ui->converterSwapButton,     // Кнопка изменения направления преобразования
                              ui->converterClearButton,    // Кнопка очистки обоих QPlainText
                              ui->converterSourceBox,      // QComboBox селектор исходного формата
                              ui->converterResultBox);     // QComboBox селектор формата вывода

    settings     = new SettingsController(this, ui);
    codecList    = new QStringList;
    languageList = new QStringList;
    themeList    = new QList<Decorator*>;

    this->setMinimumSize(600, 300);

    // Внешний вид приложения
    setPropertiesToMainWidget();
    setPropertiesToConsole();
    setPropertiesToTable();
    setPropertiesToConverter();


    setAppFont();





    fillLanguageList();
    fillCodecList();
    fillThemeList();

    applyColorScheme(0);


    // Подключение кнопок закрыть, свернуть, развернуть окно, так как стандартные скрыты
    connect(ui->closeButton,    &QToolButton::clicked, parent, &QWidget::close);
    connect(ui->minimizeButton, &QToolButton::clicked, parent, &QWidget::showMinimized);
    connect(ui->maximazeButton, &QToolButton::clicked, [parent](){ parent->maximizeFramelessWindow();});
    // Дополнительные функциональные кнопки
    connect(ui->showConnectionButton,    &QPushButton::clicked,
            [this](){ui->workspaceWidget->setCurrentIndex(quickIndexSettings);
                     ui->rightStackedPanel->setCurrentIndex(SettingsController::settingsMenu::settingsIndexConnection);});
    connect(ui->switchToConsoleButton,   &QPushButton::clicked,
            [this](){ui->workspaceWidget->setCurrentIndex(quickIndexConsole);});
    connect(ui->switchToTableButton,     &QPushButton::clicked,
            [this](){ui->workspaceWidget->setCurrentIndex(quickIndexTable);});
    connect(ui->switchToConverterButton, &QPushButton::clicked,
            [this](){ui->workspaceWidget->setCurrentIndex(quickIndexConverter);});
    connect(ui->showSettingsButton,      &QPushButton::clicked,
            [this](){ui->workspaceWidget->setCurrentIndex(quickIndexSettings);});
    //
    connect(settings, &SettingsController::currentThemeChanged, this, &MainWidget::applyColorScheme);
    //connect(settings, &SettingsController::currentLanguageChanged, this, &MainWidget::);
    connect(settings, &SettingsController::currentTextCodecChanged, this, &MainWidget::setAppTextCodec);
    connect(settings, &SettingsController::consoleEchoChanged, console, &ConsoleWidget::setEchoMode);
    connect(settings, &SettingsController::tableEchoChanged, tableConsole, &TableConsole::setEchoMode);

    qDebug() << QTextCodec::availableCodecs();
}

MainWidget::~MainWidget(){
    delete themeList;
    delete languageList;
    delete codecList;
    delete settings;
    delete darkTheme;
    delete fakeTheme;
    delete converter;
    delete tableConsole;
    delete console;
    delete serial;
    delete appFont;
    delete ui;
}


void MainWidget::setPropertiesToMainWidget(void){
    ui->closeButton->setText("");
    ui->maximazeButton->setText("");
    ui->minimizeButton->setText("");
    ui->appName->setText("");
    ui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->showConnectionButton->setMinimumSize(50,50);
    ui->showConnectionButton->setMinimumSize(50,50);
    ui->showSettingsButton->setMinimumSize(50,50);
    ui->switchToConsoleButton->setMinimumSize(50,50);
    ui->switchToTableButton->setMinimumSize(50,50);
    ui->switchToConverterButton->setMinimumSize(50,50);
    ui->showConnectionButton->setText("");
    ui->showConnectionButton->setText("");
    ui->showSettingsButton->setText("");
    ui->switchToConsoleButton->setText("");
    ui->switchToTableButton->setText("");
    ui->switchToConverterButton->setText("");
}
void MainWidget::setPropertiesToConsole(void){

}
void MainWidget::setPropertiesToTable(void){

}
void MainWidget::setPropertiesToConverter(void){

}
void MainWidget::setAppFont(){
    QApplication::setFont(*appFont);
}
void MainWidget::setAppTextCodec(int index){
    QString codecName = this->codecList->at(index);
    converter->setCurrentCodec(codecName.toLatin1());
}
void MainWidget::fillLanguageList(){
    languageList->append("English");
    languageList->append("Русский");
    settings->setLanguageList(languageList);
}
void MainWidget::fillCodecList(){
    codecList->append("CP866");
    codecList->append("windows-1251");
    settings->setCodecList(codecList);
}
void MainWidget::fillThemeList(){
    darkTheme = new Decorator("Gray",
                              Decorator::Light,
                              {0x1a1c20, 0xdcddde, 0x3d563d , 0xdcddde},
                              {0x2b2d33, 0xdcddde, 0x3d563d , 0xdcddde},
                              {0x33363d, 0xdcddde, 0x3d563d , 0xdcddde});
    themeList->append(darkTheme);

    fakeTheme = new Decorator("HUUI",
                              Decorator::Dark,
                              {0x1a8456, 0xdcddde, 0x0d563d, 0xdcddde,},
                              {0x2b2d33, 0xFFFFFF, 0x0d563d, 0xdcddde,},
                              {0x31363d, 0xdcddde, 0x0d563d, 0xdcddde,});
    themeList->append(fakeTheme);

    QStringList list;
    foreach (const Decorator *temp, *themeList)
        list.append(temp->name());
    settings->setThemeList(&list);
}
void MainWidget::applyColorScheme(int indexOfTheme){
    applyColorSchemeToMainWidget(themeList->at(indexOfTheme));   // Основная рамка
    applyColorSchemetoConsole(themeList->at(indexOfTheme));      // Виджет консоли
    applyColorSchemeToTable(themeList->at(indexOfTheme));        // Виджет таблицы
    applyColorSchemeToConverter(themeList->at(indexOfTheme));    // Конвертер
    applyColorSchemeToSettings(themeList->at(indexOfTheme));     // Виджет настроек
    this->setAutoFillBackground(true);                           // Этот флаг слетает, нужно восстановить
}
void MainWidget::applyColorSchemeToMainWidget(Decorator *scheme){
    scheme->setBasicColorsToWidget  (this, scheme->baseColor());
    scheme->applyToAppLabel         (ui->appName);
    scheme->applyToCloseButton      (ui->closeButton);
    scheme->applyToMaximizeButton   (ui->maximazeButton);
    scheme->applyToMinimizeButton   (ui->minimizeButton);
    scheme->applyToConnectionButton (ui->showConnectionButton);
    scheme->applyToConsoleButton    (ui->switchToConsoleButton);
    scheme->applyToTableButton      (ui->switchToTableButton);
    scheme->applyToConverterButton  (ui->switchToConverterButton);
    scheme->applyToSettingsButton   (ui->showSettingsButton);
}
void MainWidget::applyColorSchemetoConsole(Decorator *scheme){
    scheme->applyToConsoleWidget    (ui->consoleField, ui->inputConsoleField, ui->sendConsoleButton, ui->clearConsoleButton);
}

void MainWidget::applyColorSchemeToTable(Decorator *scheme){
    scheme->applyToTableWidget      (ui->tableField, ui->inputTableField, ui->sendTableButton, ui->clearTableButton);
}

void MainWidget::applyColorSchemeToConverter(Decorator *scheme){
    scheme->setBasicColorsToWidget(ui->converterPage, scheme->secondColor());
    scheme->applyToConverterWidget(ui->converterSource, ui->converterResult, ui->converterSourceBox, ui->converterResultBox,
                                   ui->converterConvertButton, ui->converterSwapButton, ui->converterClearButton);
}
void MainWidget::applyColorSchemeToSettings(Decorator *scheme){
    scheme->setScrollBarColors      (ui->settingsLeftPanel->horizontalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setScrollBarColors      (ui->settingsLeftPanel->verticalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setBasicColorsToWidget  (ui->leftPanelContents, scheme->baseColor());
    scheme->setBasicColorsToWidget  (ui->settingsPage, scheme->baseColor());
    scheme->setSettingsButtonsColors(ui->connectionContentsButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->generalContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->consoleContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->tableContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->logsContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setSettingsButtonsColors(ui->bindsContentButton, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    scheme->setScrollBarColors      (ui->settingsRightPanel->horizontalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setScrollBarColors      (ui->settingsRightPanel->verticalScrollBar(), scheme->secondColor(), scheme->baseColor());
    scheme->setBasicColorsToWidget  (ui->rightPanelContents, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->rightStackedPanel, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->labelSectionConnection, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->labelPorts, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->labelBaudrate, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->labelData, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->labelParity, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->labelStopBits, scheme->secondColor());
    scheme->setBasicColorsToWidget  (ui->labelFlowControl, scheme->secondColor());
    // Вкладка Connection
    scheme->setComboBoxColors(ui->boxPorts);
    scheme->setComboBoxColors(ui->boxData);
    scheme->setComboBoxColors(ui->boxParity);
    scheme->setComboBoxColors(ui->boxBaudrate);
    scheme->setComboBoxColors(ui->boxStopBits);
    scheme->setComboBoxColors(ui->boxFlowControl);
    scheme->setStandartButtonColors(ui->buttonConnectDisconnect, scheme->baseColor(), scheme->thirdColor(), scheme->secondColor());
    // Вкладка General
    scheme->setComboBoxColors(ui->comboBoxTheme);
    scheme->setComboBoxColors(ui->comboBoxLanguage);
    scheme->setComboBoxColors(ui->comboBoxCodec);
    scheme->setBasicColorsToWidget(ui->labelTheme, scheme->secondColor());
    scheme->setBasicColorsToWidget(ui->labelCodec, scheme->secondColor());
    scheme->setBasicColorsToWidget(ui->labelLanguage, scheme->secondColor());
    scheme->setBasicColorsToWidget(ui->labelSectionGeneral, scheme->secondColor());
    // Вкладка Table
    scheme->setBasicColorsToWidget(ui->labelSectionTable, scheme->secondColor());
    scheme->setBasicColorsToWidget(ui->labelTableEcho, scheme->secondColor());
    scheme->setCheckBoxColors(ui->checkboxTableEcho, scheme->baseColor(), scheme->thirdColor(), scheme->baseColor());
    // Вкладка Console
    scheme->setBasicColorsToWidget(ui->labelSectionConsole, scheme->secondColor());
    scheme->setBasicColorsToWidget(ui->labelConsoleEcho, scheme->secondColor());
    scheme->setCheckBoxColors(ui->checkboxConsoleEcho, scheme->baseColor(), scheme->thirdColor(), scheme->baseColor());
}


