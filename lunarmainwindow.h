#ifndef LUNARMAINWINDOW_H
#define LUNARMAINWINDOW_H

#include <iostream>
#include "constants.h"
#include <QMainWindow>
#include <QPainter>
#include <QSplitter>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QSettings>
#include <QToolButton>
#include <QToolBar>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QModelIndex>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QRadialGradient>
#include <QPixmap>
#include <QMap>
#include <QPoint>
#include <QRegion>
#include <QPropertyAnimation>
#include "gui/listview.h"
#include "gui/menubar.h"
#include "gui/menu.h"
#include "gui/contextualmenu.h"
#include "games/gui/windowmodifgame.h"
#include "games/db/games.h"
#include "db/database.h"
#include "tools/processmanager.h"
#include "gui/windowprocessmanager.h"
#include "gui/windowoptions.h"
#include "gui/gameinfowidget.h"
#include "tools/slideshow/slideshow.h"
#include "gui/aboutlunar.h"
#include "tools/export/windowexport.h"
#include "tools/import/windowimport.h"
#include "tools/stylesheetmanager.h"
#include "gui/scrollbar.h"
#include "gui/emulatorswindow.h"
#include "gui/pluginswindow.h"

namespace Ui {
class LunarMainWindow;
}

class LunarMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LunarMainWindow(QWidget *parent = 0);
    ~LunarMainWindow();

public slots:
    void addGame();
    void onClickItemList(QModelIndex mi);
    void onEditGameButtonClick();
    void onDeleteGameButtonClick();
    void onLaunchGameButtonClick();
    void onUninstallButtonClick();
    void openContextualMenu(QPoint);
    void openProcessManagerWindow();
    void deleteProcessManagerWindow();
    void openOptionDialog();
    void openAboutLunar();
    void onSlideChanged(QPixmap *pix);
    void onFadeFinished();
    void openWindowExport();
    void openWindowImport();
    void openEmulatorsWindow();
    void onUninstallProcessFinished(int code);
    void onPluginsActionTriggered();

protected:
    void paintEvent(QPaintEvent *pe);
    void closeEvent(QCloseEvent *ce);

private:
    void processCacheCleaning();
    void loadGamesList();
    void displayGameInfos(QModelIndex mi);

    void initGui();
    void buildGui();
    void buildMenus();
    void readOptions();
    void saveOptions();
    void initActions();
    void initEvents();
    void uninstallGame(QString path);

private:
    Ui::LunarMainWindow *ui;

    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    QStackedWidget *m_overviewStack; // Contains the overview and game specific widget

    GameInfoWidget *m_gameInfo;

    ListView *m_gameList;

    MenuBar *m_menuBar;

    Menu *m_mainMenu;
    Menu *m_gameMenu;
    Menu *m_emulation;
    Menu *m_toolsMenu;
    Menu *m_helpMenu;

    QToolButton *m_gameToolButton;

    QSettings m_configurationFile;

    int m_startPosX;
    int m_startPosY;

    unsigned int m_startWidth;
    unsigned int m_startHeight;

    bool m_isMaximized;
    bool m_allowSlideshow;
    bool m_neverCleanCache;

    QDate m_cacheDuration;
    /**
     * Lunar Menu
     */
    QAction *m_actionImport;
    QAction *m_actionExport;
    QAction *m_actionQuit;

    /**
     *  Games menu
     */
    QAction *m_actionNewGame;
    QAction *m_actionEditGame;
    QAction *m_actionDeleteGame;
    QAction *m_actionLaunchGame;
    QAction *m_actionUninstallGame;
    QAction *m_actionShowEmulators;

    /**
     *  Tools menu
     */
    QAction *m_actionOptions;
    QAction *m_actionPlugins;
    QAction *m_actionProcessManager;

    /**
     *  Help menu
     */
    QAction *m_actionAboutLunar;
    QAction *m_actionAboutQt;

    WindowModifGame *m_windowAddGames;
    WindowProcessManager *m_windowProcessManager;
    WindowOptions *m_windowOptions;

    /*
     * Data
     */
    Games m_games;

    ProcessManager *m_processManager;

    QString m_apiType; //Database API

    QGraphicsView *m_graphicSlide;
    QGraphicsScene *m_graphicScene;
    QPixmap *m_currentSlide;


    Slideshow *m_slideshow;

    QPropertyAnimation *m_animation; //Screenshots animation

    int m_currentUninstallingGameId;
};

#endif // LunarMAINWINDOW_H
