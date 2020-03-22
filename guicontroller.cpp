#include "guicontroller.h"
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>

Ui::MainWidget* GuiController::gui = nullptr;

GuiController::GuiController(QObject *parent, Ui::MainWidget* gui) : QObject(parent)
{
    GuiController::gui = gui;
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
    // Вкладка Connection
    connect(gui->checkboxConsoleEcho, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->consoleEchoChanged(true);
                            else
                                emit this->consoleEchoChanged(false);});
    // Вкладка Table
    connect(gui->checkboxTableEcho, &QCheckBox::stateChanged,
            [=](int state){ if(state == Qt::CheckState::Checked)
                                emit this->tableEchoChanged(true);
                            else
                                emit this->tableEchoChanged(false);});

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
}
void GuiController::setPropertiesSectionConsole (void){
    gui->labelSectionConsole->setText(tr("Console"));
    gui->labelSectionConsole->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelConsoleEcho->setText(tr("Echo:"));
    gui->checkboxConsoleEcho->setText("");
}
void GuiController::setPropertiesSectionTable (void){
    gui->labelSectionTable->setText(tr("Table"));
    gui->labelSectionTable->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelTableEcho->setText(tr("Echo:"));
    gui->checkboxTableEcho->setText("");
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

