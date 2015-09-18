#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <iostream>
#include <QObject>
#include <QDebug>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDir>
#include <QIcon>
#include <QProgressDialog>
#include <QFileInfoList>
#include <QFile>
#include <QString>
#include <zlib.h>
#include "../../tools/qtarout.h"
#include "../../tools/singleton.h"

class SaveManager : public Singleton<SaveManager>
{
    Q_OBJECT
public:
    void compressSaveFolder(QString gameName, QString path);
    void sendSave();

public slots:
    void updateProgressionBar(float p);
    void newFileArchived(QString s);
    void cleanConnections();
protected:
    virtual void onDestroyed();
private:
    void parseSaveFolder(QTarOut &tar, QDir root, QString path);

private:
    QProgressDialog *m_progressDialog;
};

#endif // SAVEMANAGER_H
