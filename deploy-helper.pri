# Current system time
win32 {
    BUILD_TIME = $$system("echo %time:~0,8%")
} else {
    BUILD_TIME = $$system("time")
}

# Recursive remove directory
defineTest(removeDirRecursive) {
    DIR_TO_DEL = $$shell_quote($$shell_path($$1))
    RETURN = $$escape_expand(\n\t)
    QMAKE_POST_LINK += $$RETURN $$QMAKE_DEL_TREE $$DIR_TO_DEL
    export(QMAKE_POST_LINK)
}
# Example
# removeDirRecursive($$DESTDIR/somedir)



# Remove some files from directory (directory_path and file_names)
defineTest(removeFilesInDir) {
    PATH = $$shell_path($$1) # full path to directory
    FILENAMES = $$2         # filenames inside directory for remove
    RETURN = $$escape_expand(\n\t)
    for(FILENAME, FILENAMES){
        QMAKE_POST_LINK += $$RETURN $$QMAKE_DEL_FILE $$shell_quote($${PATH}$${FILENAME})
    }
    export(QMAKE_POST_LINK)
}
# Example
# FILENAMES = qlib1.dll qlib2.dll qlib3.dll qlib4.dll
# removeFilesInDir($$DESTDIR/somedir/, $$FILENAMES)




# Remove some files
defineTest(removeFiles) {
    FILES_TO_DEL = $$shell_quote($$shell_path($$1)) # full path (split spaces) or mask *
    RETURN = $$escape_expand(\n\t)
    for(FILE, FILES_TO_DEL){
        QMAKE_POST_LINK += $$RETURN $$QMAKE_DEL_FILE $$FILE
    }
    export(QMAKE_POST_LINK)
}
# Example
# FILEPATHES = /path/to/qlib1.dll /path/to/qlib2.dll /path/to/qlib3.dll /path/to/qlib4.dll
# removeFiles($$FILENAMES)


# Create directory if not exist, then copy some files to that directory
defineTest(copyFilesToDir) {
    FILES = $$shell_quote($$shell_path($$1))  # full filepath (split spaces) or masks * ?
    DIR = $$shell_quote($$shell_path($$2))   # directory path
    RETURN = $$escape_expand(\n\t)
    QMAKE_POST_LINK += $$RETURN $$sprintf($$QMAKE_MKDIR_CMD, $$DIR)
    for(FILE,FILES){
        QMAKE_POST_LINK += $$RETURN $$QMAKE_COPY $$FILE $$DIR
    }
    export(QMAKE_POST_LINK)
}
# copyFilesToDir(some/*.dll, $$DESTDIR/other)
# or
# copyFilesToDir(path/to/1.dll path/to/2.dll, $$DESTDIR/other)


# Copy all files from source directory to the destination directory
defineTest(copyDirToDestDir) {
    SOURCE_DIR = $$shell_quote($$shell_path($$1))
    RESULT_DIR = $$shell_quote($$shell_path($$2))
    RETURN = $$escape_expand(\n\t)
    QMAKE_POST_LINK += $$RETURN $$sprintf($$QMAKE_MKDIR_CMD, $$RESULT_DIR)
    for(FILE, SOURCE_DIR) {
        QMAKE_POST_LINK += $$RETURN $$QMAKE_COPY_DIR $$SOURCE_DIR $$RESULT_DIR
    }
    export(QMAKE_POST_LINK)
}

# Copy all dependent dlls into target path
defineTest(collectAllDllDepends) {
    DEPLOY_TARGET = $$shell_quote($$shell_path($$1))
    RETURN = $$escape_expand(\n\t)
    # Select platform collector
    win32 { DEPLOY_COMMAND = windeployqt }
    macx { DEPLOY_COMMAND = macdeployq }
    QMAKE_POST_LINK += $$RETURN $$DEPLOY_COMMAND $$DEPLOY_TARGET
    export(QMAKE_POST_LINK)
}
# Example:
# collectAllDllDepends($$OUT_PWD/Release/target.exe)


# Create offline installer using QInstallerFramework
defineTest(createOfflineInstaller) {
    BINARYCREATOR_PATH = $$shell_quote($$shell_path($$(QTDIR)\..\..\Tools\QtInstallerFramework\3.2\bin\binarycreator))
    RETURN = $$escape_expand(\n\t)
    PACKAGES_PATH = $$shell_quote($$shell_path($$1))
    CONFIG_FILE_PATH = $$shell_quote($$shell_path($$2))
    INSTALLER_PATH = $$shell_quote($$shell_path($$3))
    QMAKE_POST_LINK += $$RETURN $$BINARYCREATOR_PATH --offline-only -c $$CONFIG_FILE_PATH  -p $$PACKAGES_PATH  $$INSTALLER_PATH
    export(QMAKE_POST_LINK)
}
# Example:
#createOfflineInstaller(<packages>, <config.xml>, <installer file name>)
#createOfflineInstaller($$OUT_PWD/packages, $$PWD\config.xml, $$OUT_PWD/offline-installer)


# Create online installer using QInstallerFramework
defineTest(createOnlineInstaller) {
    BINARYCREATOR_PATH = $$shell_quote($$shell_path($$(QTDIR)\..\..\Tools\QtInstallerFramework\3.2\bin\binarycreator))
    RETURN = $$escape_expand(\n\t)
    PACKAGES_PATH = $$shell_quote($$shell_path($$1))
    CONFIG_FILE_PATH = $$shell_quote($$shell_path($$2))
    INSTALLER_PATH = $$shell_quote($$shell_path($$3))
    QMAKE_POST_LINK += $$RETURN $$BINARYCREATOR_PATH --online-only -c $$CONFIG_FILE_PATH -p $$PACKAGES_PATH  $$INSTALLER_PATH
    export(QMAKE_POST_LINK)
}
# Example:
#createOnlineInstaller(<packages>, <config.xml>, <installer file name>)
#createOnlineInstaller($$OUT_PWD/packages, $$PWD\config.xml, $$OUT_PWD/online-installer)


defineTest(createRepo){
    REPOGEN_PATH = $$shell_quote($$shell_path($$(QTDIR)\..\..\Tools\QtInstallerFramework\3.2\bin\repogen))
    RETURN = $$escape_expand(\n\t)
    PACKAGES_PATH = $$shell_quote($$shell_path($$1))
    PACKAGES_NAME = $$2
    REPOSITORY_PATH = $$shell_quote($$shell_path($$3))

    QMAKE_POST_LINK += $$RETURN $$REPOGEN_PATH -p $$PACKAGES_PATH -i $$PACKAGES_NAME --update $$REPOSITORY_PATH
    export(QMAKE_POST_LINK)
}
