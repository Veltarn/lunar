#include "pluginmanager.h"

void PluginManager::onFirstInit()
{
#ifdef QT_DEBUG
    m_pluginsPath = "plugins/";
#else
    m_pluginsPath = qApp->applicationDirPath() + "/plugins/";
#endif
    this->discoverPlugins();
}

void PluginManager::loadPlugin(int index)
{
    /*if(index >= 0 && index < m_pluginsList.size()) {
        QFileInfo file(m_pluginsList[index]->fileName());
        QPluginLoader *qtPlugin = m_pluginsList[index];
        if(!qtPlugin->load()) {
            qDebug() << "Cannot load plugin " << file.fileName() << endl << m_pluginsList[index]->errorString() << endl;
        } else {
            PandorePlugin *plugin = qobject_cast<PandorePlugin*>(qtPlugin->instance());
            plugin->onLoad();
        }
    }*/
}

QList<QPluginLoader *> PluginManager::pluginsList()
{
    return m_pluginsList;
}

QPluginLoader *PluginManager::getPluginByName(QString name)
{
    foreach(QPluginLoader *plugin, m_pluginsList) {
        QJsonObject metadata = plugin->metaData().value("MetaData").toObject();
        QString pluginName = metadata.value("name").toString();

        if(name == pluginName) {
            return plugin;
        }
    }
    return NULL;
}

void PluginManager::setPluginEnabled(QString name, bool status)
{
    Plugins::setPluginEnabled(name, status);
}

bool PluginManager::isPluginEnabled(QString pluginName)
{
    Plugin p = Plugins::getPluginByName(pluginName);
    if(!p.pluginName.isEmpty()) {
        return p.enabled;
    }

    return false;
}



void PluginManager::discoverPlugins()
{
    QDir pluginsDir(m_pluginsPath);
    QFileInfoList pluginsFile = pluginsDir.entryInfoList(QStringList("*.dll"), QDir::NoDotAndDotDot | QDir::Files);

    foreach(QFileInfo file, pluginsFile) {
        QPluginLoader *plugin = new QPluginLoader(file.absoluteFilePath());
        m_pluginsList.append(plugin);
    }
}

void PluginManager::onDestroyed() {
    foreach(QPluginLoader *qtPlugin, m_pluginsList) {
        /*PandorePlugin *plugin = qobject_cast<PandorePlugin*>(qtPlugin->instance());
        plugin->onRelease();*/
    }

    int i = 0;
    foreach(QPluginLoader *qtPlugin, m_pluginsList) {
        if(qtPlugin->isLoaded())
            i++;
    }

    if(i > 0) {
        qCritical() << i << " plugin(s) are still loaded!" << endl;
    }
}
