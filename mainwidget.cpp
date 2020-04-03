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
#include <QSettings>


MainWidget::MainWidget(FramelessWindow *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setMinimumSize(600, 300);
    parent->setWindowTitle("Advanced Terminal");
    parent->setWindowIcon(QIcon(":/light/resources/icons/light/consolemode.png"));

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

    logger       = new Logger(this, serial);
    settings     = new GuiController(this, ui);
    codecList    = new QStringList;
    languageList = new QStringList;
    themeList    = new QList<Decorator*>;

    setAppFont();
    fillLanguageList();
    fillCodecList();
    fillThemeList();


    // Подключение кнопок закрыть, свернуть, развернуть окно, так как стандартные скрыты
    connect(ui->closeButton,    &QToolButton::clicked, parent, &QWidget::close);
    connect(ui->minimizeButton, &QToolButton::clicked, parent, &QWidget::showMinimized);
    connect(ui->maximazeButton, &QToolButton::clicked, [parent](){parent->maximizeFramelessWindow();});

    // Изменения настроек приложения
    connect(settings, &GuiController::currentThemeChanged, this, &MainWidget::applyColorScheme);
    connect(settings, &GuiController::currentLanguageChanged, this, &MainWidget::setAppLanguage);
    connect(settings, &GuiController::currentTextCodecChanged, this, &MainWidget::setAppTextCodec);

    connect(settings, &GuiController::consoleEchoChanged, console, &ConsoleWidget::setEchoMode);
    connect(settings, &GuiController::consoleCyclicChanged, console, &ConsoleWidget::setCyclicMode);
    connect(settings, &GuiController::consoleCyclicIntervalChanged, console, &ConsoleWidget::setCyclicInterval);
    connect(settings, &GuiController::consoleCyclicDataChanged, console, &ConsoleWidget::setBindData);
    connect(settings, &GuiController::consoleStartCycle, console, &ConsoleWidget::startCyclicSending);
    connect(settings, &GuiController::consoleStopCycle, console, &ConsoleWidget::stopCyclicSending);
    connect(console, &ConsoleWidget::cyclicStopped, settings, &GuiController::consoleCyclicStoped);

    connect(settings, &GuiController::tableEchoChanged, tableConsole, &TableConsole::setEchoMode);
    connect(settings, &GuiController::tableCyclicChanged, tableConsole, &TableConsole::setCyclicMode);
    connect(settings, &GuiController::tableCyclicIntervalChanged, tableConsole, &TableConsole::setCyclicInterval);
    connect(settings, &GuiController::tableCyclicDataChanged, tableConsole, &TableConsole::setBindData);
    connect(settings, &GuiController::tableStartCycle, tableConsole, &TableConsole::startCyclicSending);
    connect(settings, &GuiController::tableStopCycle, tableConsole, &TableConsole::stopCyclicSending);
    connect(tableConsole, &TableConsole::cyclicStopped, settings, &GuiController::tableCyclicStoped);

    connect(settings, &GuiController::captureTimeChanges, serial, &SerialGui::setCaptureTime);
    connect(settings, &GuiController::captureBytesChanges, serial, &SerialGui::setCaptureSize);

    connect(settings, &GuiController::logEnableChanged, logger, &Logger::setEnabled);
    connect(settings, &GuiController::logPathChanged, logger, &Logger::setPath);
    connect(settings, &GuiController::logColumnSizeChanged, logger, &Logger::setBytesInRow);
    connect(settings, &GuiController::logColumnSpacing, logger, &Logger::setColumnSpace);

    settings->restore();
}

MainWidget::~MainWidget(){
    settings->save();
    delete logger;
    delete themeList;
    delete languageList;
    delete codecList;
    delete settings;
    delete darkTheme;
    delete greenTheme;
    delete bitbucketTheme;
    delete githubTheme;
    delete purpleTheme;
    delete converter;
    delete tableConsole;
    delete console;
    delete serial;
    delete appFont;
    delete ui;
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
    Decorator::setTargetGui(ui); // Связываем декоратор с gui
    darkTheme = new Decorator("Gray",
                              Decorator::Light,
                              {0x1a1c20, 0xdcddde, 0x3d563d , 0xdcddde},
                              {0x2b2d33, 0xdcddde, 0x3d563d , 0xdcddde},
                              {0x33363d, 0xdcddde, 0x3d563d , 0xdcddde});
    greenTheme = new Decorator("Green",
                               Decorator::Light,
                               {0x1a8456, 0xdcddde, 0x0d563d, 0xdcddde,},
                               {0x2b2d33, 0xFFFFFF, 0x0d563d, 0xdcddde,},
                               {0x31363d, 0xdcddde, 0x0d563d, 0xdcddde,});
    bitbucketTheme = new Decorator("Bitbucket",
                                   Decorator::Light,
                                   {0x0044a8, 0xffffff, 0x0063ce, 0xffffff,},
                                   {0xf4f5f7, 0x3e516e, 0x0044a8, 0xffffff,},
                                   {0x1b3557, 0xf4f5f7, 0x0d563d, 0xdcddde,});
    githubTheme = new Decorator("Github",
                                Decorator::Light,
                                {0x1f242a, 0xf2f2f2, 0x3e4247, 0xf2f2f2,},
                                {0xf4f6f8, 0x393746, 0x5ec850, 0x393746,},
                                {0x3e4247, 0xf2f2f2, 0x0d563d, 0xf2f2f2,});
    purpleTheme = new Decorator("Purple",
                                Decorator::Light,
                                {0x342947, 0xd5d1df, 0xfdd552, 0x342947,},
                                {0x4f3e68, 0xd5d1df, 0xfdd552, 0x4f3e68,},
                                {0x776692, 0xd5d1df, 0xfdd552, 0x776692,});

    themeList->append(darkTheme);
    themeList->append(greenTheme);
    themeList->append(bitbucketTheme);
    themeList->append(githubTheme);
    themeList->append(purpleTheme);

    QStringList list;
    foreach (const Decorator *temp, *themeList)
        list.append(temp->name());
    settings->setThemeList(&list);
}
void MainWidget::applyColorScheme(int indexOfTheme){
    themeList->at(indexOfTheme)->apply();
}
void MainWidget::setAppLanguage(int index){
    QStringList langList;
    langList << ":/resources/language_en.qm";
    langList << ":/resources/language_ru.qm";
    language.load(langList.at(index));
    qApp->installTranslator(&language);
    serial->retranslate();
    console->retranslate();
    tableConsole->retranslate();
    converter->retranslate();
    settings->retranstate();
}

