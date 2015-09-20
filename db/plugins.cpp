#include "plugins.h"

Plugins::Plugins()
{

}

QList<Plugin> Plugins::getPlugins()
{
    Database database = Database();
    database.open();
    QSqlQuery query(database.db());

    QString sql = "SELECT * FROM plugins";

    if(!query.exec(sql)) {
        QString errorMessage = QString(QObject::tr("Cannot get plugins list") + "\n" + query.lastError().text());
        database.close();
        throw PluginsException(errorMessage);
    }

    QList<Plugin> plugins;

    while(query.next()) {
        Plugin plugin;
        QSqlRecord record = query.record();

        QString pluginName = record.value("name").toString();
        int i_enabled = record.value("enabled").toInt();
        bool enabled = i_enabled == 1 ? true : false;

        plugin.pluginName = pluginName;
        plugin.enabled = enabled;

        plugins.append(plugin);
    }

    return plugins;
}

bool Plugins::pluginExists(QString name)
{
    Database database = Database();
    database.open();
    QSqlQuery query(database.db());

    QString sql = "SELECT COUNT(*) FROM plugins WHERE name = :name";

    if(!query.prepare(sql)) {
        QString errorMessage = QString(QObject::tr("Cannot get plugins list") + "\n" + query.lastError().text());
        database.close();
        throw PluginsException(errorMessage);
    }

    query.bindValue(":name", name);
    query.exec();

    int s = -1;
    while(query.next()) {
        s = query.value(0).toInt();
    }
    return s;
}

Plugin Plugins::getPluginByName(QString name)
{
    Database database = Database();
    database.open();
    QSqlQuery query(database.db());

    QString sql = "SELECT * FROM plugins WHERE name = :name";

    if(!query.prepare(sql)) {
        QString errorMessage = QString(QObject::tr("Cannot get plugins list") + "\n" + query.lastError().text());
        database.close();
        throw PluginsException(errorMessage);
    }

    query.bindValue(":name", name);
    query.exec();

    Plugin plugin;

    //Should iterate once
    while(query.next()) {
        QSqlRecord record = query.record();
        QString pluginName = record.value("name").toString();
        int i_enabled = record.value("enabled").toInt();
        bool enabled = i_enabled == 1 ? true : false;

        plugin.pluginName = pluginName;
        plugin.enabled = enabled;
    }

    database.close();
    return plugin;
}

void Plugins::setPluginEnabled(QString name, bool status)
{
    int i_enabled = status ? 1 : 0;

    if(pluginExists(name)) {
        Database db = Database();
        db.open();
        QSqlQuery query(db.db());

        QString sql = "UPDATE plugins SET enabled = :enabled WHERE name = :name";

        if(!query.prepare(sql)) {
            QString errorMessage = QString(QObject::tr("Cannot update plugins list") + "\n" + query.lastError().text());
            db.close();
            throw PluginsException(errorMessage);
        }

        query.bindValue(":name", name);
        query.bindValue(":enabled", i_enabled);
        query.exec();

        db.close();
    } else {
        Database db = Database();
        db.open();
        QSqlQuery query(db.db());

        QString sql = "INSERT INTO plugins VALUES(:name, :enabled)";

        if(!query.prepare(sql)) {
            QString errorMessage = QString(QObject::tr("Cannot insert plugins list") + "\n" + query.lastError().text());
            db.close();
            throw PluginsException(errorMessage);
        }

        query.bindValue(":name", name);
        query.bindValue(":enabled", i_enabled);
        query.exec();

        db.close();
    }
}

