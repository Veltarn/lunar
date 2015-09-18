#include "savemanager.h"


void SaveManager::compressSaveFolder(QString gameName, QString path)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QDate date = dateTime.date();
    QTime time = dateTime.time();


    QString archiveName = QDir::tempPath() + "/" + gameName + "-" + QString::number(date.day()) + "-" + QString::number(date.month()) + "-" + QString::number(date.year()) + "-" + QString::number(time.hour()) + "-" + QString::number(time.minute()) + "-" + QString::number(time.second()) + "_save.tar";

    m_progressDialog = new QProgressDialog(QObject::tr("Archiving save files"), QObject::tr("Cancel"), 0, 100);
    m_progressDialog->setWindowIcon(QIcon(QPixmap("resources/imgs/logo.png")));
    m_progressDialog->setWindowTitle(QObject::tr("Archiving save files"));
    m_progressDialog->setCancelButton(0); //Rendre annulable...

    m_progressDialog->setMinimumDuration(1000);
    QTarOut *tarOut = new QTarOut(archiveName, path);
    QObject::connect(tarOut, SIGNAL(progression(float)), this, SLOT(updateProgressionBar(float)));
    QObject::connect(tarOut, SIGNAL(fileBeingArchived(QString)), this, SLOT(newFileArchived(QString)));
    QObject::connect(tarOut, SIGNAL(finished()), this, SLOT(cleanConnections()));
    QDir root(path);
    this->parseSaveFolder(*tarOut, root, path);

    tarOut->start();
}

void SaveManager::parseSaveFolder(QTarOut &tar, QDir root, QString path)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for(QFileInfoList::iterator it = list.begin(); it != list.end(); ++it)
    {
        if(it->isDir())
        {
            this->parseSaveFolder(tar, root, it->filePath());
        }
        else
        {
            tar.addPath(it->absoluteFilePath());
        }
    }
}

void SaveManager::updateProgressionBar(float p)
{
    if(m_progressDialog != NULL)
        m_progressDialog->setValue(p);
}

void SaveManager::cleanConnections()
{
    QTarOut *tar = dynamic_cast<QTarOut*>(sender());

    tar->disconnect(SIGNAL(progession(float)));
    tar->disconnect(SIGNAL(fileBeingArchived(QString)));
    tar->disconnect(SIGNAL(finished));

    delete m_progressDialog;
    m_progressDialog = NULL;

}

void SaveManager::onDestroyed()
{

}

void SaveManager::newFileArchived(QString s)
{
    if(m_progressDialog != NULL)
    {
        QString label = QObject::tr("Archiving") + " " + s;
        m_progressDialog->setLabelText(label);
    }
}
