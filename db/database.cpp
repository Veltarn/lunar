#include "database.h"
#include "emulation/emulatorsknowledgebase.h"


Database::Database() :
    m_host("localhost"),
    m_hostname("root"),
    m_hostpass(""),
    m_database("lunar.db")
{
    QSettings options(CONFIGURATION_FILE, QSettings::IniFormat);

    options.beginGroup("General");
    m_settingsVersion = options.value("version", "0.0").toString();
    options.endGroup();

    options.beginGroup("database");
    m_firstStart = options.value("firstStart", true).toBool();
    options.endGroup();

    initDb();
}

Database::~Database()
{
    if(m_db.isOpen())
    {
        m_openedConnections--;
        m_db.close();
    }
}


void Database::initDb()
{
    QFileInfo dbInfo("lunar.db");
    //Creating database if this is the first start
    if(m_firstStart || !dbInfo.exists())
    {
        this->open();
        QSqlQuery query(m_db);

        QStringList tablesList;
        tablesList.push_back("CREATE TABLE 'games' ( 'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'name' TEXT NOT NULL, 'exepath' TEXT NOT NULL, 'uninstallpath' TEXT, 'savepath' TEXT, 'allowsavesync' BOOL NOT NULL DEFAULT false, 'playedtime' INTEGER, 'lasttimeplayed' INTEGER, 'emulator_used' INTEGER, 'command_arguments' TEXT);");
        tablesList.push_back("CREATE TABLE 'pictures' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'game_id' INTEGER NOT NULL , 'picturepath' TEXT NOT NULL )");
        tablesList.push_back("CREATE TABLE 'saveregister' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'game_id' INTEGER NOT NULL , 'savetime' DATETIME NOT NULL )");
        tablesList.push_back("CREATE TABLE 'api_game_association' ('api_name' VARCHAR(150) NOT NULL , 'local_game_id' INTEGER NOT NULL , 'remote_game_id' INTEGER NOT NULL )");
        tablesList.push_back("CREATE TABLE 'platform' ( 'id' INTEGER PRIMARY KEY AUTOINCREMENT, 'platform_name' VARCHAR (30));");
        tablesList.push_back("CREATE TABLE emulator ( 'id' INTEGER PRIMARY KEY AUTOINCREMENT, 'name' VARCHAR (120), 'creation_date' DATETIME, 'exe_path' TEXT, 'command_line_pattern' TEXT, 'emulated_platform' INTEGER);");
        tablesList.push_back("CREATE TABLE 'emulators_knowledge_base' ('id' INTEGER PRIMARY KEY AUTOINCREMENT, 'emulator_name' VARCHAR (30), 'emulated_platform' INTEGER, 'command_line_pattern' TEXT, 'data' BLOB );");
        tablesList.push_back("CREATE TABLE emulators_platforms_junction('emulator_id' INTEGER, 'platform_id' INTEGER)");
        tablesList.push_back("CREATE TABLE 'emulators_knowledge_platforms_junction' ( 'emulator_knowledge_id' INTEGER, 'platform_id' INTEGER)");


        for(int i = 0; i < tablesList.size(); i++)
        {
            if(!query.exec(tablesList.at(i)))
            {
                QMessageBox::critical(0, QObject::tr("Error creating tables"), QString(QObject::tr("An error as occured during the creation of the table") + QString(" #") + QString::number(i + 1)));
                return;
            }
        }
        this->close();

        QSettings options(CONFIGURATION_FILE, QSettings::IniFormat);

        options.beginGroup("database");
        options.setValue("firstStart", false);
        options.endGroup();

        options.beginGroup("General");
        options.setValue("version", SOFTWARE_VERSION);
        options.endGroup();

        this->hydratePlatformsTable();
        this->initializeKnowledgeBase();

        m_firstStart = false;
    }
    else if(versionCompare(m_settingsVersion, SOFTWARE_VERSION) == -1)
    {
        //updateDatabase();
    }
}

void Database::updateDatabase()
{
    this->open();

    QString sql = "ALTER TABLE games ADD COLUMN uninstallpath TEXT";

    QSqlQuery query(m_db);

    if(!query.exec(sql))
    {
        QMessageBox::critical(0, QObject::tr("Error while updating tables"), QString(QObject::tr("An error as occured during the update of the tables") + ":  " + query.lastError().text()));
        this->close();
        return;
    }

    QSettings options(CONFIGURATION_FILE, QSettings::IniFormat);

    options.beginGroup("General");
    options.setValue("version", SOFTWARE_VERSION);
    options.endGroup();

    qDebug() << "Lunar has been successfully updated" << endl;

    this->close();
}

void Database::open()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName(m_host);
    m_db.setUserName(m_hostname);
    m_db.setPassword(m_hostpass);
    m_db.setDatabaseName(m_database);

    if(!m_db.open())
    {
        QMessageBox::critical(0, QObject::tr("Error"), QString(QObject::tr("An error has occured during the connection to the database. Unable to continue") + "<br />" + m_db.lastError().text()));
        exit(1);
    }
    m_openedConnections++;
}

void Database::close()
{
    m_db.close();

    if(!m_db.isOpen()) {
        m_openedConnections--;
    }
}

QSqlDatabase &Database::db()
{
    return m_db;
}

int Database::openConnections()
{
    return m_openedConnections;
}

int Database::versionCompare(QString v1, QString v2)
{
    QStringList v1List = v1.split(".");
    QStringList v2List = v2.split(".");
    bool end = false;
    int valueIndicator = 0;
    int minSize = std::min(v1List.size(), v2List.size());
    for(int i = 0; i < minSize && !end; ++i)
    {
        int v1Number = v1List.at(i).toInt();
        int v2Number = v2List.at(i).toInt();

        if(v1Number < v2Number)
        {
            valueIndicator = -1;
            end = true;
        }
        else if(v1Number > v2Number)
        {
            valueIndicator = 1;
            end = true;
        }
        else
            valueIndicator = 0;
    }

    return valueIndicator;
}

void Database::hydratePlatformsTable()
{
    QFile file(PLATFORM_FILE);
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);

    this->open();
    QString sql = "INSERT INTO 'platform' ('platform_name') VALUES(:name)";
    QSqlQuery query(m_db);

    if(!query.prepare(sql)) {
        QMessageBox::critical(0, QObject::tr("Error creating tables"), QString(QObject::tr("An error has occured during the creation of the tables: ") + query.lastError().text()));
    }

    while(!textStream.atEnd()) {
        QString a = textStream.readLine();

        query.bindValue(":name", a);
        query.exec();
    }
    this->close();
}

void Database::initializeKnowledgeBase()
{
    EmulatorsKnowledgeBase *emulatorsKnowledge = new EmulatorsKnowledgeBase();
    try {
        emulatorsKnowledge->initializeKnowledgeBase();
    } catch(EmulatorKnowledgeBaseException e) {
        QString msg = QString::fromStdString(e.message());
        QMessageBox::critical(0, QObject::tr("Error"), msg);
    }

    delete emulatorsKnowledge;
    emulatorsKnowledge = NULL;
}

int Database::m_openedConnections = 0;
