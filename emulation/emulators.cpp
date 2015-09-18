#include "emulators.h"

Emulators::Emulators()
{

}

Emulator Emulators::getEmulator(const int id)
{
    Emulator emulator;
    emulator.setId(-1);
    Database database;
    database.open();
    QSqlQuery query(database.db());

    QString sql("SELECT * FROM emulator WHERE id=:id");

    if(!query.prepare(sql)) {
        QString message = QString(QObject::tr("An error has occured while getting emulators informations") + "\n" + query.lastError().text());
        database.close();
        throw EmulatorException(message);
    }

    query.bindValue(":id", id);
    query.exec();

    while(query.next()){
        QSqlRecord record = query.record();

        emulator = this->createEmulator(record);
    }

    return emulator;
}

QList<Emulator> Emulators::getEmulators()
{
    QList<Emulator> emulators;
    Database database;
    database.open();
    QSqlQuery query(database.db());

    QString sql = "SELECT * FROM emulator ORDER BY id";

    if(!query.exec(sql)) {
        QString message = QString(QObject::tr("An error has occured while getting emulators informations") + "\n" + query.lastError().text());
        database.close();
        throw EmulatorException(message);
    }

    while(query.next()) {
        QSqlRecord record = query.record();

        Emulator emulator = this->createEmulator(record);
        emulators.append(emulator);
    }

    database.close();
    return emulators;
}

void Emulators::removeEmulator(const int id)
{
    Database database;
    database.open();
    QSqlQuery query(database.db());

    QString sql = "DELETE FROM emulator WHERE id = :id";

    if(!query.prepare(sql)) {
        QString message = QString(QObject::tr("An error has occured during emulator removal") + "\n" + query.lastError().text());
        database.close();
        throw EmulatorException(message);
    }

    query.bindValue(":id", id);
    query.exec();
}

Emulator Emulators::createEmulator(QSqlRecord &record)
{
    int id = record.value("id").toInt();
    QString name = record.value("name").toString();
    QString creationDate = record.value("creation_date").toString();
    QString exePath = record.value("exe_path").toString();
    QString commandLinePattern = record.value("command_line_pattern").toString();
    int emulatedPlatform = record.value("emulated_platform").toInt();

    Emulator emulator(id, name, creationDate, exePath, commandLinePattern, emulatedPlatform);
    return emulator;
}

