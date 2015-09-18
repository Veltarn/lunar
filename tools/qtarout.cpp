#include "qtarout.h"

QTarOut::QTarOut(QString tarName, QString &workDirectory, QObject *parent) : QThread(parent), TarOut(tarName.toStdString(), "trunc"), m_workingDirectory(workDirectory)
{
}

void QTarOut::addPath(QString filePath)
{
    m_fileListToArchive.enqueue(filePath);
}

void QTarOut::run()
{
    this->archive();
    this->exec();
}

void QTarOut::archive()
{
    unsigned int nbFiles = m_fileListToArchive.size();
    unsigned int i = 0;
    QDir root(m_workingDirectory);
    while(!m_fileListToArchive.isEmpty())
    {
        QString str = m_fileListToArchive.dequeue();
        QDir dir(str);

        //qDebug() << root.relativeFilePath(str) << endl;
        emit fileBeingArchived(root.relativeFilePath(str));
        this->putFile(str.toStdString(), root.relativeFilePath(str).toStdString());

        ++i;
        //Calculating progession
        float progress = this->calculateProgression(i, nbFiles);
        emit progression(progress);
    }

    this->finish();
}

float QTarOut::calculateProgression(unsigned int fileIndex, unsigned int nbFiles)
{
    float result = 0.0;

    result = (fileIndex * 100) / nbFiles;

    return result;
}
