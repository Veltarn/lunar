#ifndef GAMEINFOWIDGET_H
#define GAMEINFOWIDGET_H

#include <iostream>
#include <QDebug>
#include <QWidget>
#include <QSqlRecord>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QTime>
#include <QModelIndex>
#include <QFont>
#include <QSettings>
#include "../constants.h"
#include "../tools/processmanager.h"
#include "../games/db/games.h"
#include "../games/apis/apis.h"
#include "../games/apis/apigame.h"
#include "../tools/slideshow/slideshow.h"
#include "emulation/emulators.h"
#include "emulation/emulator.h"
#include "button.h"

class GameInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameInfoWidget(QWidget *parent = 0);
    
    void clearInfos();
    void setGame(QModelIndex gameIndex, QString apiName);
    /**
     * @brief Get informations from database via current used API
     */
    void getRemoteGameInfos(int remoteGameId);
signals:
    
public slots:
    void startGame();
    void onFinishedGet(ApiGame game);
private:
    bool m_allowSlideshow;
    QModelIndex m_currentIndex;

    QString m_apiType;
    QString m_titleStyleSheet;
    QString m_textStyleSheet;
    //QString m_backgroundLabel;

    QGridLayout *m_mainLayout;
    QGridLayout *m_gameInfos;
    QHBoxLayout *m_infoLayout;

    QLabel *m_gameTitle;
    QLabel *m_playedTime;
    QLabel *m_lastPlayedTime; //Unused
    QLabel *m_gameRate; //Api only
    QLabel *m_developerLabel;
    QLabel *m_publisherLabel;
    QLabel *m_esrbLabel;
    QLabel *m_overviewLabel;
    QLabel *m_genreGame;
    QLabel *m_emulatorUsedLabel;

    QWidget *m_savesWidget; //A remplacer par le vrai widget d'affichage des sauvegardes

    Button *m_playButton;

    Games m_games;

    AbstractApi *m_dbGameApi;

    ProcessManager *m_processManager;

    Slideshow *m_slideshow;

private:
    void buildGui();
    void initEvents();
    void msToRegularTime(int *h, int *m, int *s, int ms);
    QString formatTimeToString(int ms);
};

#endif // GAMEINFOWIDGET_H
