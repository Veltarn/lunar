#include "pluginmanager.h"

void PluginManager::onFirstInit()
{
    m_pluginsPath = "plugins/";
    this->discoverPlugins();
}

void PluginManager::loadPlugin(int index)
{
    if(index >= 0 && index < m_pluginsList.size()) {
        QFileInfo file(m_pluginsList[index]->fileName());
        QPluginLoader *qtPlugin = m_pluginsList[index];
        if(!qtPlugin->load()) {
            qDebug() << "Cannot load plugin " << file.fileName() << endl << m_pluginsList[index]->errorString() << endl;
        } else {
            PandorePlugin *plugin = qobject_cast<PandorePlugin*>(qtPlugin->instance());
            plugin->onLoad();
        }
    }
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
        PandorePlugin *plugin = qobject_cast<PandorePlugin*>(qtPlugin->instance());
        plugin->onRelease();
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
