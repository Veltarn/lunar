#include "import.h"

Import::Import(QObject *parent) : QObject(parent)
{
    m_database = Database();
}

bool Import::startImport(QString file)
{
    QDomDocument lunarImport("LunarImport");

    QFile xmlFile(file);
    m_database.open();

    if(!xmlFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, tr("Warning"), tr("Unable to open the selected file") + "<br />" + file);
        m_database.close();
        return false;
    }

    if(!lunarImport.setContent(&xmlFile))
    {
        QMessageBox::warning(0, tr("Error"), tr("The selected XML file contains some errors, unable to import it"));
        m_database.close();
        return false;
    }

    QDomElement root = lunarImport.firstChildElement();

    QDomElement game = root.firstChildElement();

    //Calculating number of <game> tag
    QDomNodeList gametag = root.elementsByTagName("game");
    int numberGameTag = gametag.size();

    QProgressDialog progress(tr("Import in progress"), "Cancel", 0, numberGameTag);
    int progression = 0;
    while(!game.isNull())
    {
        //Parsing inner game element
        QDomElement gameElmt = game.firstChildElement();

        QString name = "";
        QString exePath = "";
        QString uninstallPath = "";
        bool allowSaveSync = false;
        QString savePath = "";
        int playedTime = 0;
        int lastPlayed = 0;
        QVector<QMap<QString, QVariant> > linkedApiTab;

        while(!gameElmt.isNull())
        {
            QDomNode valueElmt = gameElmt.firstChild();

            if(gameElmt.nodeName() == "LinkedApiGames" && gameElmt.hasChildNodes())
            {
                QDomElement linkedGameElmt = gameElmt.firstChildElement();

                while(!linkedGameElmt.isNull())
                {
                    QDomElement apiGameElmt = linkedGameElmt.firstChildElement();
                    QMap<QString, QVariant> map;

                    while(!apiGameElmt.isNull())
                    {
                        QString apiName = "";
                        int remoteGame = 0;

                        QDomNode apiGameValue = apiGameElmt.firstChild();

                        if(apiGameElmt.nodeName() == "ApiName")
                        {
                            apiName = apiGameValue.nodeValue();
                            map["ApiName"] = apiName;
                        }
                        else if(apiGameElmt.nodeName() == "RemoteGameId")
                        {
                            remoteGame = apiGameValue.nodeValue().toInt();
                            map["RemoteGameId"] = remoteGame;
                        }

                        apiGameElmt = apiGameElmt.nextSiblingElement();
                    }
                    linkedApiTab.push_back(map);

                    linkedGameElmt = linkedGameElmt.nextSiblingElement();
                }
            }
            else
            {
                if(gameElmt.nodeName() == "Title")
                {
                    name = valueElmt.nodeValue();
                }
                else if(gameElmt.nodeName() == "ExePath")
                {
                    exePath =valueElmt.nodeValue();
                }
                else if(gameElmt.nodeName() == "SavePath")
                {

                }
                else if(gameElmt.nodeName() == "UninstallPath")
                {
                    uninstallPath = valueElmt.nodeValue();
                }
                else if(gameElmt.nodeName() == "AllowSaveSync")
                {
                    QVariant v(valueElmt.nodeValue());
                    allowSaveSync = v.toBool();
                }
                else if(gameElmt.nodeName() == "PlayedTime")
                {
                    playedTime = valueElmt.nodeValue().toInt();
                }
                else if(gameElmt.nodeName() == "LastPlayed")
                {
                    lastPlayed = valueElmt.nodeValue().toInt();
                }
            }

            gameElmt = gameElmt.nextSiblingElement();
        }


        int gameId = m_games.add(name, exePath, uninstallPath, savePath, allowSaveSync);
        m_games.updatePlayTime(gameId, playedTime);

        if(!linkedApiTab.empty())
        {
            for(QVector<QMap<QString, QVariant> >::iterator it = linkedApiTab.begin(); it != linkedApiTab.end(); ++it)
            {
                QString apiName = "";
                int remoteGameId = 0;

                apiName = it->value("ApiName").toString();
                remoteGameId = it->value("RemoteGameId").toInt();

                m_games.addApiGameAssociation(apiName, remoteGameId, gameId);
            }
        }
        progression++;
        progress.setValue(progression);
        QApplication::processEvents();

        game = game.nextSiblingElement();
    }

    m_database.close();
    return true;
}
