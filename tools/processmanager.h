#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <iostream>
#include <QObject>
#include <QModelIndex>
#include <QMessageBox>
#include <QMap>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include "../games/db/games.h"
#include "emulation/emulators.h"
#include "emulation/emulator.h"
#include "emulation/commandlinestringreplacer.h"
#include "process.h"
#include "singleton.h"
#include "constants.h"

class ProcessManager : public Singleton<ProcessManager>
{
   Q_OBJECT
public:

    void startGame(QModelIndex indexGame);
    void startProgram(QString url);
    bool processExists(QModelIndex mi); //Search if the process associated to "mi" exists
    QMap<Process*, QModelIndex> &getList();

public slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onProcessError(QProcess::ProcessError error);
protected:
    virtual void onDestroyed();
private:
    //Returns the full command line to start the emulator
    //Dirty choice: [0] is the full command, [1] is the emulator filepath
    QString prepareEmulation(QString isoUrl, Emulator &emulator, QString arguments);
private:
    QMap<Process*, QModelIndex> m_processesList;
    Games m_game;
};

#endif // PROCESSMANAGER_H
