#ifndef PLUGINS_H
#define PLUGINS_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QList>
#include "database.h"
#include "Exceptions/pluginsexception.h"

struct Plugin {
    Plugin() {
        this->pluginName = QString();
        this->enabled = false;
    }

    QString pluginName;
    bool enabled;
};

class Plugins
{
public:
    Plugins();
    static bool pluginExists(QString name);
    static Plugin getPluginByName(QString name);
    static void setPluginEnabled(QString name, bool status);
};

#endif // PLUGINS_H
