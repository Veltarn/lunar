#ifndef WINDOWMODIFGAME_H
#define WINDOWMODIFGAME_H

#include <iostream>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QGroupBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include "../apis/apis.h"
#include "../apis/apigame.h"
#include "games/db/games.h"
#include "../../gui/button.h"
#include "../gui/dialogselectgame.h"
#include "emulation/emulators.h"
#include "emulation/emulator.h"
#include "emulation/commandlinestringreplacer.h"

enum GameType {
    PC_GAME, ROM_GAME
};

class WindowModifGame : public QDialog
{
    Q_OBJECT
public:
    explicit WindowModifGame(QWidget *parent = 0, int gameId = 0);
    virtual ~WindowModifGame();
signals:
    
public slots:
    void openExeFileSelector();
    void openUninstallFileSelector();
    void openSavePathSelector();
    void enableSaveSyncBox(bool on);
    void validateForm();
    void onBtnSearchClicked();
    void onSearchFinished(ApiGameList agl);
    void onLineGameNameChanged(QString text);
    void onClickCheckBoxDeleteLink(bool on);
    void onGameTypeChanged(int index);
    void onEmulatorChanged(int index);
    void onArgumentsChanged();
protected:
    void paintEvent(QPaintEvent *pe);

private:
    void buildGui();
    void initEvents();
    void loadGameData();
    void loadEmulators();
    Emulator getEmulator(int id);
    void updateCommandLine();
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_buttonsLayout;
    QHBoxLayout *m_exePathLayout;
    QHBoxLayout *m_uninstallPathLayout;
    QHBoxLayout *m_gameNameLayout;
    QHBoxLayout *m_savePathLayout;
    QFormLayout *m_formLayout;
    QFormLayout *m_savesFormLayout;
    QFormLayout *m_romOptionsLayout;
    QVBoxLayout *m_commandLineLayout;
    QHBoxLayout *m_platformsEmulatorLayout;

    QLineEdit *m_lineGameName;
    QLineEdit *m_lineExePath;
    QLineEdit *m_lineUninstallPath;
    QLineEdit *m_lineSavePath;

    QLabel *m_gameNameInfoLabel;
    QLabel *m_executablePathLabel;

    QCheckBox *m_chkDeleteGameLink;

    QComboBox *m_gameTypeBox, *m_emulatorCombo, *m_platformsCombo;
    QTextEdit *m_commandLineText;
    QLabel *m_commandLinePreview;

    QGroupBox *m_boxSaves, *m_romOptionsBox;

    Button *m_btnValidate;
    Button *m_btnExePathBrowse;
    Button *m_btnUninstallPathBrowse;
    Button *m_btnSavePathBrowse;
    Button *m_btnCancel;
    Button *m_btnSearchGame; //Apis

    QString m_lastPositionExeWindow;
    QString m_lastPositionSaveWindow;
    
    Games m_game;

    AbstractApi *m_api;
    ApiGame m_gameToLinkWith;

    int m_gameId;
    int m_currentRemoteGameId; //Online APIs only

    int m_currentGameType;

    QList<Emulator> m_emulatorsList;
    Emulator m_selectedEmulator;
    CommandLineStringReplacer m_commandLineReplacer;
    QString m_romArguments;
};

#endif // WINDOWMODIFGAME_H
