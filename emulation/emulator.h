#ifndef EMULATOR_H
#define EMULATOR_H

#include <QDateTime>
#include <QString>
#include "db/platforms.h"
#include "constants.h"

class Emulator
{
public:
    Emulator();
    Emulator(int id, QString name, QString creationDate, QString exePath, QString commandLinePattern, int emulatedPlatform);

    int id() const;
    void setId(int id);

    QString emulatorName() const;
    void setEmulatorName(const QString &emulatorName);

    QDateTime creationDate() const;
    void setCreationDate(const QDateTime &creationDate);
    void setCreationDate(QString &date);

    QString exePath() const;
    void setExePath(const QString &exePath);

    QString commandLinePattern() const;
    void setCommandLinePattern(const QString &commandLinePattern);

    Platform emulatedPlatform() const;
    void setEmulatedPlatform(const int &emulatedPlatform);

protected:
    int m_id;
    QString m_emulatorName;
    QDateTime m_creationDate;
    QString m_exePath;
    QString m_commandLinePattern;
    Platform m_emulatedPlatform;
};

#endif // EMULATOR_H
