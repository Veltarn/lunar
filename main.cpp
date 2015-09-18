#include "lunarmainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "core/pluginmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    //translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if(!translator.load(a.applicationDirPath() + "/lunar_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        std::cout << "erreur" << " " << a.applicationDirPath().toStdString() << std::endl;
    }

    a.installTranslator(&translator);
    PluginManager *pm = PluginManager::getInstance();
    pm->loadPlugin(0);

    LunarMainWindow w;
    w.show();
    
    return a.exec();
}
