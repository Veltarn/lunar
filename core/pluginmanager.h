#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDebug>
#include <QObject>
#include <QFileInfo>
#include <QMap>
#include <QDir>
#include <QPluginLoader>
#include "tools/singleton.h"
#include "pandoreplugin.h"

class PluginManager : public Singleton<PluginManager>
{
    Q_OBJECT
public:
     virtual void onFirstInit();
     void loadPlugin(int index);
protected:
     virtual void onDestroyed();
private:
    void discoverPlugins();

private:
    QString m_pluginsPath;
    QList<QPluginLoader *> m_pluginsList;
};

#endif // PLUGINMANAGER_H
