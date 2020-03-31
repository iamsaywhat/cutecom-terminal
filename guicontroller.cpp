#include "guicontroller.h"
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QDebug>

Ui::MainWidget* GuiController::gui = nullptr;

GuiController::GuiController(QObject *parent, Ui::MainWidget* gui) : QObject(parent)
{
    GuiController::gui = gui;
    parent->installEventFilter(this);
    setProperties();

    // Обслуживание основного меню
    connect(gui->showConnectionButton,    &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexSettings);
                     gui->rightStackedPanel->setCurrentIndex(GuiController::settingsMenu::settingsIndexConnection);});
    connect(gui->switchToConsoleButton,   &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexConsole);});
    connect(gui->switchToTableButton,     &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexTable);});
    connect(gui->switchToConverterButton, &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexConverter);});
    connect(gui->showSettingsButton,      &QPushButton::clicked,
            [=](){gui->workspaceWidget->setCurrentIndex(quickIndexSettings);});
    // Переключение между вкладками
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
    // Вкладка General
    connect(gui->comboBoxTheme, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GuiController::currentThemeChanged);
    connect(gui->comboBoxCodec, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GuiController::currentTextCodecChanged);
    connect(gui->comboBoxLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GuiController::currentLanguageChanged);
    // Вкладка Console
    connect(gui->checkboxConsoleEcho, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->consoleEchoChanged(true);
                            else
                                emit this->consoleEchoChanged(false);});
    connect(gui->buttonConsoleHotKey1, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey2, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey3, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    connect(gui->buttonConsoleHotKey4, &QToolButton::clicked, this, &GuiController::consoleHotkeys);
    // Вкладка Table
    connect(gui->checkboxTableEcho, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->tableEchoChanged(true);
                            else
                                emit this->tableEchoChanged(false);});
    connect(gui->buttonTableHotKey1, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey2, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey3, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->buttonTableHotKey4, &QToolButton::clicked, this, &GuiController::tableHotkeys);
    connect(gui->lineEditTableHotKey1, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey2, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey3, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);
    connect(gui->lineEditTableHotKey4, &QLineEdit::textChanged, this, &GuiController::hexDelimiters);

    connect(gui->buttonLogPath, &QToolButton::clicked, this, &GuiController::selectLogPath);

    gui->checkboxConsoleEcho->setCheckState(Qt::CheckState::Checked);
    gui->checkboxTableEcho->setCheckState(Qt::CheckState::Checked);
}
GuiController::~GuiController(){
    delete hexValidator;
}
void GuiController::setProperties (void){
    setPropertiesMainWidget();
    setPropertiesMenu();
    setPropertiesSectionConnection();
    setPropertiesSectionGeneral();
    setPropertiesSectionConsole();
    setPropertiesSectionTable();
    setPropertiesSectionLogs();
}
void GuiController::setPropertiesMainWidget(void){
    gui->centralWidget->layout()->setMargin(0);
    gui->closeButton->setText("");
    gui->maximazeButton->setText("");
    gui->minimizeButton->setText("");
    gui->appName->setText("");
    gui->appName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    gui->showConnectionButton->setMinimumSize(50,50);
    gui->showConnectionButton->setMinimumSize(50,50);
    gui->showSettingsButton->setMinimumSize(50,50);
    gui->switchToConsoleButton->setMinimumSize(50,50);
    gui->switchToTableButton->setMinimumSize(50,50);
    gui->switchToConverterButton->setMinimumSize(50,50);
    gui->showConnectionButton->setText("");
    gui->showConnectionButton->setText("");
    gui->showSettingsButton->setText("");
    gui->switchToConsoleButton->setText("");
    gui->switchToTableButton->setText("");
    gui->switchToConverterButton->setText("");
}
void GuiController::setPropertiesMenu(void){
    gui->connectionContentsButton->setText(tr("Connection"));
    gui->generalContentButton->setText(tr("General"));
    gui->consoleContentButton->setText(tr("Console"));
    gui->tableContentButton->setText(tr("Table"));
    gui->logsContentButton->setText(tr("Logs"));
    gui->bindsContentButton->setText(tr("Binds"));
    gui->infoLabel->setText(tr("Version: 0.0.0"));
    gui->infoLabel->setFont(QFont("Arial", 8, QFont::Bold));
    gui->infoLabel->setAlignment(Qt::AlignRight);
    gui->settingsPage->layout()->setMargin(0);
    gui->pageDelimiterLayout->setMargin(0);
    gui->leftPanelContents->layout()->setMargin(0);
    gui->rightPanelContents->layout()->setMargin(0);
    gui->rightStackedPanel->layout()->setMargin(0);
    gui->pageDelimiterLayout->setSpacing(0);
}
void GuiController::setPropertiesSectionConnection (void){
    gui->labelSectionConnection->setText(tr("Connection"));
    gui->labelSectionConnection->setFont(QFont("Arial", 14, QFont::Bold));
    gui->labelPorts->setText(tr("Port:"));
    gui->labelData->setText(tr("Data:"));
    gui->labelStopBits->setText(tr("Stop bits:"));
    gui->labelParity->setText(tr("Parity:"));
    gui->labelBaudrate->setText(tr("Baudrate:"));
    gui->labelFlowControl->setText(tr("Flow control:"));
}
void GuiController::setPropertiesSectionGeneral (void){
    gui->labelSectionGeneral->setText(tr("General"));
    gui->labelSectionGeneral->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelTheme->setText(tr("Theme:"));
    gui->labelLanguage->setText(tr("Language:"));
    gui->labelCodec->setText(tr("Text codec:"));
    gui->labelCaptureTime->setText(tr("Capture time, ms:"));
    gui->labelCaptureBytes->setText(tr("Capture bytes:"));
    gui->spinBoxCaptureTime->setRange(0, 2147483647);
    gui->spinBoxCaptureBytes->setRange(0, 2147483647);
}
void GuiController::setPropertiesSectionConsole (void){
    gui->labelSectionConsole->setText(tr("Console"));
    gui->labelSectionConsole->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelConsoleEcho->setText(tr("Echo:"));
    gui->checkboxConsoleEcho->setText("");

    gui->labelConsoleCyclic->setText(tr("Cyclic mode:"));
    gui->labelConsoleCyclicInterval->setText(tr("Interval, ms:"));
    gui->checkboxConsoleCyclic->setText("");
    gui->labelConsoleHotKey1->setText("1:");
    gui->labelConsoleHotKey2->setText("2:");
    gui->labelConsoleHotKey3->setText("3:");
    gui->labelConsoleHotKey4->setText("4:");
    gui->buttonConsoleHotKey1->setText(" ");
    gui->buttonConsoleHotKey2->setText(" ");
    gui->buttonConsoleHotKey3->setText(" ");
    gui->buttonConsoleHotKey4->setText(" ");
    gui->spinboxConsoleCyclicInterval->setRange(5, 10000);
}
void GuiController::setPropertiesSectionTable (void){
    gui->labelSectionTable->setText(tr("Table"));
    gui->labelSectionTable->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelTableEcho->setText(tr("Echo:"));
    gui->checkboxTableEcho->setText("");

    gui->labelTableCyclicMode->setText(tr("Cyclic mode:"));
    gui->checkboxTableCyclic->setText("");
    gui->labelTableCyclicInterval->setText(tr("Interval:"));
    gui->labelTableHotKey1->setText(tr("1:"));
    gui->labelTableHotKey2->setText(tr("2:"));
    gui->labelTableHotKey3->setText(tr("3:"));
    gui->labelTableHotKey4->setText(tr("4:"));

    gui->spinboxTableCyclicInterval->setRange(5, 10000);

    gui->lineEditTableHotKey1->setValidator(hexValidator);
    gui->lineEditTableHotKey2->setValidator(hexValidator);
    gui->lineEditTableHotKey3->setValidator(hexValidator);
    gui->lineEditTableHotKey4->setValidator(hexValidator);
}
void GuiController::setPropertiesSectionLogs (void){
    gui->labelSectionLog->setText(tr("Logs"));
    gui->labelSectionLog->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelLogEnable->setText(tr("Enable:"));
    gui->labelLogPath->setText(tr("Path:"));
}
void GuiController::setLanguageList(QStringList* list){
    gui->comboBoxLanguage->clear();
    gui->comboBoxLanguage->addItems(*list);
}
void GuiController::setCodecList(QStringList* list){
    gui->comboBoxCodec->clear();
    gui->comboBoxCodec->addItems(*list);
}
void GuiController::setThemeList(QStringList* list){
    gui->comboBoxTheme->clear();
    gui->comboBoxTheme->addItems(*list);
}
void GuiController::retranstate(void){
    gui->connectionContentsButton->setText(tr("Connection"));
    gui->generalContentButton->setText(tr("General"));
    gui->consoleContentButton->setText(tr("Console"));
    gui->tableContentButton->setText(tr("Table"));
    gui->logsContentButton->setText(tr("Logs"));
    gui->bindsContentButton->setText(tr("Binds"));
    gui->infoLabel->setText(tr("Version: 0.0.0"));

    gui->labelSectionConnection->setText(tr("Connection"));
    gui->labelPorts->setText(tr("Port:"));
    gui->labelData->setText(tr("Data:"));
    gui->labelStopBits->setText(tr("Stop bits:"));
    gui->labelParity->setText(tr("Parity:"));
    gui->labelBaudrate->setText(tr("Baudrate:"));
    gui->labelFlowControl->setText(tr("Flow control:"));

    gui->labelSectionGeneral->setText(tr("General"));
    gui->labelTheme->setText(tr("Theme:"));
    gui->labelLanguage->setText(tr("Language:"));
    gui->labelCodec->setText(tr("Text codec:"));

    gui->labelSectionConsole->setText(tr("Console"));
    gui->labelConsoleEcho->setText(tr("Echo:"));

    gui->labelSectionTable->setText(tr("Table"));
    gui->labelTableEcho->setText(tr("Echo:"));
}
bool GuiController::eventFilter(QObject *target, QEvent *event){
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
            emit gui->buttonConsoleHotKey1->clicked();
            return true;
        case quickIndexTable:
            emit gui->buttonTableHotKey1->clicked();
            return true;
        }
        break;
    case Qt::Key_2:
        qDebug() << "Key_2";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            emit gui->buttonConsoleHotKey2->clicked();
            return true;
        case quickIndexTable:
            emit gui->buttonTableHotKey2->clicked();
            return true;
        }
        break;
    case Qt::Key_3:
        qDebug() << "Key_3";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            emit gui->buttonConsoleHotKey3->clicked();
            return true;
        case quickIndexTable:
            emit gui->buttonTableHotKey3->clicked();
            return true;
        }
        break;
    case Qt::Key_4:
        qDebug() << "Key_4";
        switch (gui->workspaceWidget->currentIndex()){
        case quickIndexConsole:
            emit gui->buttonConsoleHotKey4->clicked();
            return true;
        case quickIndexTable:
            emit gui->buttonTableHotKey4->clicked();
            return true;
        }
        break;
    }
    return QObject::eventFilter(target, event);
}
void GuiController::consoleHotkeys(void){
    QString data;
    static QToolButton *button = nullptr;
    if(button != static_cast<QToolButton*>(QObject::sender())){
        button = static_cast<QToolButton*>(QObject::sender());
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
        emit consoleCyclicChanged(gui->checkboxConsoleCyclic->checkState() == Qt::CheckState::Checked ? true : false);
        emit consoleCyclicIntervalChanged(gui->spinboxConsoleCyclicInterval->value());
        emit consoleCyclicDataChanged(data);
        emit consoleStartCycle();
    }
    else {
        button = nullptr;
        emit consoleStopCycle();
    }
}
void GuiController::tableHotkeys(void){
    QString data;
    static QToolButton *button = nullptr;
    if(button != static_cast<QToolButton*>(QObject::sender())){
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
        emit tableCyclicChanged(gui->checkboxTableCyclic->checkState() == Qt::CheckState::Checked ? true : false);
        emit tableCyclicIntervalChanged(gui->spinboxTableCyclicInterval->value());
        emit tableCyclicDataChanged(data);
        emit tableStartCycle();
    }
    else {
        button = nullptr;
        emit tableStopCycle();
    }
}
void GuiController::hexDelimiters(const QString& text){
    QLineEdit* lineEdit = static_cast<QLineEdit*>(QObject::sender());
    QString data = text;
    Converter::setDelimitersInHexString(data, 2, ' ');
    lineEdit->setText(data);
}
void GuiController::selectLogPath(void){
    QString path = QFileDialog::getExistingDirectory(nullptr, tr("Select path"), "/logs",
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    gui->lineEditLogPath->setText(path);
}
