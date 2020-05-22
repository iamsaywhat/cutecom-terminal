#include "uiproxy.h"

UiProxy::UiProxy(QObject *parent, Ui::MainWidget* gui) :QObject(parent)
{
    Decorator::setTargetGui(gui);

    port = new SerialGui(gui->boxPorts,                   // ComboBox c доступными Com портами
                         gui->boxBaudrate,                // ComboBox с настройками скорости
                         gui->boxParity,                  // ComboBox с настройками паритета
                         gui->boxData,                    // ComboBox с настройками бит данных
                         gui->boxStopBits,                // ComboBox с настройками стоп-бит
                         gui->boxFlowControl,             // ComboBox с настройками контроля
                         gui->buttonConnectDisconnect);   // Кнопка подключения/отключения
    terminal = new Terminal(this,
                           port,                       // Указатель на SerialSettings экземпляр
                           gui->consoleField,          // Указатель на QPlainTextEdit форму
                           gui->inputConsoleField,     // Указатель на QLineEdit форму
                           gui->sendConsoleButton,     // Указатель на QPushButton форму
                           gui->clearConsoleButton);   // Указатель на QPushButton форму
    table = new Table(this,
                      port,                   // Указатель на SerialSettings экземпляр
                      gui->tableField,        // Указатель на QTableView форму
                      gui->inputTableField,   // Указатель на QLineEdit форму
                      gui->sendTableButton,   // Указатель на QPushButton форму
                      gui->clearTableButton); // Указатель на QPushButton форму
    converter = new Converter(this,
                              gui->converterSource,         // QPlainText для ввода
                              gui->converterResult,         // QplainText для вывода результата
                              gui->converterConvertButton,  // Кнопка выполнения конвертации
                              gui->converterSwapButton,     // Кнопка изменения направления преобразования
                              gui->converterClearButton,    // Кнопка очистки обоих QPlainText
                              gui->converterSourceBox,      // QComboBox селектор исходного формата
                              gui->converterResultBox);     // QComboBox селектор формата вывода
    logger = new Logger(this, port);

    createCodecList(gui->comboBoxCodec);
    createThemeList(gui->comboBoxTheme);
    createLanguageList(gui->comboBoxLanguage);
    setFont();

    connect(this, &UiProxy::consoleEchoChanged, terminal, &Terminal::setEchoMode);
    connect(this, &UiProxy::consoleCyclicChanged, terminal, &Terminal::setCyclicMode);
    connect(this, &UiProxy::consoleCyclicIntervalChanged, terminal, &Terminal::setCyclicInterval);
    connect(this, &UiProxy::consoleCyclicDataChanged, terminal, &Terminal::setBindData);
    connect(this, &UiProxy::consoleStartCycle, terminal, &Terminal::startCyclicSending);
    connect(this, &UiProxy::consoleStopCycle, terminal, &Terminal::stopCyclicSending);
    connect(terminal, &Terminal::cyclicStopped, this, &UiProxy::consoleCyclicStoped);

    connect(this, &UiProxy::tableEchoChanged, table, &Table::setEchoMode);
    connect(this, &UiProxy::tableCyclicChanged, table, &Table::setCyclicMode);
    connect(this, &UiProxy::tableCyclicIntervalChanged, table, &Table::setCyclicInterval);
    connect(this, &UiProxy::tableCyclicDataChanged, table, &Table::setBindData);
    connect(this, &UiProxy::tableStartCycle, table, &Table::startCyclicSending);
    connect(this, &UiProxy::tableStopCycle, table, &Table::stopCyclicSending);
    connect(table, &Table::cyclicStopped, this, &UiProxy::tableCyclicStoped);

    connect(this, &UiProxy::captureTimeChanges, port, &SerialGui::setCaptureTime);
    connect(this, &UiProxy::captureBytesChanges, port, &SerialGui::setCaptureSize);

    connect(this, &UiProxy::logEnableChanged, logger, &Logger::setEnabled);
    connect(this, &UiProxy::logPathChanged, logger, &Logger::setPath);
    connect(this, &UiProxy::logColumnSizeChanged, logger, &Logger::setBytesInRow);
    connect(this, &UiProxy::logColumnSpacing, logger, &Logger::setColumnSpace);
}
UiProxy::~UiProxy(void){
    delete codecList;
    delete languageList;
    delete themeNameList;
    delete themeList;

    delete appFont;

    delete darkTheme;
    delete greenTheme;
    delete bitbucketTheme;
    delete githubTheme;
    delete purpleTheme;

    delete logger;
    delete converter;
    delete table;
    delete terminal;
    delete port;
}
void UiProxy::createLanguageList(QComboBox *comboBox){
    languageList = new QStringList;
    languageList->append("English");
    languageList->append("Русский");
    comboBox->addItems(*languageList);
}
void UiProxy::createCodecList(QComboBox *comboBox){
    codecList = new QStringList;
    codecList->append("CP866");
    codecList->append("windows-1251");
    comboBox->addItems(*codecList);
}
void UiProxy::createThemeList(QComboBox *comboBox){
    themeList = new QList<Decorator*>;
    themeNameList = new QStringList;
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

    foreach (const Decorator *temp, *themeList)
        themeNameList->append(temp->name());

    comboBox->addItems(*themeNameList);
}
void UiProxy::setTheme(int indexOfTheme){
    themeList->at(indexOfTheme)->apply();
    qDebug() << "\nUiProxy: theme changed: " << themeList->at(indexOfTheme)->name();
}
void UiProxy::setTextCodec(int index){
    QString codecName = this->codecList->at(index);
    converter->setCurrentCodec(codecName.toLatin1());
    qDebug() << "\nUiProxy: text codec changed: " << codecList->at(index);
}
void UiProxy::setLanguage(int index){
    QStringList langList;
    langList << ":/language/language_en.qm";
    langList << ":/language/language_ru.qm";
    language.load(langList.at(index));
    qApp->installTranslator(&language);
    port->retranslate();
    terminal->retranslate();
    table->retranslate();
    converter->retranslate();
    qDebug() << "\nUiProxy: language changed: " << langList.at(index);
}
void UiProxy::setFont(){
    QApplication::setFont(*appFont);
}
