#include "abstractapi.h"

AbstractApi::AbstractApi(QObject *parent) : QObject(parent)
{
    m_database = Database();
}

bool AbstractApi::isOnlineApi() const
{
    return m_onlineApi;
}

QString AbstractApi::getApiName() const
{
    return m_apiName;
}

int AbstractApi::getMaxRate() const
{
    return m_maxRate;
}
