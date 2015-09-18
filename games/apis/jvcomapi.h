#ifndef JVCOMAPI_H
#define JVCOMAPI_H

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QAuthenticator>
#include <QByteArray>
#include <QDate>
#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include "abstractapi.h"

/**
 * @brief Api which communicate with the database of the french website jeuxvideo.com
 * @todo Retirer le _ws des liens afin d'avoir les images en pleine taille
 * @version 0.7
 */
class JvComApi : public AbstractApi
{
    Q_OBJECT
public:
    JvComApi(QObject *parent = 0);
    virtual ~JvComApi();

    virtual void search(QString gameName);
    virtual void get(int gameId);

public slots:
    void slotReadyRead();
    void slotFinishedSearch();
    void slotFinishedGet();
    void slotFinishedGetDetails();
    void slotFinishedGetPictures();
    void slotNetworkReplyError(QNetworkReply::NetworkError error);
    void authenticationSlot(QNetworkReply *nr, QAuthenticator *authenticator);
private:
    ApiGameList parseSearchResult();
    void parseGetResult();

    void parseSimpleGameFileResult(QDomDocument *dom);
    void parseDetailedGameFile(QDomDocument *dom);
    void parseGameScreenshots(QDomDocument *dom);

    //Fonction nettoyant les conneries de l'api de jv.com
    QString cleanDirtyTags(QString data);
private:
    QString m_detailedFilePath; // Fiche détaillée d'un jeu
    QString m_ratePath;
    QString m_picturesPath;

    QNetworkAccessManager *m_network;
    QNetworkReply *m_networkReply;
    QAuthenticator m_apiAuthenticator;

    QByteArray m_xmlBuffer;

    ApiGame m_currentGame;
    int m_gameIdToGet;
};

#endif // JVCOMAPI_H
