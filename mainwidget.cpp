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
#include <QFileDialog>


MainWidget::MainWidget(FramelessWindow *parent)
    : FramelessWindow(parent)
    , gui(new Ui::MainWidget)
{
    gui->setupUi(centralWidget());
    uiProxy = new UiProxy(this, gui);

    setMinimumSize(725, 445);
    setWindowTitle("CuteCOM");
    setWindowIcon(QIcon(":/icons/appication.ico"));
    centralWidget()->layout()->layout()->setMargin(0);
    centralWidget()->layout()->layout()->setSpacing(0);

    installEventFilter(this);
    setProperties();
    connectQuickMenu();
    connectSettingsMenu();
    connectSectionConnection();
    connectSectionGeneral();
    connectSectionConsole();

    connectSectionTable();
    connectSectionLog();

    // Подключение кнопок закрыть, свернуть, развернуть окно, так как стандартные скрыты
    connect(gui->closeButton,    &QToolButton::clicked, this, &FramelessWindow::close);
    connect(gui->minimizeButton, &QToolButton::clicked, this, &FramelessWindow::showMinimized);
    connect(gui->maximazeButton, &QToolButton::clicked, this, &FramelessWindow::changeFullScreenMode);
    connect(this, &FramelessWindow::windowMaximized, [this](){gui->maximazeButton->setChecked(true);});
    connect(this, &FramelessWindow::windowNormalized, [this](){gui->maximazeButton->setChecked(false);});
    restoreSettings();
}
MainWidget::~MainWidget(){
    saveSettings();
    delete uiProxy;
    delete hexValidator;
    delete gui;
}
void MainWidget::connectQuickMenu(void){
    connect(gui->showConnectionButton,    &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexSettings);
                  gui->rightStackedPanel->setCurrentIndex(MainWidget::settingsMenu::settingsIndexConnection);});
    connect(gui->switchToConsoleButton,   &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexConsole);});
    connect(gui->switchToTableButton,     &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexTable);});
    connect(gui->switchToConverterButton, &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexConverter);});
    connect(gui->showSettingsButton,      &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexSettings);});
}
void MainWidget::connectSettingsMenu(void){
    connect(gui->connectionContentsButton, &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexConnection);});
    connect(gui->generalContentButton,     &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexGeneral);});
    connect(gui->consoleContentButton,     &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexConsole);});
    connect(gui->tableContentButton,       &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexTable);});
    connect(gui->logsContentButton,        &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexLogs);});
    connect(gui->bindsContentButton,       &QPushButton::clicked,
            [=](){gui->rightStackedPanel->setCurrentIndex(settingsIndexBinds);});
}
void MainWidget::connectSectionConnection(void){

}
void MainWidget::connectSectionGeneral(void){
    connect(gui->comboBoxTheme, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWidget::generalSettingsChanged);
    connect(gui->comboBoxCodec, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWidget::generalSettingsChanged);
    connect(gui->comboBoxLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWidget::generalSettingsChanged);
    connect(gui->spinBoxCaptureTime, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWidget::generalSettingsChanged);
    connect(gui->spinBoxCaptureBytes, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWidget::generalSettingsChanged);
    connect(gui->buttonGeneralApply, &QPushButton::pressed, this, &MainWidget::generalSettingsChanged);
}
void MainWidget::connectSectionConsole(void){
    connect(gui->checkboxConsoleEcho, &QCheckBox::stateChanged,
            [=](int state){emit uiProxy->consoleEchoChanged(state == Qt::CheckState::Checked ? true:false);});
    connect(gui->buttonConsoleHotKey1, &QToolButton::clicked, this, &MainWidget::consoleHotkeys);
    connect(gui->buttonConsoleHotKey2, &QToolButton::clicked, this, &MainWidget::consoleHotkeys);
    connect(gui->buttonConsoleHotKey3, &QToolButton::clicked, this, &MainWidget::consoleHotkeys);
    connect(gui->buttonConsoleHotKey4, &QToolButton::clicked, this, &MainWidget::consoleHotkeys);
    connect(uiProxy, &UiProxy::consoleCyclicStoped, this, &MainWidget::consoleCyclicStoped);
}
void MainWidget::connectSectionTable(void){
    connect(gui->checkboxTableEcho, &QCheckBox::stateChanged,
            [=](int state){emit uiProxy->tableEchoChanged(state == Qt::CheckState::Checked ? true:false);});
    connect(gui->buttonTableHotKey1, &QToolButton::clicked, this, &MainWidget::tableHotkeys);
    connect(gui->buttonTableHotKey2, &QToolButton::clicked, this, &MainWidget::tableHotkeys);
    connect(gui->buttonTableHotKey3, &QToolButton::clicked, this, &MainWidget::tableHotkeys);
    connect(gui->buttonTableHotKey4, &QToolButton::clicked, this, &MainWidget::tableHotkeys);
    connect(gui->lineEditTableHotKey1, &QLineEdit::textChanged, this, &MainWidget::hexDelimiters);
    connect(gui->lineEditTableHotKey2, &QLineEdit::textChanged, this, &MainWidget::hexDelimiters);
    connect(gui->lineEditTableHotKey3, &QLineEdit::textChanged, this, &MainWidget::hexDelimiters);
    connect(gui->lineEditTableHotKey4, &QLineEdit::textChanged, this, &MainWidget::hexDelimiters);
    connect(uiProxy, &UiProxy::tableCyclicStoped, this, &MainWidget::tableCyclicStoped);
}
void MainWidget::connectSectionLog(void){
    connect(gui->checkBoxLogEnable, &QCheckBox::stateChanged, [=](int state){emit uiProxy->logEnableChanged(state == Qt::CheckState::Checked ? true:false);});
    connect(gui->buttonLogPath, &QToolButton::clicked, this, &MainWidget::selectLogPath);
    connect(gui->buttonLogApply, &QPushButton::pressed, [=](){
            emit uiProxy->logColumnSizeChanged(gui->spinBoxLogColumnSize->value());
            emit uiProxy->logColumnSpacing(gui->spinBoxLogSpace->value());});
}
void MainWidget::setProperties (void){
    setPropertiesMainWidget();
    setPropertiesMenu();
    setPropertiesSectionConnection();
    setPropertiesSectionGeneral();
    setPropertiesSectionConsole();
    setPropertiesSectionTable();
    setPropertiesSectionLogs();
    setPropertiesSectionBinds();
}
void MainWidget::retranstate(void){
    setTextSettingsMenu();
    setTextSectionConnection();
    setTextSectionGeneral();
    setTextSectionConsole();
    setTextSectionTable();
    setTextSectionLogs();
    setTextSectionBinds();
}
void MainWidget::setPropertiesMainWidget(void){
    gui->centralWidget->layout()->setMargin(0);
    gui->closeButton->setText("");
    gui->maximazeButton->setText("");
    gui->maximazeButton->setCheckable(true);
    gui->minimizeButton->setText("");
    gui->appName->setText("");
    gui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    gui->showConnectionButton->setText("");
    gui->showConnectionButton->setText("");
    gui->showSettingsButton->setText("");
    gui->switchToConsoleButton->setText("");
    gui->switchToTableButton->setText("");
    gui->switchToConverterButton->setText("");
}
void MainWidget::setPropertiesMenu(void){
    gui->infoLabel->setFont(QFont("Arial", 7, QFont::Bold));
    gui->infoLabel->setAlignment(Qt::AlignRight);
    gui->infoLabel->setMargin(10);
    gui->settingsPage->layout()->setMargin(0);
    gui->pageDelimiterLayout->setMargin(0);
    gui->leftPanelContents->layout()->setMargin(0);
    gui->rightPanelContents->layout()->setMargin(0);
    gui->rightStackedPanel->layout()->setMargin(0);
    gui->pageDelimiterLayout->setSpacing(0);
}
void MainWidget::setPropertiesSectionConnection (void){
    gui->labelSectionConnection->setFont(QFont("Arial", 14, QFont::Bold));
}
void MainWidget::setPropertiesSectionGeneral (void){
    gui->labelSectionGeneral->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->spinBoxCaptureTime->setRange(0, 2147483647);
    gui->spinBoxCaptureBytes->setRange(0, 2147483647);
}
void MainWidget::setPropertiesSectionConsole (void){
    gui->labelSectionConsole->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->checkboxConsoleEcho->setText("");
    gui->checkboxConsoleCyclic->setText("");
    gui->buttonConsoleHotKey1->setText(" ");
    gui->buttonConsoleHotKey2->setText(" ");
    gui->buttonConsoleHotKey3->setText(" ");
    gui->buttonConsoleHotKey4->setText(" ");
    gui->buttonConsoleHotKey1->setCheckable(true);
    gui->buttonConsoleHotKey2->setCheckable(true);
    gui->buttonConsoleHotKey3->setCheckable(true);
    gui->buttonConsoleHotKey4->setCheckable(true);
    gui->spinboxConsoleCyclicInterval->setRange(5, 10000);
}
void MainWidget::setPropertiesSectionTable (void){
    gui->labelSectionTable->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->checkboxTableEcho->setText("");
    gui->checkboxTableCyclic->setText("");
    gui->buttonTableHotKey1->setText(" ");
    gui->buttonTableHotKey2->setText(" ");
    gui->buttonTableHotKey3->setText(" ");
    gui->buttonTableHotKey4->setText(" ");
    gui->buttonTableHotKey1->setCheckable(true);
    gui->buttonTableHotKey2->setCheckable(true);
    gui->buttonTableHotKey3->setCheckable(true);
    gui->buttonTableHotKey4->setCheckable(true);
    gui->spinboxTableCyclicInterval->setRange(5, 10000);
    gui->lineEditTableHotKey1->setValidator(hexValidator);
    gui->lineEditTableHotKey2->setValidator(hexValidator);
    gui->lineEditTableHotKey3->setValidator(hexValidator);
    gui->lineEditTableHotKey4->setValidator(hexValidator);
}
void MainWidget::setPropertiesSectionLogs (void){
    gui->labelSectionLog->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->lineEditLogPath->setReadOnly(true);
    gui->checkBoxLogEnable->setText("");
    gui->spinBoxLogSpace->setRange(1, 60);
    gui->spinBoxLogColumnSize->setRange(1, 60);
}
void MainWidget::setPropertiesSectionBinds(void){
    gui->bindsContentButton->hide();
}
void MainWidget::setTextSettingsMenu(void){
    gui->connectionContentsButton->setText(tr("Connection"));
    gui->generalContentButton->setText(tr("General"));
    gui->consoleContentButton->setText(tr("Console"));
    gui->tableContentButton->setText(tr("Table"));
    gui->logsContentButton->setText(tr("Logs"));
    gui->bindsContentButton->setText(tr("Binds"));
    gui->infoLabel->setText(tr("version: ") + "0.0.0");
}
void MainWidget::setTextSectionConnection(void){
    gui->labelSectionConnection->setText(tr("Connection"));
    gui->labelPorts->setText(tr("Port:"));
    gui->labelData->setText(tr("Data:"));
    gui->labelStopBits->setText(tr("Stop bits:"));
    gui->labelParity->setText(tr("Parity:"));
    gui->labelBaudrate->setText(tr("Baudrate:"));
    gui->labelFlowControl->setText(tr("Flow control:"));
}
void MainWidget::setTextSectionGeneral(void){
    gui->labelSectionGeneral->setText(tr("General"));
    gui->labelTheme->setText(tr("Theme:"));
    gui->labelLanguage->setText(tr("Language:"));
    gui->labelCodec->setText(tr("Text codec:"));
    gui->labelCaptureTime->setText(tr("Capture time, ms:"));
    gui->labelCaptureBytes->setText(tr("Capture bytes:"));
    gui->buttonGeneralApply->setText(tr("Apply"));
}
void MainWidget::setTextSectionConsole(void){
    gui->labelSectionConsole->setText(tr("Console"));
    gui->labelConsoleEcho->setText(tr("Echo:"));
    gui->labelConsoleCyclic->setText(tr("Cyclic mode:"));
    gui->labelConsoleCyclicInterval->setText(tr("Interval, ms:"));
    gui->labelConsoleHotKey1->setText(tr("Hot key 1:"));
    gui->labelConsoleHotKey2->setText(tr("Hot key 2:"));
    gui->labelConsoleHotKey3->setText(tr("Hot key 3:"));
    gui->labelConsoleHotKey4->setText(tr("Hot key 4:"));
}
void MainWidget::setTextSectionTable(void){
    gui->labelSectionTable->setText(tr("Table"));
    gui->labelTableEcho->setText(tr("Echo:"));
    gui->labelTableCyclicMode->setText(tr("Cyclic mode:"));
    gui->labelTableCyclicInterval->setText(tr("Interval, ms:"));
    gui->labelTableHotKey1->setText(tr("Hot key 1:"));
    gui->labelTableHotKey2->setText(tr("Hot key 2:"));
    gui->labelTableHotKey3->setText(tr("Hot key 3:"));
    gui->labelTableHotKey4->setText(tr("Hot key 4:"));
}
void MainWidget::setTextSectionLogs(void){
    gui->labelSectionLog->setText(tr("Logs"));
    gui->labelLogEnable->setText(tr("Enable:"));
    gui->labelLogPath->setText(tr("Path:"));
    gui->labelLogSpace->setText(tr("Сolumn spacing:"));
    gui->labelLogColomnSize->setText(tr("Сolumn size, byte:"));
    gui->buttonLogApply->setText(tr("Apply"));
}
void MainWidget::setTextSectionBinds(void){

}
bool MainWidget::eventFilter(QObject *target, QEvent *event){
    if(event->type() != QKeyEvent::KeyPress || static_cast<QKeyEvent*>(event)->isAutoRepeat())
        return QObject::eventFilter(target, event);

    switch (static_cast<QKeyEvent*>(event)->key()) {
    case Qt::Key_Return:
        qDebug() << "Enter";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            emit gui->sendConsoleButton->clicked();
            return true;
        case quickIndexTable:
            emit gui->sendTableButton->clicked();
            return true;
        case quickIndexConverter:
            emit gui->converterConvertButton->clicked();
            return true;
        }
    case Qt::Key_1:
        qDebug() << "Key_1";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey1->setChecked(gui->buttonConsoleHotKey1->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey1->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey1->setChecked(gui->buttonTableHotKey1->isChecked() ? false:true);
            emit gui->buttonTableHotKey1->clicked();
            return true;
        }
        break;
    case Qt::Key_2:
        qDebug() << "Key_2";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey2->setChecked(gui->buttonConsoleHotKey2->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey2->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey2->setChecked(gui->buttonTableHotKey2->isChecked() ? false:true);
            emit gui->buttonTableHotKey2->clicked();
            return true;
        }
        break;
    case Qt::Key_3:
        qDebug() << "Key_3";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey3->setChecked(gui->buttonConsoleHotKey3->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey3->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey3->setChecked(gui->buttonTableHotKey3->isChecked() ? false:true);
            emit gui->buttonTableHotKey3->clicked();
            return true;
        }
        break;
    case Qt::Key_4:
        qDebug() << "Key_4";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            gui->buttonConsoleHotKey4->setChecked(gui->buttonConsoleHotKey4->isChecked() ? false:true);
            emit gui->buttonConsoleHotKey4->clicked();
            return true;
        case quickIndexTable:
            gui->buttonTableHotKey4->setChecked(gui->buttonTableHotKey4->isChecked() ? false:true);
            emit gui->buttonTableHotKey4->clicked();
            return true;
        }
        break;
    }
    return QObject::eventFilter(target, event);
}
void MainWidget::tableCyclicStoped(void){
    gui->buttonTableHotKey1->setChecked(false);
    gui->buttonTableHotKey2->setChecked(false);
    gui->buttonTableHotKey3->setChecked(false);
    gui->buttonTableHotKey4->setChecked(false);
}
void MainWidget::consoleCyclicStoped(void){
    gui->buttonConsoleHotKey1->setChecked(false);
    gui->buttonConsoleHotKey2->setChecked(false);
    gui->buttonConsoleHotKey3->setChecked(false);
    gui->buttonConsoleHotKey4->setChecked(false);
}
void MainWidget::consoleHotkeys(void){
    QString data;
    static QToolButton *button = nullptr;
    QToolButton* senderButton = static_cast<QToolButton*>(QObject::sender());
    if(button != senderButton || (button == senderButton && button->isChecked())){
        if(button != nullptr)
            button->setChecked(false);
        button = senderButton;
        if(button == gui->buttonConsoleHotKey1)
            data = gui->lineEditConsoleHotKey1->text();
        else if(button == gui->buttonConsoleHotKey2)
            data = gui->lineEditConsoleHotKey2->text();
        else if(button == gui->buttonConsoleHotKey3)
            data = gui->lineEditConsoleHotKey3->text();
        else if(button == gui->buttonConsoleHotKey4)
            data = gui->lineEditConsoleHotKey4->text();
        else
            return;
        emit uiProxy->consoleCyclicChanged(gui->checkboxConsoleCyclic->checkState() == Qt::CheckState::Checked ? true : false);
        emit uiProxy->consoleCyclicIntervalChanged(gui->spinboxConsoleCyclicInterval->value());
        emit uiProxy->consoleCyclicDataChanged(data);
        emit uiProxy->consoleStartCycle();
    }
    else {
        button = nullptr;
        emit uiProxy->consoleStopCycle();
    }
}
void MainWidget::tableHotkeys(void){
    QString data;
    static QToolButton *button = nullptr;
    QToolButton* senderButton = static_cast<QToolButton*>(QObject::sender());
    if(button != senderButton || (button == senderButton && button->isChecked())){
        if(button != nullptr)
            button->setChecked(false);
        button = static_cast<QToolButton*>(QObject::sender());
        if(button == gui->buttonTableHotKey1)
            data = gui->lineEditTableHotKey1->text();
        else if(button == gui->buttonTableHotKey2)
            data = gui->lineEditTableHotKey2->text();
        else if(button == gui->buttonTableHotKey3)
            data = gui->lineEditTableHotKey3->text();
        else if(button == gui->buttonTableHotKey4)
            data = gui->lineEditTableHotKey4->text();
        else
            return;
        emit uiProxy->tableCyclicChanged(gui->checkboxTableCyclic->checkState() == Qt::CheckState::Checked ? true : false);
        emit uiProxy->tableCyclicIntervalChanged(gui->spinboxTableCyclicInterval->value());
        emit uiProxy->tableCyclicDataChanged(data);
        emit uiProxy->tableStartCycle();
    }
    else {
        button = nullptr;
        emit uiProxy->tableStopCycle();
    }
}
void MainWidget::hexDelimiters(const QString& text){
    QLineEdit* lineEdit = static_cast<QLineEdit*>(QObject::sender());
    QString data = text;
    Converter::setDelimitersInHexString(data, 2, ' ');
    lineEdit->setText(data);
}
void MainWidget::generalSettingsChanged(void){
    QComboBox* comboBoxes = static_cast<QComboBox*>(QObject::sender());
    QSpinBox*  spinBoxes = static_cast<QSpinBox*>(QObject::sender());
    QPushButton*  button = static_cast<QPushButton*>(QObject::sender());
    static bool themeChanged = false;
    static bool codecChanged = false;
    static bool languageChanged = false;
    static bool captureTimeChanged = false;
    static bool captureBytesChanged = false;

    static int themeIndex = gui->comboBoxTheme->currentIndex();
    static int codecIndex = gui->comboBoxCodec->currentIndex();
    static int languageIndex = gui->comboBoxLanguage->currentIndex();
    static int captureTime = gui->spinBoxCaptureTime->value();
    static int captureBytes = gui->spinBoxCaptureBytes->value();

    if(comboBoxes == gui->comboBoxTheme){
        if(themeIndex != gui->comboBoxTheme->currentIndex())
            themeChanged = true;
        else
            themeChanged = false;
    }
    else if(comboBoxes == gui->comboBoxCodec){
        if(codecIndex != gui->comboBoxCodec->currentIndex())
            codecChanged = true;
        else
            codecChanged = false;
    }
    else if(comboBoxes == gui->comboBoxLanguage){
        if(languageIndex != gui->comboBoxLanguage->currentIndex())
            languageChanged = true;
        else
            languageChanged = false;
    }
    else if(spinBoxes == gui->spinBoxCaptureTime){
        if(captureTime !=  gui->spinBoxCaptureTime->value())
            captureTimeChanged = true;
        else
            captureTimeChanged = false;
    }
    else if(spinBoxes == gui->spinBoxCaptureBytes){
        if(captureBytes !=  gui->spinBoxCaptureBytes->value())
            captureBytesChanged = true;
        else
            captureBytesChanged = false;
    }
    else if(button == gui->buttonGeneralApply){
        if(themeChanged){
            themeIndex = gui->comboBoxTheme->currentIndex();
            uiProxy->setTheme(themeIndex);
            themeChanged = false;
        }
        if(codecChanged){
            codecIndex = gui->comboBoxCodec->currentIndex();
            uiProxy->setTextCodec(codecIndex);
            codecChanged = false;
        }
        if(languageChanged){
            languageIndex = gui->comboBoxLanguage->currentIndex();
            uiProxy->setLanguage(languageIndex);
            retranstate();
            languageChanged = false;
        }
        if(captureTimeChanged){
            captureTime = gui->spinBoxCaptureTime->value();
            emit uiProxy->captureTimeChanges(captureTime);
            captureTimeChanged = false;
        }
        if(captureBytesChanged){
            captureBytes = gui->spinBoxCaptureBytes->value();
            emit uiProxy->captureBytesChanges(captureBytes);
            captureBytesChanged = false;
        }
    }
    if(themeChanged||codecChanged||languageChanged||captureTimeChanged||captureBytesChanged)
        gui->buttonGeneralApply->show();
    else
         gui->buttonGeneralApply->hide();
}
void MainWidget::selectLogPath(void){
    QString previousPath = gui->lineEditLogPath->text();
    QString path = QFileDialog::getExistingDirectory(nullptr, tr("Select path"),
                                                     previousPath,
                                                     QFileDialog::ShowDirsOnly |
                                                     QFileDialog::DontResolveSymlinks);
    if(path == "")
        gui->lineEditLogPath->setText(previousPath);
    else
        gui->lineEditLogPath->setText(path);
    emit uiProxy->logPathChanged(gui->lineEditLogPath->text());
}
void MainWidget::saveSettings(void){
    QSettings settings("settings.conf", QSettings::IniFormat );

    settings.beginGroup("app");
    settings.setValue("size", this->normalGeometry());
    settings.endGroup();

    settings.beginGroup("general");
    settings.setValue("theme", gui->comboBoxTheme->currentIndex());
    settings.setValue("language", gui->comboBoxLanguage->currentIndex());
    settings.setValue("text codec", gui->comboBoxCodec->currentIndex());
    settings.setValue("capture_time", gui->spinBoxCaptureTime->value());
    settings.setValue("capture_bytes", gui->spinBoxCaptureBytes->value());
    settings.endGroup();

    settings.beginGroup("connection");
    settings.setValue("port", gui->boxPorts->currentText());
    settings.setValue("baudrate", gui->boxBaudrate->currentText());
    settings.setValue("data", gui->boxData->currentIndex());
    settings.setValue("parity", gui->boxParity->currentIndex());
    settings.setValue("stop bits", gui->boxStopBits->currentIndex());
    settings.setValue("flow control", gui->boxFlowControl->currentIndex());
    settings.endGroup();

    settings.beginGroup("console");
    settings.setValue("echo", gui->checkboxConsoleEcho->checkState());
    settings.setValue("cyclic", gui->checkboxConsoleCyclic->checkState());
    settings.setValue("interval", gui->spinboxConsoleCyclicInterval->value());
    settings.setValue("bind1", gui->lineEditConsoleHotKey1->text());
    settings.setValue("bind2", gui->lineEditConsoleHotKey2->text());
    settings.setValue("bind3", gui->lineEditConsoleHotKey3->text());
    settings.setValue("bind4", gui->lineEditConsoleHotKey4->text());
    settings.endGroup();

    settings.beginGroup("table");
    settings.setValue("echo", gui->checkboxTableEcho->checkState());
    settings.setValue("cyclic", gui->checkboxTableCyclic->checkState());
    settings.setValue("interval", gui->spinboxTableCyclicInterval->value());
    settings.setValue("bind1", gui->lineEditTableHotKey1->text());
    settings.setValue("bind2", gui->lineEditTableHotKey2->text());
    settings.setValue("bind3", gui->lineEditTableHotKey3->text());
    settings.setValue("bind4", gui->lineEditTableHotKey4->text());
    settings.endGroup();

    settings.beginGroup("log");
    settings.setValue("enable", gui->checkBoxLogEnable->checkState());
    settings.setValue("path", gui->lineEditLogPath->text());
    settings.setValue("column_size", gui->spinBoxLogColumnSize->value());
    settings.setValue("column_spacing", gui->spinBoxLogSpace->value());
    settings.endGroup();
}
void MainWidget::restoreSettings(void){
    QSettings settings("settings.conf", QSettings::IniFormat );

    settings.beginGroup("app");
    this->setGeometry(settings.value("size", this->normalGeometry()).toRect());
    settings.endGroup();

    settings.beginGroup("general");
    gui->comboBoxTheme->setCurrentIndex(settings.value("theme", 0).toInt());
    gui->comboBoxLanguage->setCurrentIndex(settings.value("language", 0).toInt());
    gui->comboBoxCodec->setCurrentIndex(settings.value("text codec", 0).toInt());
    gui->spinBoxCaptureTime->setValue(settings.value("capture_time", 50).toInt());
    gui->spinBoxCaptureBytes->setValue(settings.value("capture_bytes", 100).toInt());
    emit uiProxy->setTheme(gui->comboBoxTheme->currentIndex());
    emit uiProxy->setLanguage(gui->comboBoxLanguage->currentIndex());
    retranstate();
    emit uiProxy->setTextCodec(gui->comboBoxCodec->currentIndex());
    emit uiProxy->captureTimeChanges(gui->spinBoxCaptureTime->value());
    emit uiProxy->captureBytesChanges(gui->spinBoxCaptureBytes->value());
    emit gui->buttonGeneralApply->pressed();
    settings.endGroup();

    settings.beginGroup("console");
    gui->checkboxConsoleEcho->setCheckState(settings.value("echo", Qt::Checked).value<Qt::CheckState>());
    gui->checkboxConsoleCyclic->setCheckState(settings.value("cyclic", Qt::Checked).value<Qt::CheckState>());
    gui->spinboxConsoleCyclicInterval->setValue(settings.value("interval", 1000).toInt());
    gui->lineEditConsoleHotKey1->setText(settings.value("bind1", "Hot key text 1").toString());
    gui->lineEditConsoleHotKey2->setText(settings.value("bind2", "Hot key text 2").toString());
    gui->lineEditConsoleHotKey3->setText(settings.value("bind3", "Hot key text 3").toString());
    gui->lineEditConsoleHotKey4->setText(settings.value("bind4", "Hot key text 4").toString());
    emit uiProxy->consoleEchoChanged(gui->checkboxConsoleEcho->checkState() == Qt::Checked ? true : false);
    emit uiProxy->consoleCyclicChanged(gui->checkboxConsoleCyclic->checkState() == Qt::Checked ? true : false);
    emit uiProxy->consoleCyclicIntervalChanged(gui->spinboxConsoleCyclicInterval->value());
    settings.endGroup();

    settings.beginGroup("table");
    gui->checkboxTableEcho->setCheckState(settings.value("echo", Qt::Checked).value<Qt::CheckState>());
    gui->checkboxTableCyclic->setCheckState(settings.value("cyclic", Qt::Checked).value<Qt::CheckState>());
    gui->spinboxTableCyclicInterval->setValue(settings.value("interval", 1000).toInt());
    gui->lineEditTableHotKey1->setText(settings.value("bind1", "48 6F 74 20 6B 65 79 20 74 65 78 74 20 31").toString());
    gui->lineEditTableHotKey2->setText(settings.value("bind2", "48 6F 74 20 6B 65 79 20 74 65 78 74 20 32").toString());
    gui->lineEditTableHotKey3->setText(settings.value("bind3", "48 6F 74 20 6B 65 79 20 74 65 78 74 20 33").toString());
    gui->lineEditTableHotKey4->setText(settings.value("bind4", "48 6F 74 20 6B 65 79 20 74 65 78 74 20 34").toString());
    emit uiProxy->tableEchoChanged(gui->checkboxTableEcho->checkState() == Qt::Checked ? true : false);
    emit uiProxy->tableCyclicChanged(gui->checkboxTableCyclic->checkState() == Qt::Checked ? true : false);
    emit uiProxy->tableCyclicIntervalChanged(gui->spinboxTableCyclicInterval->value());
    settings.endGroup();

    settings.beginGroup("connection");                                                    // Настройки по-умолчанию
    gui->boxPorts->setCurrentText(settings.value("port", "").toString());                 //
    gui->boxBaudrate->setCurrentText(settings.value("baudrate", "9600").toString());      //
    gui->boxData->setCurrentIndex(settings.value("data", 3).toInt());                     // индекс 3 соответствует Databit = 8
    gui->boxParity->setCurrentIndex(settings.value("parity", 0).toInt());                 // индекс 0 соответствует Parity = NONE
    gui->boxStopBits->setCurrentIndex(settings.value("stop bits", 0).toInt());            // индекс 0 соответствует Stopbit = 1
    gui->boxFlowControl->setCurrentIndex(settings.value("flow control", 0).toInt());      // индекс 0 соответствует Flow control = NONE
    settings.endGroup();

    settings.beginGroup("log");
    gui->checkBoxLogEnable->setCheckState(settings.value("enable", Qt::Unchecked).value<Qt::CheckState>());
    gui->lineEditLogPath->setText(settings.value("path", QApplication::applicationDirPath()+"/logs").toString());
    gui->spinBoxLogColumnSize->setValue(settings.value("column_size", 20 ).toInt());
    gui->spinBoxLogSpace->setValue(settings.value("column_spacing", 5).toInt());
    emit uiProxy->logEnableChanged(gui->checkBoxLogEnable->checkState() == Qt::Checked ? true : false);
    emit uiProxy->logPathChanged(gui->lineEditLogPath->text());
    emit uiProxy->logColumnSizeChanged(gui->spinBoxLogColumnSize->value());
    emit uiProxy->logColumnSpacing(gui->spinBoxLogSpace->value());
    settings.endGroup();
}
