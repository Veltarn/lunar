#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <QProcess>
#include <QTime>

class Process : public QProcess
{
public:
    Process(QObject *parent = 0);
    ~Process();

    void start(OpenMode mode = ReadWrite);
    void start(const QString &program, const QStringList &arguments, OpenMode mode = ReadWrite);
    void start(const QString & command, OpenMode mode = ReadWrite);

    int getPlayedTime();
    QTime getProcessTime();
private:
    QTime m_processDuration;
};

#endif // PROCESS_H
