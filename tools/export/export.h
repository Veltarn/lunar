#ifndef EXPORT_H
#define EXPORT_H

#include <iostream>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QtXml>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

#include "../../db/database.h"

class Export : public QObject
{
    Q_OBJECT
public:
    explicit Export(QObject *parent = 0);
    bool startExport(QString path, bool preserveApiLink = false);
signals:
    
public slots:


private:
    Database m_database;
};

#endif // EXPORT_H
