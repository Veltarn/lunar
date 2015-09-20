#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T22:44:18
#
#-------------------------------------------------

QT       += core gui sql network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lunar
TEMPLATE = app


SOURCES += main.cpp\
        lunarmainwindow.cpp \
    gui/menubar.cpp \
    gui/menu.cpp \
    gui/listview.cpp \
    gui/aboutlunardialog.cpp \
    gui/button.cpp \
    db/database.cpp \
    games/gui/windowmodifgame.cpp \
    games/db/games.cpp \
    gui/contextualmenu.cpp \
    tools/processmanager.cpp \
    tools/process.cpp \
    gui/windowprocessmanager.cpp \
    gui/windowoptions.cpp \
    gui/gameinfowidget.cpp \
    games/tools/savemanager.cpp \
    tools/libTar/TarOut.cpp \
    tools/libTar/TarIn.cpp \
    tools/libTar/Tar.cpp \
    tools/libTar/StringOps.cpp \
    tools/libTar/FileInfo.cpp \
    tools/libTar/AbstractTar.cpp \
    tools/qtarout.cpp \
    games/apis/apigame.cpp \
    games/apis/abstractapi.cpp \
    games/apis/jvcomapi.cpp \
    games/apis/apis.cpp \
    games/gui/dialogselectgame.cpp \
    tools/slideshow/slideshow.cpp \
    tools/slideshow/picturedownloader.cpp \
    gui/aboutlunar.cpp \
    tools/import/import.cpp \
    tools/export/export.cpp \
    tools/export/windowexport.cpp \
    tools/import/windowimport.cpp \
    tools/cache.cpp \
    tools/stylesheetmanager.cpp \
    gui/scrollbar.cpp \
    gui/windowtitlebar.cpp \
    gui/emulatorswindow.cpp \
    gui/addemulatorwindow.cpp \
    db/platforms.cpp \
    Exceptions/platformsexception.cpp \
    emulation/emulatorsknowledgebase.cpp \
    Exceptions/emulatorknowledgebaseexception.cpp \
    Exceptions/emulatorexception.cpp \
    emulation/emulators.cpp \
    emulation/emulator.cpp \
    emulation/commandlinestringreplacer.cpp \
    gui/qmultiplechoices.cpp \
    gui/qdatacheckbox.cpp \
    core/pluginmanager.cpp \
    gui/pluginswindow.cpp \
    db/plugins.cpp \
    Exceptions/pluginsexception.cpp

HEADERS  += lunarmainwindow.h \
    constants.h \
    gui/menubar.h \
    gui/menu.h \
    gui/listview.h \
    gui/aboutlunardialog.h \
    gui/button.h \
    db/database.h \
    games/gui/windowmodifgame.h \
    games/db/games.h \
    gui/contextualmenu.h \
    tools/processmanager.h \
    tools/singleton.h \
    tools/process.h \
    gui/windowprocessmanager.h \
    gui/windowoptions.h \
    gui/gameinfowidget.h \
    games/tools/savemanager.h \
    tools/libTar/TarOut.h \
    tools/libTar/TarIn.h \
    tools/libTar/Tar.h \
    tools/libTar/StringOps.h \
    tools/libTar/FileInfo.h \
    tools/libTar/AbstractTar.h \
    tools/qtarout.h \
    games/apis/apigame.h \
    games/apis/abstractapi.h \
    games/apis/jvcomapi.h \
    games/apis/apis.h \
    games/gui/dialogselectgame.h \
    tools/slideshow/slideshow.h \
    tools/slideshow/picturedownloader.h \
    gui/aboutlunar.h \
    tools/import/import.h \
    tools/export/export.h \
    tools/export/windowexport.h \
    tools/import/windowimport.h \
    tools/cache.h \
    tools/stylesheetmanager.h \
    gui/scrollbar.h \
    gui/windowtitlebar.h \
    gui/emulatorswindow.h \
    gui/addemulatorwindow.h \
    db/platforms.h \
    Exceptions/platformsexception.h \
    emulation/emulatorsknowledgebase.h \
    Exceptions/emulatorknowledgebaseexception.h \
    Exceptions/emulatorexception.h \
    emulation/emulators.h \
    emulation/emulator.h \
    emulation/commandlinestringreplacer.h \
    gui/qmultiplechoices.h \
    gui/qdatacheckbox.h \
    core/pluginmanager.h \
    gui/pluginswindow.h \
    db/plugins.h \
    Exceptions/pluginsexception.h

INCLUDEPATH  += C:/Users/Veltarn/Documents/Prog/Qt/Lunar/Plugins/Pandore/

FORMS    += lunarmainwindow.ui \
    gui/windowprocessmanager.ui \
    gui/windowoptions.ui \
    gui/aboutlunar.ui \
    tools/export/windowexport.ui \
    tools/import/windowimport.ui
TRANSLATIONS = lunar_fr.ts

RC_FILE += \
    ressources.rc

OTHER_FILES += \
    resources/css/HorizontalScrollBar.css \
    resources/css/VerticalScrollBar.css
