#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <cstdlib>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "../constants.h"

class EmulatorsKnowledgeBase;

class Database
{
public:
    Database();
    ~Database();

    void open();
    void close();
    //Get a reference of the database
    QSqlDatabase &db();

    static int m_openedConnections;
    static int openConnections();

private:
    /**
     * Verify if this is the first start of Lunar
     *if it is, it will create the tables
     */
    void initDb();
    void createTables();

    void updateDatabase();
    /**
     * @brief Compare v1 with v2
     * @param v1
     * @param v2
     * @return -1 if v1 is tinyer than v2, 0 if v1 equals v2, 1 if v1 is greater than v2
     */
    int versionCompare(QString v1, QString v2);
    void hydratePlatformsTable();
    void initializeKnowledgeBase();

private:
    QSqlDatabase m_db;
    QString m_host;
    QString m_hostname;
    QString m_hostpass;
    QString m_database;
    /*
     * No necessarly identical to software
     * current version, and if it's not
     * the same, an update have to be
     * performed
     */
    QString m_settingsVersion;
    bool m_firstStart;
};

#endif // DATABASE_H
