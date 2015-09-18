#include "process.h"

Process::Process(QObject *parent) : QProcess(parent)
{
}

Process::~Process()
{
}

void Process::start(QIODevice::OpenMode mode)
{
    QProcess::start(mode);
    m_processDuration.restart();
}

void Process::start(const QString &program, const QStringList &arguments, OpenMode mode)
{
    QProcess::start(program, arguments, mode);
    m_processDuration.restart();
}

void Process::start(const QString & command, OpenMode mode)
{
    QProcess::start(command, mode);
    m_processDuration.restart();
}

int Process::getPlayedTime()
{
    return m_processDuration.elapsed();
}

QTime Process::getProcessTime()
{
    return m_processDuration;
}
