#include "games.h"

using namespace std;

Games::Games()
{
    m_database = Database();
    m_saveManager = SaveManager::getInstance();
}

Games::~Games()
{}

int Games::add(QString gameName, QString gamePath, QString uninstallPath, QString savePath, bool allowSaveSync, int emulatorUsed, QString commandArguments)
{
    m_database.open();
    QSqlQuery query(m_database.db());

    QString sql = "INSERT INTO games (name, exepath, savepath, allowsavesync, uninstallpath, emulator_used, command_arguments) VALUES(:name, :exepath, :savepath, :allowsavesync, :uninstallpath, :emulator_used, :command_arguments)";

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return 0;
    }

    query.bindValue(":name", gameName);
    query.bindValue(":exepath", gamePath);
    query.bindValue(":savepath", savePath);
    query.bindValue(":allowsavesync", allowSaveSync);
    query.bindValue(":uninstallpath", uninstallPath);
    query.bindValue(":emulator_used", emulatorUsed);
    query.bindValue(":command_arguments", commandArguments);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return 0;
    }

    m_lastInsertGameId = query.lastInsertId().toInt();

    m_database.close();
    return m_lastInsertGameId;
}

bool Games::addApiGameAssociation(QString apiName,  int remoteGameId, int localGameId)
{
    m_database.open();

    QSqlQuery query(m_database.db());

    QString sql("INSERT INTO api_game_association VALUES(:apiName, :localGameId, :remoteGameId)");

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    query.bindValue(":apiName", apiName);
    if(localGameId == 0)
        query.bindValue(":localGameId", m_lastInsertGameId);
    else
        query.bindValue(":localGameId", localGameId);

    query.bindValue(":remoteGameId", remoteGameId);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request l.71<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    m_database.close();
    return true;
}

bool Games::updateApiGameAssociation(QString apiName, int localGameId, int newRemoteGameId, int oldRemoteGameId)
{
    m_database.open();

    QSqlQuery query(m_database.db());
    QString sql("SELECT COUNT(local_game_id) AS numRows FROM api_game_association WHERE api_name = :apiName AND local_game_id = :localGameId AND remote_game_id = :remoteGameId");


    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    query.bindValue(":apiName", apiName);
    query.bindValue(":localGameId", localGameId);
    query.bindValue(":remoteGameId", oldRemoteGameId);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    int numRowsAffected = 0;
    if(query.next())
    {
        QSqlRecord num = query.record();
        numRowsAffected = num.value("numRows").toInt();
    }

    if(numRowsAffected == 1)
    {

        sql = "UPDATE api_game_association SET remote_game_id = :remoteGameId WHERE local_game_id = :localGameId AND api_name = :apiName";

        if(!query.prepare(sql))
        {
            QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
            m_database.close();
            return false;
        }

        query.bindValue(":apiName", apiName);
        query.bindValue(":localGameId", localGameId);
        query.bindValue(":remoteGameId", newRemoteGameId);

        if(!query.exec())
        {
            QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request l.131<br />") + query.lastError().text()));
            m_database.close();
            return false;
        }
        m_database.close();
    }
    else if(numRowsAffected == 0) //If there is no record
    {
        m_database.close();
        this->addApiGameAssociation(apiName, newRemoteGameId, localGameId);
    }
    else
    {
        QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("Several records matches, unable to continue"));

        m_database.close();
        return false;
    }

    return true;
}

bool Games::update(int gameId, QString gameName, QString gamePath, QString uninstallPath, QString savePath, bool allowSaveSync, int emulatorUsed, QString commandArguments)
{
    m_database.open();

    QSqlQuery query(m_database.db());

    QString sql = "UPDATE games SET name=:gameName, exepath=:gamePath, uninstallpath=:uninstallpath, savepath=:savePath, allowsavesync=:allowSaveSync, emulator_used=:emulatorUsed, command_arguments=:commandArguments WHERE id=:gameId";

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    query.bindValue(":gameId", gameId);
    query.bindValue(":gameName", gameName);
    query.bindValue(":gamePath", gamePath);
    query.bindValue(":savePath", savePath);
    query.bindValue(":allowSaveSync", allowSaveSync);
    query.bindValue(":uninstallpath", uninstallPath);
    query.bindValue(":emulatorUsed", emulatorUsed);
    query.bindValue(":commandArguments", commandArguments);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    m_database.close();
    return true;
}

bool Games::deleteApiGameAssociation(QString apiName, int localGameId, int remoteGameId)
{
    m_database.open();

    QSqlQuery query(m_database.db());

    QString sql("DELETE FROM api_game_association WHERE api_name = :apiName AND local_game_id = :localGameId AND remote_game_id = :remoteGameId");

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    query.bindValue(":apiName", apiName);
    query.bindValue(":localGameId", localGameId);
    query.bindValue(":remoteGameId", remoteGameId);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    m_database.close();
    return true;
}

bool Games::deleteGame(int gameId)
{
    m_database.open();

    QSqlQuery query(m_database.db());
    QString sql = "DELETE FROM games WHERE id=:idGame";

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    query.bindValue(":idGame", gameId);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    //Deleting api_game_association
    sql = "DELETE FROM api_game_association WHERE local_game_id = :gameId";

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    query.bindValue(":gameId", gameId);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during the SQL request<br />") + query.lastError().text()));
        m_database.close();
        return false;
    }

    m_database.close();
    return true;
}

QSqlQueryModel *Games::findAll()
{
    m_database.open();
    QSqlQueryModel *model = new QSqlQueryModel;

    //Changing column order in order to correctly display game names in the list view without removing any columns
    QString sql = "SELECT name, id, exepath, savepath, allowsavesync, uninstallpath, playedtime, lasttimeplayed, emulator_used, command_arguments FROM games ORDER BY name";
    model->setQuery(sql, m_database.db());

    if(model->lastError().isValid())
    {
        QMessageBox::critical(0, QObject::tr("Error"), QString(QObject::tr("An error has occured during the games request") + "<br />" + model->lastError().text()));
        m_database.close();
        return new QSqlQueryModel();
    }
    m_database.close();
    return model;
}

QSqlRecord Games::find(int game_id, QString apiName)
{
    m_database.open();

    QSqlQuery query(m_database.db());
    QString sql("");

    if(apiName == "basic")
        sql = "SELECT * FROM games WHERE id=:gameId";
    else
    {
        sql = "SELECT games.id, games.name, games.exepath, games.uninstallpath, games.savepath, games.allowsavesync, games.playedtime, games.lasttimeplayed, games.emulator_used, games.command_arguments, aga.api_name, aga.local_game_id, aga.remote_game_id \
                FROM games \
                LEFT JOIN api_game_association AS aga ON games.id = aga.local_game_id \
                AND aga.api_name = :apiName \
                 WHERE id = :gameId";
    }

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during a SQL query at line: ") + QString::number(__LINE__) + "<br />" + query.lastError().text()));
        m_database.close();
        return QSqlRecord();
    }

    query.bindValue(":gameId", game_id);

    if(apiName != "basic")
    {
        query.bindValue(":apiName", apiName);
    }

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during a SQL query at line: ") + QString::number(__LINE__) + "<br />" + query.lastError().text()));
        m_database.close();
        return QSqlRecord();
    }

    //Getting the only record selected by sqlite
    QSqlRecord rec;

    if(query.next())
    {
        rec = query.record();
    }

    m_database.close();
    return rec;
}

void Games::updatePlayTime(int gameId, int playDuration)
{
    //Getting game in order to add the current play time to the new
    QSqlRecord game = this->find(gameId);

    int currentPlayedTime = game.value("playedtime").toInt();

    playDuration = currentPlayedTime + playDuration;

    m_database.open();

    QSqlQuery query(m_database.db());
    QString sql("UPDATE games SET playedtime=:playedTime, lasttimeplayed=:lastTime WHERE id = :gameId");

    if(!query.prepare(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_P"), QString(QObject::tr("An error has occured during a SQL query at line: ") + QString::number(__LINE__) + "<br />" + query.lastError().text()));
        m_database.close();
        return;
    }

    int lastTime = QTime::currentTime().elapsed();
    query.bindValue(":playedTime", playDuration);
    query.bindValue(":lastTime", lastTime);
    query.bindValue(":gameId", gameId);

    if(!query.exec())
    {
        QMessageBox::critical(0, QObject::tr("Error #SQL_EX"), QString(QObject::tr("An error has occured during a SQL query at line: ") + QString::number(__LINE__) + "<br />" + query.lastError().text()));
        m_database.close();
        return;
    }

    m_database.close();
}

void Games::syncSaves(int gameId)
{
    QSqlRecord tmp = this->find(gameId);

    //No test to do, we know that the game exists

    if(tmp.value("allowsavesync").toBool())
    {
        QString savePath = tmp.value("savepath").toString();

        m_saveManager->compressSaveFolder(tmp.value("name").toString(), savePath);
    }
}
