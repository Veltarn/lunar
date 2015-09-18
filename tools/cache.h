#ifndef CACHE_H
#define CACHE_H

#include <cmath>
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QVariant>
#include <QMessageBox>
#include <QVector>
#include "../constants.h"

class Cache : public QObject
{
    Q_OBJECT
public:
    explicit Cache(QObject *parent = 0);

    void clearCache();
    QString getCacheSize();
    
signals:
    
public slots:

private:
    QDir m_cachePath;
    
};

#endif // CACHE_H
