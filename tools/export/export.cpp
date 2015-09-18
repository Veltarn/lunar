#include "export.h"

Export::Export(QObject *parent) :
    QObject(parent)
{
    m_database = Database();
}

bool Export::startExport(QString path, bool preserveApiLink)
{
    QDomDocument lunarExport("lunarExport");
    QDomProcessingInstruction pri = lunarExport.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8' standalone='no'");
    lunarExport.appendChild(pri);
    QDomElement rootElmt = lunarExport.createElement("lunar");

    lunarExport.appendChild(rootElmt);

    m_database.open();
    QSqlQuery query(m_database.db());
    QString sql = "SELECT * FROM games ORDER BY id";

    if(!query.exec(sql))
    {
        QMessageBox::warning(0, tr("Warning"), QString(tr("Unable to export the database") + "<br />" + query.lastError().text()));
        m_database.close();
        return false;
    }

    while(query.next())
    {
        QDomElement game = lunarExport.createElement("game");
        rootElmt.appendChild(game);
        QSqlRecord record = query.record();

        int id = record.value("id").toInt();
        QString title = record.value("name").toString();
        QString exePath = record.value("exepath").toString();
        QString savePath = record.value("savepath").toString();
        QString uninstallPath = record.value("uninstallpath").toString();
        bool allowSaveSync = record.value("allowsavesync").toBool();
        int playedTime = record.value("playedtime").toInt();
        int lastPlayed = record.value("lasttimeplayed").toInt();

        //Building xml
        QDomElement idElmt = lunarExport.createElement("ID");
        QDomText idText = lunarExport.createTextNode("idText");
        idText.setData(QString::number(id));
        idElmt.appendChild(idText);
        game.appendChild(idElmt);

        QDomElement nameElmt = lunarExport.createElement("Title");
        QDomText nameText = lunarExport.createTextNode("nameText");
        nameText.setData(title);
        nameElmt.appendChild(nameText);
        game.appendChild(nameElmt);

        QDomElement exePathElement = lunarExport.createElement("ExePath");
        QDomText exePathText = lunarExport.createTextNode("exePathText");
        exePathText.setData(exePath);
        exePathElement.appendChild(exePathText);
        game.appendChild(exePathElement);

        if(!uninstallPath.isEmpty())
        {
            QDomElement uninstallPathElmt = lunarExport.createElement("UninstallPath");
            QDomText uninstallPathText = lunarExport.createTextNode("uninstallPathText");
            uninstallPathText.setData(uninstallPath);
            uninstallPathElmt.appendChild(uninstallPathText);
            game.appendChild(uninstallPathElmt);
        }

        if(!savePath.isEmpty())
        {
            QDomElement savePathElmt = lunarExport.createElement("SavePath");
            QDomText savePathText = lunarExport.createTextNode("savePathText");
            savePathText.setData(savePath);
            savePathElmt.appendChild(savePathText);
            game.appendChild(savePathElmt);
        }

        QDomElement allowSaveElmt = lunarExport.createElement("AllowSaveSync");
        QDomText allowSaveText = lunarExport.createTextNode("allowSaveSyncText");
        allowSaveText.setData(QVariant(allowSaveSync).toString());
        allowSaveElmt.appendChild(allowSaveText);
        game.appendChild(allowSaveElmt);

        QDomElement playedTimeElmt = lunarExport.createElement("PlayedTime");
        QDomText playedTimeText = lunarExport.createTextNode("playedTimeText");
        playedTimeText.setData(QString::number(playedTime));
        playedTimeElmt.appendChild(playedTimeText);
        game.appendChild(playedTimeElmt);

        QDomElement lastPlayedElmt = lunarExport.createElement("LastPlayed");
        QDomText lastPlayedText = lunarExport.createTextNode("lastPlayedText");
        lastPlayedText.setData(QString::number(lastPlayed));
        lastPlayedElmt.appendChild(lastPlayedText);
        game.appendChild(lastPlayedElmt);


        if(preserveApiLink)
        {
            //Getting all apis link
            QString apiSql = "SELECT * FROM api_game_association WHERE local_game_id = :gameId ORDER BY local_game_id";
            QSqlQuery apiQuery(m_database.db());

            if(!apiQuery.prepare(apiSql))
            {
                QMessageBox::warning(0, tr("Warning"), QString(tr("Unable to export the database") + "<br />" + query.lastError().text()));
                m_database.close();
                return false;
            }

            apiQuery.bindValue(":gameId", id);

            if(!apiQuery.exec())
            {
                QMessageBox::warning(0, tr("Warning"), QString(tr("Unable to export the database") + "<br />" + query.lastError().text()));
                m_database.close();
                return false;
            }


            QDomElement linkedApisGames = lunarExport.createElement("LinkedApiGames");
            game.appendChild(linkedApisGames);
            while(apiQuery.next())
            {
                QDomElement apiGame = lunarExport.createElement("ApiGame");
                linkedApisGames.appendChild(apiGame);

                QSqlRecord apiRecord = apiQuery.record();

                QString apiName = apiRecord.value("api_name").toString();
                int localGameId = apiRecord.value("local_game_id").toInt();
                int remoteGameId = apiRecord.value("remote_game_id").toInt();

                QDomElement apiNameElmt = lunarExport.createElement("ApiName");
                QDomText apiNameText = lunarExport.createTextNode("apiNameText");
                apiNameText.setData(apiName);
                apiNameElmt.appendChild(apiNameText);
                apiGame.appendChild(apiNameElmt);

                QDomElement localGameIdElmt = lunarExport.createElement("LocalGameId");
                QDomText localGameIdText = lunarExport.createTextNode("localGameIdText");
                localGameIdText.setData(QString::number(localGameId));
                localGameIdElmt.appendChild(localGameIdText);
                apiGame.appendChild(localGameIdElmt);

                QDomElement remoteGameIdElmt = lunarExport.createElement("RemoteGameId");
                QDomText remoteGameIdText = lunarExport.createTextNode("remoteGameIdText");
                remoteGameIdText.setData(QString::number(remoteGameId));
                remoteGameIdElmt.appendChild(remoteGameIdText);
                apiGame.appendChild(remoteGameIdElmt);
            }
        }
    }

    QFile file(path);

    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, tr("Warning"), "Unable to open the XML file");
        file.close();
        m_database.close();
        return false;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << lunarExport.toString();

    if(file.flush())
    {
        QMessageBox::information(0, tr("Success"), tr("The exportation is successful"));
        file.close();
        m_database.close();
        return true;
    }
    else
    {
        QMessageBox::warning(0, tr("Warning"), tr("An error has occured during exportation"));
        file.close();
        m_database.close();
        return false;
    }


}
