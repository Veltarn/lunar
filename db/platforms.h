#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QList>
#include "database.h"
#include "Exceptions/platformsexception.h"

struct Platform {
    int platform_id;
    QString platform_name;
};

class Platforms
{
public:
    Platforms();
    static QList<Platform> getPlatformsList();
    static Platform getPlatformByName(QString name);
    static Platform getPlatformById(int id);
    static QList<Platform> getPlatformsByEmulatorId(int emulatorId);
};

#endif // PLATFORMS_H
