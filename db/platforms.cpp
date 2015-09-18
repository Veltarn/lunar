#include "platforms.h"

Platforms::Platforms()
{

}

QList<Platform> Platforms::getPlatformsList()
{
    Database database = Database();
    database.open();
    QSqlQuery query(database.db());
    QList<Platform> platformsList;

    QString sql = "SELECT * FROM platform ORDER BY platform_name ASC";

    if(!query.exec(sql)) {
        QString errorMessage = QString(QObject::tr("Cannot get platforms list") + "\n" + query.lastError().text());
        database.close();
        throw PlatformsException(errorMessage);
    }

    while(query.next()) {
        Platform platform;
        QSqlRecord record = query.record();
        int id = record.value("id").toInt();
        QString name = record.value("platform_name").toString();
        platform.platform_id = id;
        platform.platform_name = name;

        platformsList.append(platform);
    }

    database.close();
    return platformsList;
}

Platform Platforms::getPlatformByName(QString name)
{
    Database database;
    database.open();
    QSqlQuery query(database.db());
    Platform platform;
    platform.platform_id = -1;
    platform.platform_name = QString();

    QString sql = "SELECT * FROM platform WHERE platform_name = :platformName";

    if(!query.prepare(sql)) {
        QString errorMessage = QString(QObject::tr("Cannot get platform data") + "\n" + query.lastError().text());
        database.close();
        throw PlatformsException(errorMessage);
    }

    query.bindValue(":platformName", name);
    query.exec();

    //Assuming there is only one result...
    while(query.next()) {
        QSqlRecord record = query.record();
        int id = record.value("id").toInt();
        QString name = record.value("platform_name").toString();

        platform.platform_id = id;
        platform.platform_name = name;
    }

    database.close();
    return platform;
}

Platform Platforms::getPlatformById(int in_id)
{
    Database database;
    database.open();

    QSqlQuery query(database.db());
    Platform platform;
    platform.platform_id = -1;
    platform.platform_name = QString();

    QString sql = "SELECT * FROM platform WHERE id = :id";

    if(!query.prepare(sql)) {
        QString errorMessage = QString(QObject::tr("Cannot get platform data") + "\n" + query.lastError().text());
        database.close();
        throw PlatformsException(errorMessage);
    }

    query.bindValue(":id", in_id);
    query.exec();

    while(query.next()) {
        QSqlRecord record = query.record();
        int id = record.value("id").toInt();
        QString name = record.value("platform_name").toString();

        platform.platform_id = id;
        platform.platform_name = name;
    }

    database.close();
    return platform;
}

QList<Platform> Platforms::getPlatformsByEmulatorId(int emulatorId)
{
    QList<Platform> platforms;
    Database database;
    database.open();

    QSqlQuery query(database.db());

    QString sql = "SELECT emulators_platforms_junction.emulator_id, emulators_platforms_junction.platform_id, platform.id, platform.platform_name FROM emulators_platforms_junction LEFT JOIN platform ON emulators_platforms_junction.platform_id = platform.id WHERE emulators_platforms_junction.emulator_id = :emulatorId";

    if(!query.prepare(sql)) {
        QString errorMessage = QString(QObject::tr("Cannot get platform data") + "\n" + query.lastError().text());
        database.close();
        throw PlatformsException(errorMessage);
    }

    query.bindValue(":emulatorId", emulatorId);
    query.exec();

    while(query.next()) {
        Platform platform;
        QSqlRecord record = query.record();

        int id = record.value("id").toInt();
        QString name = record.value("platform_name").toString();

        platform.platform_id = id;
        platform.platform_name = name;

        platforms.append(platform);
    }

    return platforms;
}


