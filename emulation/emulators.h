#ifndef EMULATORS_H
#define EMULATORS_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include "emulation/emulator.h"
#include "Exceptions/emulatorexception.h"
#include "db/database.h"

class Emulators
{
public:
    Emulators();
    Emulator getEmulator(const int id);
    QList<Emulator> getEmulators();
    static void removeEmulator(const int id);
private:
    Emulator createEmulator(QSqlRecord &record);
};

#endif // EMULATORS_H
