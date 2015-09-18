#include "processmanager.h"

using namespace std;


void ProcessManager::startGame(QModelIndex indexGame)
{
    Process *process = new Process;
    process->setStandardErrorFile("log/" + indexGame.data().toString() + ".err.log");
    process->setStandardOutputFile("log/" + indexGame.data().toString() + ".log");
    QModelIndex indexPath = indexGame.sibling(indexGame.row(), 2);
    QModelIndex emulatorUsedIndex = indexGame.sibling(indexGame.row(), 8);
    QModelIndex argumentsIndex = indexGame.sibling(indexGame.row(), 9);

    QString url = indexPath.data().toString();
    int emulatorUsed = emulatorUsedIndex.data().toInt();

    //If the game is a regular PC game
    if(emulatorUsed == -1) {
        QFileInfo fileInfo(url);
#ifdef WINDOWS
        //Replacing slashes by anti slashes on windows
        url = url.replace("/", "\\");
        url = "\"" + url + "\"";
#endif
        //Starting the game only if the process doesnt exists
        if(!processExists(indexGame))
        {
            m_processesList[process] = indexGame;

            connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onProcessFinished(int,QProcess::ExitStatus)));

            process->setWorkingDirectory(fileInfo.path());
            process->start(url, QStringList());
        }
        else
        {
            QMessageBox::warning(0, tr("Warning"), QString("<b>" + indexGame.data().toString() + "</b> " + tr("is already running")));
        }
    } else {
        //Otherwise, it's an emulated game
        Emulators emulators;
        Emulator emulator = emulators.getEmulator(emulatorUsed);
        QString arguments = argumentsIndex.data().toString();
        QFileInfo fileInfo(url);
        if(!processExists(indexGame)) {
            QString commandData = this->prepareEmulation(url, emulator, arguments);
            int i = 0;
            if(!commandData.isEmpty()) {
                m_processesList[process] = indexGame;
                connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onProcessFinished(int,QProcess::ExitStatus)));
                connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));

                QFileInfo emulatorDir(QDir::toNativeSeparators(emulator.exePath()));
                process->setWorkingDirectory(emulatorDir.path());
                qDebug() << "Working dir: " << process->workingDirectory() << endl;
/*
                //Yahooza!
                QStringList args;
                args << commandData[1];

                if(commandData.size() == 3 && !commandData[2].isEmpty()) {
                    args << commandData[2];
                }
#ifdef WINDOWS
                commandData[0] = "\"" + commandData[0] + "\"";
#endif
                process->setProgram(commandData[0]);
                process->setArguments(args);*/
                process->start(commandData);
                qDebug() << process->program() << endl << process->arguments() << endl;
            } else {
                QMessageBox::critical(0, tr("Error"), tr("Cannot start the emulation, something happenned with the command.."));
            }
        }
    }
}

void ProcessManager::startProgram(QString url)
{
    #ifdef WINDOWS
    //Replacing slashes by anti slashes on windows
    url = url.replace("/", "\\");
    url = "\"" + url + "\"";
    #endif
    Process::startDetached(url);
}

void ProcessManager::onProcessFinished(int exitCode, Process::ExitStatus status)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(status);

    Process *p = dynamic_cast<Process*>(sender());

    QModelIndex mi = m_processesList[p];
    QModelIndex id = mi.sibling(mi.row(), 1);

    int playDuration = p->getPlayedTime();

    m_game.updatePlayTime(id.data().toInt(), playDuration);
    m_game.syncSaves(id.data().toInt());

    m_processesList.remove(p);

    delete p;
    p = NULL;
}

void ProcessManager::onProcessError(QProcess::ProcessError error)
{
    std::cout << error << std::endl;
    QProcess *p = qobject_cast<QProcess*>(sender());

    qDebug() << "State: " << p->exitStatus() << endl;
}

void ProcessManager::onDestroyed()
{

}

/*QStringList ProcessManager::prepareEmulation(QString isoUrl, int emulatorId, QString arguments)
{
    QStringList list;
    CommandLineStringReplacer commandReplacer;
    Emulators emulators;
    Emulator emulator = emulators.getEmulator(emulatorId);

    if(emulator.id() != -1) {
        QString exepath = emulator.exePath();
        QString commandLinePattern = emulator.commandLinePattern();
        commandReplacer.setCommandLinePattern(commandLinePattern);
#ifdef WINDOWS
        exepath = exepath.replace("/", "\\");
        //exepath = "\"" + exepath + "\"";
        isoUrl = isoUrl.replace("/", "\\");
        //isoUrl = "\"" + isoUrl + "\"";
#endif
        commandReplacer.replace("%emulator_path%", exepath);
        commandReplacer.replace("%rom_path%", isoUrl);

        if(!arguments.isEmpty()) {
            commandReplacer.replace("%optionnal_arguments%", arguments);
        } else {
            commandReplacer.replace("%optionnal_arguments%", "");
        }
        //list.append(commandReplacer.result());
        list.append(exepath);
        list.append(isoUrl);
        /*if(!arguments.isEmpty()) {
            list.append(arguments);
        }*//*
        return list;

    } else {
        return QStringList();
    }
}*/

QString ProcessManager::prepareEmulation(QString isoUrl, Emulator &emulator, QString arguments)
{
    QStringList list = QStringList();
    CommandLineStringReplacer commandReplacer;

    if(emulator.id() != -1) {
        QString exepath = emulator.exePath();
        QString commandLinePattern = emulator.commandLinePattern();
        commandReplacer.setCommandLinePattern(commandLinePattern);
#ifdef WINDOWS
        exepath = exepath.replace("/", "\\");
        exepath = "\"" + exepath + "\"";
        isoUrl = isoUrl.replace("/", "\\");
        isoUrl = "\"" + isoUrl + "\"";
#endif
        commandReplacer.replace("%emulator_path%", exepath);
        commandReplacer.replace("%rom_path%", isoUrl);

        if(!arguments.isEmpty()) {
            commandReplacer.replace("%optionnal_arguments%", arguments);
        } else {
            commandReplacer.replace("%optionnal_arguments%", "");
        }
        //list.append(commandReplacer.result());
        list.append(exepath);
        list.append(isoUrl);
        /*if(!arguments.isEmpty()) {
            list.append(arguments);
        }*/
        return commandReplacer.result();

    } else {
        return QString/*List*/();
    }
}

bool ProcessManager::processExists(QModelIndex mi)
{
    for(QMap<Process*, QModelIndex>::iterator it = m_processesList.begin(); it != m_processesList.end(); ++it)
    {
        if(*it == mi)
        {
            return true;
        }
    }

    return false;
}

QMap<Process*, QModelIndex> &ProcessManager::getList()
{
    return m_processesList;
}
