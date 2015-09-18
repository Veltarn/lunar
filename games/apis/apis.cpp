#include "apis.h"

AbstractApi *ApiSelector::getUsedApi()
{
    AbstractApi *api = NULL;
    QSettings confFile(CONFIGURATION_FILE, QSettings::IniFormat);
    QString currentApi = "";

    confFile.beginGroup("Database API");
    currentApi = confFile.value("API", "basic").toString();
    confFile.endGroup();

    if(currentApi == "jeuxvideo_dot_com")
    {
        api = new JvComApi();
    }

    return api;
}
