#include "cache.h"

Cache::Cache(QObject *parent) :
    QObject(parent)
{
    m_cachePath.setPath(CACHE_PATH);
}

void Cache::clearCache()
{
    QStringList entryList = m_cachePath.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    bool error = false;
    for(QStringList::iterator it = entryList.begin(); it != entryList.end(); ++it)
    {
        QFile file(CACHE_PATH + *it);
        if(!file.remove())
        {
            error = true;
            qDebug() << "An error has occured during suppression of: " << *it << endl;
        }
    }

    if(!error)
    {
        QMessageBox::information(0, tr("Success"), tr("Lunar's cache has been correctly deleted"));
    }
    else
    {
        QMessageBox::warning(0, tr("Warning"), tr("Unable to delete several cache files..."));
    }
}

QString Cache::getCacheSize()
{
    float result = 0.0;
    QFileInfoList infoList = m_cachePath.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for(QFileInfoList::iterator it = infoList.begin(); it != infoList.end(); ++it)
    {
        result += it->size();
    }

    //Convert to human readable format
    QVector<QString> byteNotation;
    byteNotation.push_back(tr("byte"));
    byteNotation.push_back("Ko");
    byteNotation.push_back("Mo");
    byteNotation.push_back("Go");
    byteNotation.push_back("To");

    int vectorIndex = 0;

    while(result > 1024 && vectorIndex < byteNotation.size())
    {
        result = result / 1024;
        vectorIndex++;
    }
    result = floorf(result * 100) / 100;
    QString stringSize = QVariant(result).toString() + " " + byteNotation[vectorIndex];
    return stringSize;
}
