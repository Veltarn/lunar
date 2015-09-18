#include "emulator.h"

Emulator::Emulator()
{

}

Emulator::Emulator(int id, QString name, QString creationDate, QString exePath, QString commandLinePattern, int emulatedPlatform)
{
    this->setId(id);
    this->setEmulatorName(name);
    this->setCreationDate(creationDate);
    this->setExePath(exePath);
    this->setCommandLinePattern(commandLinePattern);
    this->setEmulatedPlatform(emulatedPlatform);
}

int Emulator::id() const
{
    return m_id;
}

void Emulator::setId(int id)
{
    m_id = id;
}

QString Emulator::emulatorName() const
{
    return m_emulatorName;
}

void Emulator::setEmulatorName(const QString &emulatorName)
{
    m_emulatorName = emulatorName;
}

QDateTime Emulator::creationDate() const
{
    return m_creationDate;
}

void Emulator::setCreationDate(const QDateTime &creationDate)
{
    m_creationDate = creationDate;
}

void Emulator::setCreationDate(QString &date)
{
   QDateTime datetime = QDateTime::fromString(date, STANDARD_FR_DATETIME);
   m_creationDate = datetime;
}

QString Emulator::exePath() const
{
    return m_exePath;
}

void Emulator::setExePath(const QString &exePath)
{
    m_exePath = exePath;
}

QString Emulator::commandLinePattern() const
{
    return m_commandLinePattern;
}

void Emulator::setCommandLinePattern(const QString &commandLinePattern)
{
    m_commandLinePattern = commandLinePattern;
}

Platform Emulator::emulatedPlatform() const
{
    return m_emulatedPlatform;
}

void Emulator::setEmulatedPlatform(const int &emulatedPlatform)
{
    Platform newPlatform = Platforms::getPlatformById(emulatedPlatform);
    m_emulatedPlatform = newPlatform;
}
