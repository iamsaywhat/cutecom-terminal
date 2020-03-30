#include "guicontroller.h"
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
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
    connect(gui->checkboxConsoleCyclic, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->consoleCyclicChanged(true);
                            else
                                emit this->consoleCyclicChanged(false);});
    connect(gui->spinboxConsoleCyclicInterval, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiController::consoleCyclicIntervalChanged);
    // Вкладка Table
    connect(gui->checkboxTableEcho, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->tableEchoChanged(true);
                            else
                                emit this->tableEchoChanged(false);});
    connect(gui->checkboxTableCyclic, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->tableCyclicChanged(true);
                            else
                                emit this->tableCyclicChanged(false);});
    connect(gui->spinboxTableCyclicInterval, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiController::tableCyclicIntervalChanged);

    connect(gui->spinBoxCaptureTime, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiController::captureTimeChanges);
    connect(gui->spinBoxCaptureBytes, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiController::captureBytesChanges);

    gui->checkboxConsoleEcho->setCheckState(Qt::CheckState::Checked);
    gui->checkboxTableEcho->setCheckState(Qt::CheckState::Checked);
}
GuiController::~GuiController(){

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
    gui->buttonConsoleHotkey1->setText(" ");
    gui->buttonConsoleHotkey2->setText(" ");
    gui->buttonConsoleHotkey3->setText(" ");
    gui->buttonConsoleHotkey4->setText(" ");
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
}
void GuiController::setPropertiesSectionLogs (void){

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
            emit gui->buttonConsoleHotkey1->clicked();
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
            emit gui->buttonConsoleHotkey2->clicked();
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
            emit gui->buttonConsoleHotkey3->clicked();
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
            emit gui->buttonConsoleHotkey4->clicked();
            return true;
        case quickIndexTable:
            emit gui->buttonTableHotKey4->clicked();
            return true;
        }
        break;
    }
    return QObject::eventFilter(target, event);
}


