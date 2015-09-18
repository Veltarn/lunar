#ifndef QTAROUT_H
#define QTAROUT_H

#include <iostream>
#include <cstring>
#include <QThread>
#include <QDebug>
#include <QQueue>
#include <QDir>
#include "libTar/TarOut.h"

class QTarOut : public QThread, public TarOut
{
    Q_OBJECT
public:
    QTarOut(QString tarName, QString &workDirectory, QObject *parent = 0);
    /**
     * @brief Add "filePath" to m_fileListToArchive. The files are archived when start() method is called
     * @param filePath
     */
    void addPath(QString filePath);
public slots:
    void archive();

signals:
    void progression(float);
    void fileBeingArchived(QString);

protected:
    void run();

private:
    float calculateProgression(unsigned int fileIndex, unsigned int nbFiles);
private:
    QQueue<QString> m_fileListToArchive;
    QString m_workingDirectory;
};

#endif // QTAROUT_H
