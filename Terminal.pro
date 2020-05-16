include("deploy-helper.pri")

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Terminal
#CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    consolemode/consolewidget.cpp \
    converter.cpp \
    decorator.cpp \
    framelesswindow.cpp \
    hexmode/delegate.cpp \
    hexmode/tableconsole.cpp \
    logger.cpp \
    main.cpp \
    mainwidget.cpp \
    serial.cpp \
    serialgui.cpp \
    uiproxy.cpp

HEADERS += \
    consolemode/consolewidget.h \
    converter.h \
    decorator.h \
    framelesswindow.h \
    hexmode/delegate.h \
    hexmode/tableconsole.h \
    logger.h \
    mainwidget.h \
    serial.h \
    serialgui.h \
    uiproxy.h

FORMS += \
    mainwidget.ui

TRANSLATIONS += \
    resources/language/language_ru.ts \
    resources/language/language_en.ts \

RESOURCES += \
    resources/icons.qrc \
    resources/languages.qrc

DISTFILES += \
    config.xml \
    package.xml

#####################################################
# Выбираем директорию сборки исполняемого файла
# в зависимости от режима сборки проекта
#####################################################
CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/Debug
} else {
    DESTDIR = $$OUT_PWD/Release

}
# разделяем по директориям все выходные файлы проекта
MOC_DIR = $$OUT_PWD/common/moc
RCC_DIR = $$OUT_PWD/common/rcc
UI_DIR = $$OUT_PWD/common/ui
unix:OBJECTS_DIR = $$OUT_PWD/common/obj/unix
win32:OBJECTS_DIR = $$OUT_PWD/common/obj/win32
macx:OBJECTS_DIR = $$OUT_PWD/common/obj/mac

QMAKE_CLEAN += $$shell_quote($$shell_path($$DESTDIR/*.exe))
QMAKE_CLEAN += $$shell_quote($$shell_path($$DESTDIR/*.dll))


#########################################################################
# Собираем все зависимости к в папку сборки
#########################################################################
isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}
# в зависимости от режима сборки проекта
# запускаем win deploy приложения в целевой директории, то есть собираем все dll
CONFIG(debug, debug|release) {
    DEPLOY_TARGET = $$OUT_PWD/Debug/$$TARGET$$TARGET_CUSTOM_EXT
} else {
    DEPLOY_TARGET = $$OUT_PWD/Release/$$TARGET$$TARGET_CUSTOM_EXT
}
collectAllDllDepends($$DEPLOY_TARGET)


##########################################################################
# Далее настройки для сборки инсталяторов с помощью QInstallerFramework
#########################################################################
CONFIG(release, release|debug) { # Собираем инсталяторы только для релизной сборки

    PACKAGE_FILE_XML = $$OUT_PWD/../Terminal/package.xml    # Указываем путь к package.xml
    PACKAGE_DATA = $$OUT_PWD/Release                        # Задаём путь к собранному приложению со всеми зависимостями

    ####################################################################
    # Далее создадим временную директорию для установочного пакета
    # package/
    # ........meta/    - сюда помещаем package.xml
    # ........data/    - сюда данные все приложения
    ####################################################################

    # Задаём пути пакета, которая должна содержать путь к папке с данными
    # Копируем все необходимо с формируем установочный пакет
    PACKAGE_DATA_PATH = $$OUT_PWD/../packages/advancedterminal/data
    PACKAGE_META_PATH = $$OUT_PWD/../packages/advancedterminal/meta
    copyDirToDestDir($$PACKAGE_DATA, $$PACKAGE_DATA_PATH)
    copyFilesToDir($$PACKAGE_FILE_XML, $$PACKAGE_META_PATH)

    # Собираем инсталяторы
    PACKAGES_PATH = $$OUT_PWD/../packages                                       # Путь к установочному пакету
    CONFIG_FILE_PATH = $$PWD\config.xml                                         # Путь к конфирурации
    ONLINE_INSTALLER_PATH = $$OUT_PWD/Release/installers/online-installer       # Путь размещения онлайн инсталятора
    OFFLINE_INSTALLER_PATH = $$OUT_PWD/Release/installers/offline-installer     # Путь размещения оффлайн инсталятора
    mkpath($$ONLINE_INSTALLER_PATH/../)                                         # Если пути не существуют - создаём их
    mkpath($$OFFLINE_INSTALLER_PATH/../)                                        #
    createOnlineInstaller($$PACKAGES_PATH, $$CONFIG_FILE_PATH, $$ONLINE_INSTALLER_PATH)
    createOfflineInstaller($$PACKAGES_PATH, $$CONFIG_FILE_PATH, $$OFFLINE_INSTALLER_PATH)

    # Удаляем временную папку установочного пакета
    removeDirRecursive($$PACKAGES_PATH)

    # Создаём дополнитеьную цель очистки при выполнении make clean
    QMAKE_CLEAN += $$shell_quote($$shell_path($$ONLINE_INSTALLER_PATH/../*.exe))
    QMAKE_CLEAN += $$shell_quote($$shell_path($$OFFLINE_INSTALLER_PATH/../*.exe))
}


