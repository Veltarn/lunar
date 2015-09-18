#include "stylesheetmanager.h"

StyleSheetManager::StyleSheetManager(QObject *parent) :
    QObject(parent)
{
    m_cssPath.setPath("resources/css/");
}

bool StyleSheetManager::load(QString filename)
{
    QString filepath = m_cssPath.path() + "/" + filename + ".css";

    m_cssFile.close();
    qDebug() << filepath << endl;
    m_cssFile.setFileName(filepath);
    if(!m_cssFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    return true;
}

QString StyleSheetManager::get()
{
    return m_cssFile.readAll();
}
