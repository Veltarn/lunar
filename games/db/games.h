#ifndef GAMES_H
#define GAMES_H

#include <iostream>
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QTime>
#include "../tools/savemanager.h"
#include "db/database.h"

class Games
{
public:
    Games();
    ~Games();

    int add(QString gameName, QString gamePath, QString uninstallPath, QString savePath, bool allowSaveSync, int emulatorUsed = -1, QString commandArguments = QString());
    bool addApiGameAssociation(QString apiName, int remoteGameId, int localGameId = 0);
    bool update(int gameId, QString gameName, QString gamePath, QString uninstallPath, QString savePath, bool allowSaveSync, int emulatorUsed = -1, QString commandArguments = QString());
    bool updateApiGameAssociation(QString apiName, int localGameId, int newRemoteGameId, int oldRemoteGameId);
    bool deleteGame(int gameId);
    bool deleteApiGameAssociation(QString apiName, int localGameId, int remoteGameId);

    void updatePlayTime(int gameId, int playDuration);

    QSqlQueryModel *findAll(); // Get every games

    QSqlRecord find(int game_id, QString apiName = "basic");

    void syncSaves(int gameId);
private:

    Database m_database;
    SaveManager *m_saveManager;
    int m_lastInsertGameId;
};

#endif // GAMES_H
