#include "settingscontroller.h"
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>


SettingsController::SettingsController(QObject *parent, Ui::MainWidget* gui) : QObject(parent)
{
    SettingsController::gui = gui;
    setProperties();

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
            this, &SettingsController::currentThemeChanged);
    connect(gui->comboBoxCodec, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsController::currentTextCodecChanged);
    connect(gui->comboBoxLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsController::currentLanguageChanged);
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
SettingsController::~SettingsController(){

}
void SettingsController::setProperties (void){
    setPropertiesMenu();
    setPropertiesSectionConnection();
    setPropertiesSectionGeneral();
    setPropertiesSectionConsole();
    setPropertiesSectionTable();
    setPropertiesSectionLogs();
}
void SettingsController::setPropertiesMenu(void){
    gui->connectionContentsButton->setText("Connection");
    gui->generalContentButton->setText("General");
    gui->consoleContentButton->setText("Console");
    gui->tableContentButton->setText("Table");
    gui->logsContentButton->setText("Logs");
    gui->bindsContentButton->setText("Binds");
    gui->infoLabel->setText("Version: 0.0.0");
    gui->settingsPage->layout()->setMargin(0);
    gui->pageDelimiterLayout->setMargin(0);
    gui->leftPanelContents->layout()->setMargin(0);
    gui->rightPanelContents->layout()->setMargin(0);
    gui->rightStackedPanel->layout()->setMargin(0);
    gui->pageDelimiterLayout->setSpacing(0);
}
void SettingsController::setPropertiesSectionConnection (void){
    gui->labelSectionConnection->setText("Connection");
    gui->labelSectionConnection->setFont(QFont("Arial", 14, QFont::Bold));
}
void SettingsController::setPropertiesSectionGeneral (void){
    gui->labelSectionGeneral->setText("General");
    gui->labelSectionGeneral->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelTheme->setText("Theme:");
    gui->labelLanguage->setText("Language:");
    gui->labelCodec->setText("Text codec:");
}
void SettingsController::setPropertiesSectionConsole (void){
    gui->labelSectionConsole->setText("Console");
    gui->labelSectionConsole->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelConsoleEcho->setText("Echo:");
    gui->checkboxConsoleEcho->setText("");
}
void SettingsController::setPropertiesSectionTable (void){
    gui->labelSectionTable->setText("Table");
    gui->labelSectionTable->setFont(QFont("Terminus", 14, QFont::Bold));
    gui->labelTableEcho->setText("Echo:");
    gui->checkboxTableEcho->setText("");
}
void SettingsController::setPropertiesSectionLogs (void){

}
void SettingsController::setLanguageList(QStringList* list){
    gui->comboBoxLanguage->clear();
    gui->comboBoxLanguage->addItems(*list);
}
void SettingsController::setCodecList(QStringList* list){
    gui->comboBoxCodec->clear();
    gui->comboBoxCodec->addItems(*list);
}
void SettingsController::setThemeList(QStringList* list){
    gui->comboBoxTheme->clear();
    gui->comboBoxTheme->addItems(*list);
}

