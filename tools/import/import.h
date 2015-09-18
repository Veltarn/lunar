#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <QObject>
#include <QMessageBox>
#include <QtXml>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QProgressDialog>
#include <QApplication>
#include "../../db/database.h"
#include "../../games/db/games.h"

class Import : public QObject
{
    Q_OBJECT
public:
    Import(QObject *parent = 0);
    bool startImport(QString file);
private:
    Database m_database;
    Games m_games;
};

#endif // IMPORT_H
