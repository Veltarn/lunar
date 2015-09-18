#ifndef EMULATORSKNOWLEDGEBASE_H
#define EMULATORSKNOWLEDGEBASE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QMap>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QRegExp>
#include "Exceptions/emulatorknowledgebaseexception.h"
#include "db/database.h"
#include "db/platforms.h"

#define EMULATOR_KNOWLEDGE_FILE "data/emulatorsknowledge.txt"
#define PLATFORM_SPLIT_CHAR ','

struct EmulatorKnowledge {
    QString emulatorName;
    QList<Platform> emulatedPlatforms;
    QString commandLinePattern;
    bool valid;
};

class EmulatorsKnowledgeBase
{
public:
    EmulatorsKnowledgeBase();
    /**
     * @brief Insert inside the emulatorKnowledgeBase table the basic data
     */
    void initializeKnowledgeBase();
    EmulatorKnowledge getKnowledgeFromPath(QString filepath);
private:
    QString getEmulatorFromFile(QString filename);
    QList<EmulatorKnowledge> createKnowledgeBase();
    QList<Platform> getEmulatedPlatform(QString platformStr);
    void initRegex();

private:
    QMap<QString, QString> m_regexEmulatorsName;
};

#endif // EMULATORSKNOWLEDGEBASE_H
