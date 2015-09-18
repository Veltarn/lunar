#include "emulatorsknowledgebase.h"

EmulatorsKnowledgeBase::EmulatorsKnowledgeBase()
{
    this->initRegex();
}

void EmulatorsKnowledgeBase::initializeKnowledgeBase()
{
    /*
     * Charger depuis un fichier?
     * En dur?
     */
    Database database = Database();

    database.open();
    QSqlQuery query(database.db());
    QString sql = "INSERT INTO emulators_knowledge_base(emulator_name, emulated_platform, command_line_pattern) VALUES(:emulator_name, :emulated_platform, :command_line_pattern)";

    if(!query.prepare(sql)) {
        QString errorMessage(QObject::tr("An error occured during the initialization of the knowledge base") + "\n" + query.lastError().text());
        database.close();
        throw EmulatorKnowledgeBaseException(errorMessage);
    }

    QList<EmulatorKnowledge> emulators_knowledge = this->createKnowledgeBase();

    foreach(EmulatorKnowledge emulator, emulators_knowledge) {
        query.bindValue(":emulator_name", emulator.emulatorName);
        //query.bindValue(":emulated_platform", emulator.emulatedPlatform);
        query.bindValue(":command_line_pattern", emulator.commandLinePattern);
        query.exec();
        //Getting last id in order to join it into emulators_platforms_junction
        int lastId = query.lastInsertId().toInt();
        Database database2 = Database();
        database2.open();
        QSqlQuery junctionQuery(database2.db());
        QString junctionSql = "INSERT INTO emulators_knowledge_platforms_junction VALUES(:emulator_knowledge_id, :platform_id)";

        if(!junctionQuery.prepare(junctionSql)) {
            QString errorMessage(QObject::tr("An error occured during the initialization of the knowledge base") + "\n" + junctionQuery.lastError().text());
            database.close();
            database2.close();
            throw EmulatorKnowledgeBaseException(errorMessage);
        }

        foreach(Platform platform, emulator.emulatedPlatforms) {
            junctionQuery.bindValue(":emulator_knowledge_id", lastId);
            junctionQuery.bindValue(":platform_id", platform.platform_id);

            junctionQuery.exec();
        }
        database2.close();
    }
    database.close();
}

EmulatorKnowledge EmulatorsKnowledgeBase::getKnowledgeFromPath(QString filepath)
{
    QFileInfo fileInfo(filepath);
    QString basename = fileInfo.baseName();

    QString emulatorName = this->getEmulatorFromFile(basename);

    if(emulatorName.isEmpty()) {
        //If nothing has return, we return an empty knowledge
        EmulatorKnowledge invalid;
        invalid.valid = false;
        return invalid;
    }

    Database database;
    database.open();

    QSqlQuery query(database.db());
    QString sql("SELECT * FROM emulators_knowledge_base WHERE emulator_name = :name");

    if(!query.prepare(sql)) {
        QString message = QString(QObject::tr("An error occured while getting data") + "\n" + query.lastError().text());
        database.close();
        throw EmulatorKnowledgeBaseException(message);
    }

    query.bindValue(":name", emulatorName);
    query.exec();
    EmulatorKnowledge emulatorKnowledge;
    //There should be only ONE iteration
    while(query.next()) {
        QSqlRecord record = query.record();
        QSqlQuery platformsQuery;
        QString platformSql("SELECT platform.id, platform.platform_name FROM platform LEFT JOIN emulators_knowledge_platforms_junction ON platform.id = emulators_knowledge_platforms_junction.platform_id WHERE emulators_knowledge_platforms_junction.emulator_knowledge_id = :emulatorKnowledgeId");

        if(!platformsQuery.prepare(platformSql)) {
            QString message = QString(QObject::tr("An error occured while getting data") + "\n" + platformsQuery.lastError().text());
            database.close();
            throw EmulatorKnowledgeBaseException(message);
        }

        platformsQuery.bindValue(":emulatorKnowledgeId", record.value("id").toInt());
        platformsQuery.exec();
        QList<Platform> platforms = QList<Platform>();

        while(platformsQuery.next()) {
            QSqlRecord platformRecord = platformsQuery.record();
            Platform platform;

            platform.platform_id = platformRecord.value("id").toInt();
            platform.platform_name = platformRecord.value("platform_name").toString();
            platforms.append(platform);
        }

        emulatorKnowledge.emulatorName = record.value("emulator_name").toString();
        emulatorKnowledge.emulatedPlatforms = platforms;
        emulatorKnowledge.commandLinePattern = record.value("command_line_pattern").toString();
        emulatorKnowledge.valid = true;
    }
    database.close();

    return emulatorKnowledge;
}

QString EmulatorsKnowledgeBase::getEmulatorFromFile(QString filename)
{
    for(QMap<QString, QString>::Iterator it = m_regexEmulatorsName.begin(); it != m_regexEmulatorsName.end(); ++it) {
        QString key = it.key();
        QString value = it.value();
        QRegExp regex(key, Qt::CaseInsensitive);

        if(regex.indexIn(filename) != -1) {
            return value;
        }
    }

    return QString();
}

QList<EmulatorKnowledge> EmulatorsKnowledgeBase::createKnowledgeBase()
{
   QList<EmulatorKnowledge> emulators;

   QFile file(EMULATOR_KNOWLEDGE_FILE);
   file.open(QIODevice::ReadOnly);

   QTextStream stream(&file);

    while(!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList breakdownedLine = line.split("|");

        if(breakdownedLine.size() == 3) {
            EmulatorKnowledge emu;
            emu.emulatorName = breakdownedLine[0];
            emu.emulatedPlatforms = this->getEmulatedPlatform(breakdownedLine[1]);
            emu.commandLinePattern = breakdownedLine[2];

            emulators.append(emu);
        }
    }
    return emulators;
}

QList<Platform> EmulatorsKnowledgeBase::getEmulatedPlatform(QString platformStr)
{
    QStringList platformsStrList = platformStr.split(PLATFORM_SPLIT_CHAR);
    QList<Platform> platformsList = QList<Platform>();

    foreach(QString bkPlatform, platformsStrList) {
        Platform platform = Platforms::getPlatformByName(bkPlatform);

        platformsList.append(platform);
    }
    return platformsList;
}

void EmulatorsKnowledgeBase::initRegex()
{
    /*
     * todo: Mettre ça dans un fichier
     */

    QString pcsx2Regex("^(pcsx2)-(.*)$");
    QString epsxeRegex("^epsxe$");
    QString bizhawk("^EmuHawk$");
    QString dolphin("^Dolphin$");

    m_regexEmulatorsName[pcsx2Regex] = "PCSX2";
    m_regexEmulatorsName[epsxeRegex] = "ePSXE";
    m_regexEmulatorsName[bizhawk] = "BizHawk";
    m_regexEmulatorsName[dolphin] = "Dolphin";
}

