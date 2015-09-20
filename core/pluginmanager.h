#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QFileInfo>
#include <QMap>
#include <QDir>
#include <QPluginLoader>
#include "db/plugins.h"
#include "tools/singleton.h"

class PluginManager : public Singleton<PluginManager>
{
    Q_OBJECT
public:
     virtual void onFirstInit();
     void loadPlugin(int index);
     QList<QPluginLoader *> pluginsList();
     QPluginLoader* getPluginByName(QString name);
     static void setPluginEnabled(QString name, bool status);
     static bool isPluginEnabled(QString pluginName);
protected:
     virtual void onDestroyed();
private:
    void discoverPlugins();

private:
    QString m_pluginsPath;
    QList<QPluginLoader *> m_pluginsList;
};

#endif // PLUGINMANAGER_H
