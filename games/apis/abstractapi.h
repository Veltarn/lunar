#ifndef ABSTRACTAPI_H
#define ABSTRACTAPI_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "apigame.h"
#include "../../db/database.h"

class AbstractApi : public QObject
{
    Q_OBJECT
public:
    AbstractApi(QObject *parent = 0);

    virtual void search(QString gameName) = 0;
    virtual void get(int gameId) = 0;

    virtual bool isOnlineApi() const;
    virtual QString getApiName() const;

    virtual int getMaxRate() const;
signals:
    void finishedSearch(ApiGameList agl);
    void finishedGet(ApiGame ag);
protected:
    QString m_apiName;
    bool m_onlineApi;
    bool m_authenticationRequired;
    QString m_host;
    QString m_getPath;
    QString m_searchPath;
    QString m_username;
    QString m_password;

    Database m_database;

    int m_maxRate;
};

#endif // ABSTRACTAPI_H
