#include "lunarmainwindow.h"
#include "ui_lunarmainwindow.h"

using namespace std;

LunarMainWindow::LunarMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LunarMainWindow),
    m_configurationFile(CONFIGURATION_FILE, QSettings::IniFormat)
{
    ui->setupUi(this);

    //m_currentSlide = new QGraphicsPixmapItem;
    m_currentSlide = NULL;
    m_graphicScene = new QGraphicsScene;
    m_graphicSlide = new QGraphicsView(m_graphicScene);


    m_processManager = ProcessManager::getInstance();
    m_slideshow = Slideshow::getInstance();
    /*m_animation = new QPropertyAnimation(m_slideshowForeground, "windowOpacity", this);
    m_animation->setLoopCount(-1);
    m_animation->setEndValue(255);
    m_animation->setStartValue(255);
    m_animation->setKeyValueAt(0.0, 255);*/

    this->setMinimumSize(900, 590);
    this->readOptions();

    if(!m_neverCleanCache)
    {
        processCacheCleaning();
    }

    if(m_isMaximized)
        this->setWindowState(Qt::WindowMaximized);
    else
        this->resize(m_startWidth, m_startHeight);

    this->initActions();
    this->initGui();
    this->buildMenus();
    this->buildGui();

    Database db = Database();

    //Setting a custom context menu for the game list
    m_gameList->setContextMenuPolicy(Qt::CustomContextMenu);

    this->initEvents();

    this->loadGamesList();

    //connect(m_animation, SIGNAL(finished()), this, SLOT(onFadeFinished()));
    if(m_allowSlideshow)
    {
        connect(m_slideshow, SIGNAL(slideChanged(QPixmap*)), this, SLOT(onSlideChanged(QPixmap*)));
    }
}

LunarMainWindow::~LunarMainWindow()
{
    this->saveOptions();
    if(Database::openConnections() > 0) {
        qDebug() << Database::openConnections() << " db connections still open!" << endl;
        #ifdef LUNAR_DEBUG
            QMessageBox::warning(0, tr("Warning!"), tr("Warning!\n") + QString::number(Database::openConnections()) + " database connections are still open!");
        #endif
    }
    delete ui;
}

void LunarMainWindow::openAboutLunar()
{
    AboutLunar lunar(this);

    lunar.exec();
}

void LunarMainWindow::saveOptions()
{
    int posX    = this->x();
    int posY    = this->y();
    int width   = this->width();
    int height  = this->height();
    bool maximized = this->isMaximized();

    if(m_configurationFile.isWritable())
    {
        m_configurationFile.beginGroup("geometry");
        m_configurationFile.beginGroup("position");

        m_configurationFile.setValue("x", posX);
        m_configurationFile.setValue("y", posY);

        m_configurationFile.endGroup();
        m_configurationFile.beginGroup("scale");

        m_configurationFile.setValue("width", width);
        m_configurationFile.setValue("height", height);
        m_configurationFile.setValue("maximized", maximized);

        m_configurationFile.endGroup();
        m_configurationFile.endGroup();
    }
}

void LunarMainWindow::readOptions()
{
    m_configurationFile.beginGroup("geometry");
    m_configurationFile.beginGroup("position");

    m_startPosX = m_configurationFile.value("x", 100).toInt();
    m_startPosY = m_configurationFile.value("y", 100).toInt();

    m_configurationFile.endGroup();
    m_configurationFile.beginGroup("scale");

    m_startWidth = m_configurationFile.value("width", this->minimumWidth()).toUInt();
    m_startHeight = m_configurationFile.value("height", this->minimumHeight()).toUInt();
    m_isMaximized = m_configurationFile.value("maximized", false).toBool();

    m_configurationFile.endGroup();


    m_configurationFile.endGroup();

    m_configurationFile.beginGroup("Database API");

    m_apiType = m_configurationFile.value("API", "basic").toString();

    m_configurationFile.endGroup();

    m_configurationFile.beginGroup("Slideshow");
    m_cacheDuration = QDate::currentDate();

    m_allowSlideshow = m_configurationFile.value("activated", true).toBool();
    m_neverCleanCache = m_configurationFile.value("neverCleanCache", false).toBool();
    qint64 days = m_configurationFile.value("cacheDuration", 10).toInt();
    m_cacheDuration = m_cacheDuration.addDays(-days);

    m_configurationFile.endGroup();
}

void LunarMainWindow::initGui()
{
    this->setWindowTitle("Lunar - " + SOFTWARE_VERSION);
    this->setWindowIcon(QIcon(QPixmap("resources/imgs/logo.png")));
    //this->setWindowFlags( Qt::FramelessWindowHint );
}

void LunarMainWindow::buildGui()
{
    m_centralWidget = new QWidget;
    m_gameInfo = new GameInfoWidget(this);
    m_mainSplitter = new QSplitter;
    StyleSheetManager ssm(this);

    m_gameList = new ListView(this);
    if(!ssm.load("VerticalScrollBar"))
        qDebug() << "Unable to open VerticalScrollBar.css" << endl;

    ScrollBar *vScrollBar = new ScrollBar(this);
    ScrollBar *hScrollBar = new ScrollBar(this);

    vScrollBar->setStyleSheet(ssm.get());

    if(!ssm.load("HorizontalScrollBar"))
        qDebug() << "Unable to open HorizontalScrollBar.css" << endl;

    hScrollBar->setStyleSheet(ssm.get());
    m_gameList->setVerticalScrollBar(vScrollBar);
    m_gameList->setHorizontalScrollBar(hScrollBar);
    m_overviewStack = new QStackedWidget;

    m_overviewStack->addWidget(m_gameInfo);

    m_gameList->setMaximumWidth(280);
    m_gameList->resize(200, m_gameList->height());

    QHBoxLayout *hbox = new QHBoxLayout;

    m_mainSplitter->addWidget(m_gameList);
    m_mainSplitter->addWidget(m_overviewStack);

    hbox->addWidget(m_mainSplitter);
    m_centralWidget->setLayout(hbox);
    this->setCentralWidget(m_centralWidget);
}

void LunarMainWindow::buildMenus()
{
    m_menuBar = new MenuBar(this);
    setMenuBar(m_menuBar);

    m_mainMenu = new Menu("Lunar");
    m_gameMenu = new Menu(tr("Games"));
    m_emulation = new Menu(tr("Emulation"));
    m_toolsMenu = new Menu(tr("Tools"));
    m_helpMenu = new Menu("?");


    menuBar()->addMenu(m_mainMenu);
    menuBar()->addMenu(m_gameMenu);
    menuBar()->addMenu(m_emulation);
    menuBar()->addMenu(m_toolsMenu);
    menuBar()->addMenu(m_helpMenu);

    m_mainMenu->addAction(m_actionImport);
    m_mainMenu->addAction(m_actionExport);
    m_mainMenu->addSeparator();
    m_mainMenu->addAction(m_actionQuit);

    m_gameMenu->addAction(m_actionNewGame);
    m_gameMenu->addAction(m_actionEditGame);
    m_gameMenu->addAction(m_actionDeleteGame);
    m_gameMenu->addSeparator();

    m_gameMenu->addAction(m_actionLaunchGame);

    m_emulation->addAction(m_actionShowEmulators);

    m_toolsMenu->addAction(m_actionProcessManager);
    m_toolsMenu->addSeparator();
    m_toolsMenu->addAction(m_actionPlugins);
    m_toolsMenu->addAction(m_actionOptions);

    m_helpMenu->addAction(m_actionAboutLunar);
    m_helpMenu->addAction(m_actionAboutQt);
}

void LunarMainWindow::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);
    painter.save();
    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
    painter.restore();
    if(m_currentSlide != NULL)
    {
        //painter.drawPixmap(0, 0, m_currentSlide->width(), m_currentSlide->height(), *m_currentSlide);
        /*qDebug() << m_currentSlide->width() << " " << m_currentSlide->height() << endl;
        qDebug() << m_currentSlide->rect() << endl;*/
        //painter.save();
        painter.save();
        QRect painterPos = painter.viewport();
        QRect gPos = m_graphicSlide->rect();

        QRect imgPos(10, 10, m_currentSlide->width(), m_currentSlide->height());
        m_graphicSlide->render(&painter, painterPos.adjusted(-200, -200, 0, 0), gPos);
        //painter.restore();
        QColor frameColor(255, 255, 255, 150);
        qDebug() << m_currentSlide->width() << " " << m_currentSlide->height() << endl;
        if(m_currentSlide->width() > 450 || m_currentSlide->height() > 450)
        {
            QRadialGradient radialGradient(QPoint(600, 287), 387);
            radialGradient.setSpread(QGradient::PadSpread);
            radialGradient.setColorAt(0.0, frameColor);
            radialGradient.setColorAt(1.0, QColor(255, 255, 255));
            QBrush b(radialGradient);
            painter.setPen(Qt::NoPen);
            painter.setBrush(b);
        }
        else
        {
            QBrush b(frameColor);
            painter.setBrush(b);
        }

        painter.drawRect(0, 0, this->width(), this->height());

        painter.restore();

    }
    /*painter.save();
    bgColor.setAlpha(16);
    painter.setBrush(brush);
    painter.drawRect(0, 0, this->width(), this->height());

    painter.restore();*/
}

void LunarMainWindow::initActions()
{
    m_actionImport          = new QAction(tr("Import"), this);
    m_actionExport          = new QAction(tr("Export"), this);
    m_actionQuit            = new QAction(tr("Quit"), this);

    m_actionNewGame         = new QAction(tr("New game"), this);
    m_actionEditGame        = new QAction(tr("Edit game"), this);
    m_actionDeleteGame      = new QAction(tr("Delete game"), this);
    m_actionLaunchGame      = new QAction(tr("Launch game"), this);
    //m_actionUninstallGame   = new QAction(tr("Uninstall game"), this);
    m_actionShowEmulators = new QAction(tr("Show Emulators"), this);

    m_actionProcessManager  = new QAction(tr("Process manager"), this);
    m_actionPlugins         = new QAction(tr("Plugins"), this);
    m_actionOptions         = new QAction(tr("Options"), this);

    m_actionAboutLunar      = new QAction(tr("About Lunar"), this);
    m_actionAboutQt         = new QAction(tr("About Qt"), this);

    m_actionQuit->setIcon(QIcon(QPixmap("resources/imgs/quit.png")));

    m_actionNewGame->setIcon(QIcon(QPixmap("resources/imgs/add_game.png")));

    m_actionEditGame->setDisabled(true);
    m_actionEditGame->setIcon(QIcon(QPixmap("resources/imgs/edit_game.png")));

    m_actionDeleteGame->setDisabled(true);
    m_actionDeleteGame->setIcon(QIcon(QPixmap("resources/imgs/delete_game.png")));

    m_actionLaunchGame->setDisabled(true);
    m_actionLaunchGame->setIcon(QIcon(QPixmap("resources/imgs/launch.png")));
    m_actionLaunchGame->setToolTip(tr("Start selected game"));

    m_actionShowEmulators->setIcon(QIcon(QPixmap("resources/imgs/emulators.png")));

    /*m_actionUninstallGame->setDisabled(true);
    m_actionUninstallGame->setIcon(QIcon(QPixmap("resources/imgs/uninstall.png")));
    m_actionUninstallGame->setToolTip(tr("Uninstall selected game if the option was previously set"));*/

    m_actionImport->setIcon(QIcon(QPixmap("resources/imgs/import.png")));
    m_actionExport->setIcon(QIcon(QPixmap("resources/imgs/export.png")));

    m_actionOptions->setIcon(QIcon(QPixmap("resources/imgs/settings.png")));
    m_actionProcessManager->setIcon(QIcon(QPixmap("resources/imgs/process.png")));
    m_actionPlugins->setIcon(QIcon(QPixmap("resources/imgs/plugin.png")));

    QToolBar *toolbar = addToolBar("Main");
    toolbar->setMinimumHeight(48);
    toolbar->setIconSize(QSize(32, 32));
    toolbar->setMovable(false);

    toolbar->addAction(m_actionNewGame);
    toolbar->addAction(m_actionEditGame);
    toolbar->addAction(m_actionDeleteGame);

    toolbar->addSeparator();
    toolbar->addAction(m_actionShowEmulators);
    /*toolbar->addAction(m_actionUninstallGame);*/

    toolbar->addSeparator();
    toolbar->addAction(m_actionProcessManager);
    toolbar->addSeparator();

    toolbar->addAction(m_actionLaunchGame);
}

void LunarMainWindow::initEvents()
{
    connect(m_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_actionAboutLunar, SIGNAL(triggered()), this, SLOT(openAboutLunar()));
    connect(m_actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(m_actionNewGame, SIGNAL(triggered()), this, SLOT(addGame()));
    connect(m_actionEditGame, SIGNAL(triggered()), this, SLOT(onEditGameButtonClick()));
    connect(m_actionDeleteGame, SIGNAL(triggered()), this, SLOT(onDeleteGameButtonClick()));
    connect(m_actionLaunchGame, SIGNAL(triggered()), this, SLOT(onLaunchGameButtonClick()));
    //connect(m_actionUninstallGame, SIGNAL(triggered()), this, SLOT(onUninstallButtonClick()));
    connect(m_actionProcessManager, SIGNAL(triggered()), this, SLOT(openProcessManagerWindow()));
    connect(m_actionOptions, SIGNAL(triggered()), this, SLOT(openOptionDialog()));
    connect(m_gameList, SIGNAL(clicked(QModelIndex)), this, SLOT(onClickItemList(QModelIndex)));
    connect(m_gameList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(openContextualMenu(QPoint)));
    connect(m_gameList, SIGNAL(activated(QModelIndex)), this, SLOT(onLaunchGameButtonClick()));
    connect(m_actionExport, SIGNAL(triggered()), this, SLOT(openWindowExport()));
    connect(m_actionImport, SIGNAL(triggered()), this, SLOT(openWindowImport()));
    connect(m_actionShowEmulators, SIGNAL(triggered()), this, SLOT(openEmulatorsWindow()));
    connect(m_actionPlugins, SIGNAL(triggered()), this, SLOT(onPluginsActionTriggered()));
}

void LunarMainWindow::addGame()
{
    m_windowAddGames = new WindowModifGame(this);
    m_windowAddGames->setWindowTitle(tr("Add a new game"));
    m_windowAddGames->exec();

    m_windowAddGames->deleteLater();
    this->loadGamesList();
}

void LunarMainWindow::onClickItemList(QModelIndex mi)
{
    if(mi.isValid())
    {
        m_actionEditGame->setEnabled(true);
        m_actionDeleteGame->setEnabled(true);
        m_actionLaunchGame->setEnabled(true);

        /*if(mi.sibling(mi.row(), 5).data().toString() != "")
            m_actionUninstallGame->setEnabled(true);
        else
            m_actionUninstallGame->setEnabled(false);*/

        displayGameInfos(mi);
    }
}

void LunarMainWindow::onUninstallButtonClick()
{
    QModelIndex index = m_gameList->currentIndex();
    QString gameName = index.data().toString();

    int response = QMessageBox::question(this, tr("Uninstall confirmation"), tr("You are about to uninstall") + " <b>" + gameName + "</b>.<br />Do you want to continue ?", QMessageBox::Yes | QMessageBox::No);

    if(response == QMessageBox::Yes)
    {
        QString uninstallPath = index.sibling(index.row(), 5).data().toString();
        Process::startDetached(uninstallPath, QStringList());
    }
}

void LunarMainWindow::uninstallGame(QString path)
{
    Process *process = new Process;

    process->start(path, QStringList());

    connect(process, SIGNAL(finished(int)), this, SLOT(onUninstallProcessFinished(int)));
}

void LunarMainWindow::onUninstallProcessFinished(int code)
{
    if(!m_games.deleteGame(m_currentUninstallingGameId))
    {
        QMessageBox::critical(this, tr("Error"), tr("An error has occured during the game deleting"));
    }
    else
    {
        this->loadGamesList();
        m_gameInfo->clearInfos();
    }
}

void LunarMainWindow::onPluginsActionTriggered()
{
    PluginsWindow pm(this);
    pm.exec();
}

void LunarMainWindow::onEditGameButtonClick()
{
    //First we get the modelIndex of the cell we clicked on (the game name)
    QModelIndex mi = m_gameList->currentIndex();
    //Then we get the modelIndex of the sibling cell width contains the database id of the game
    QModelIndex id = mi.sibling(mi.row(), 1);

    m_windowAddGames = new WindowModifGame(this, id.data().toInt());
    m_windowAddGames->setWindowTitle(tr("Edit a game"));
    m_windowAddGames->exec();

    m_windowAddGames->deleteLater();
    this->loadGamesList();
}

void LunarMainWindow::onDeleteGameButtonClick()
{
    bool deleteItemLater = false;
    //First we get the modelIndex of the cell we clicked on (the game name)
    QModelIndex mi = m_gameList->currentIndex();
    //Then we get the modelIndex of the sibling cell width contains the database id of the game
    QModelIndex id = mi.sibling(mi.row(), 1);
    QString uninstallPath = mi.sibling(mi.row(), 5).data().toString();

    int rep = QMessageBox::question(this, tr("Delete confirmation"), QString(tr("Do you want to delete") + " <b>" + mi.data().toString() + "</b> ?"), QMessageBox::Yes | QMessageBox::No);

    if(rep == QMessageBox::Yes)
    {
        int gameId = id.data().toInt();

        if(uninstallPath != "")
        {
            int response = QMessageBox::question(this, tr("Uninstall game"), tr("Do you also want to uninstall") + " <b>" + mi.data().toString() + "</b> " + tr("from your computer") + "?", QMessageBox::Yes | QMessageBox::No);

            if(response == QMessageBox::Yes)
            {
                deleteItemLater = true;
                m_currentUninstallingGameId = gameId;
                uninstallGame(uninstallPath);
            }
        }

        if(!deleteItemLater)
        {
            if(!m_games.deleteGame(gameId))
            {
                QMessageBox::critical(this, tr("Error"), tr("An error has occured during the game deleting"));
            }
            else
            {
                this->loadGamesList();
                m_gameInfo->clearInfos();
            }
        }
    }
}

void LunarMainWindow::onLaunchGameButtonClick()
{
    //First we get the modelIndex of the cell we clicked on (the game name)
    QModelIndex mi = m_gameList->currentIndex();
    if(mi.isValid())
    {
        m_processManager->startGame(mi);
    }
}

void LunarMainWindow::openContextualMenu(QPoint p)
{
     //First we get the modelIndex of the cell we clicked on (the game name)
     QModelIndex mi = m_gameList->currentIndex();

     //Invoking itemClick event
     onClickItemList(mi);

     QPoint a = m_gameList->mapToGlobal(p);
     ContextualMenu cMenu(this);

     cMenu.addAction(m_actionLaunchGame);

     cMenu.addSeparator();

     cMenu.addAction(m_actionNewGame);
     cMenu.addAction(m_actionEditGame);
     cMenu.addAction(m_actionDeleteGame);

     /*cMenu.addSeparator();

     cMenu.addAction(m_actionUninstallGame);*/

     cMenu.exec(a);
}

void LunarMainWindow::loadGamesList()
{
    QSqlQueryModel *games = m_games.findAll();
    //Removing all columns except names
    /*games->removeColumn(0);
    games->removeColumn(2);
    games->removeColumn(3);
    games->removeColumn(4);
    games->removeColumn(5);
    games->removeColumn(6);*/

    m_gameList->setModel(games);
}

void LunarMainWindow::openProcessManagerWindow()
{
    m_windowProcessManager = new WindowProcessManager(this);

    m_windowProcessManager->show();

    connect(m_windowProcessManager, SIGNAL(finished(int)), this, SLOT(deleteProcessManagerWindow()));
}

void LunarMainWindow::deleteProcessManagerWindow()
{
    if(!m_windowProcessManager->disconnect(SIGNAL(finished(int))))
    {
        cout << "Signal finished() non deconnecté" << endl;
    }

    m_windowProcessManager->deleteLater();
}

void LunarMainWindow::openOptionDialog()
{
    m_windowOptions = new WindowOptions(this);
    m_windowOptions->exec();

    m_windowOptions->deleteLater();
}

void LunarMainWindow::closeEvent(QCloseEvent *ce)
{
    int nbGame = m_processManager->getList().size();
    QString strGame = "";

    if(nbGame > 0)
    {
        if(nbGame > 0 && nbGame != 1)
        {
            strGame = tr("games are");
        }
        else
        {
            strGame = tr("game is");
        }

        int reponse = QMessageBox::question(this, tr("Confirmation"), QString(QString::number(nbGame) + " " + strGame + tr(" still running.<br />Stopping Lunar will stop the save synchronisation.<br />Do you really want to quit ?")), QMessageBox::Yes | QMessageBox::No);

        if(reponse == QMessageBox::Yes)
        {
            ce->accept();
        }
        else
        {
            ce->ignore();
        }
    }
}

void LunarMainWindow::displayGameInfos(QModelIndex mi)
{
    m_gameInfo->clearInfos();
    m_graphicScene->clear();
    m_currentSlide = NULL;
    this->repaint();
    m_gameInfo->setGame(mi, m_apiType);
}

void LunarMainWindow::onSlideChanged(QPixmap *pix)
{
    //Calculating key positions
    /*int slideDuration = m_slideshow->getInterval();

    qreal firstKey = 250.f / (slideDuration - (350.f + 250.f));
    qreal secondKey = (qreal)1.f - (350.f / (slideDuration - (350.f + 250.f)));
    qDebug() << firstKey << " " << secondKey << endl;*/
    //Setting animation

    //m_currentSlide->setPixmap(pix);
    m_currentSlide = pix;
    m_graphicSlide->setGeometry(this->rect());
    //m_graphicScene->addItem(m_currentSlide);
    m_graphicScene->clear();
    m_graphicScene->addPixmap(*m_currentSlide);
    /*m_animation->setKeyValueAt(firstKey, 20);
    m_animation->setKeyValueAt(secondKey, 20);
    m_animation->setKeyValueAt(1, 255);*/

    //m_animation->start();

    this->repaint();
}

void LunarMainWindow::onFadeFinished()
{
    qDebug() << "plop" << endl;
    //m_setTargetObject(0);
    m_graphicScene->clear();
    m_currentSlide = NULL;
}

void LunarMainWindow::processCacheCleaning()
{
    QDir dir("cache/");
    QFileInfoList listDir = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    int i = 0;
    for(QFileInfoList::iterator it = listDir.begin(); it != listDir.end(); ++it)
    {
        if(it->lastModified().date() <= m_cacheDuration)
        {
            qDebug() << it->lastModified().date() << " " << m_cacheDuration << endl;
            QFile file("cache/" + it->fileName());
            file.remove();
            i++;
        }
    }
    qDebug() << "Cleaned " << i << " expired elements from the cache" << endl;
}

void LunarMainWindow::openWindowExport()
{
    WindowExport we(this);
    we.exec();
}

void LunarMainWindow::openWindowImport()
{
    WindowImport wi(this);
    wi.exec();

    this->loadGamesList();
}

void LunarMainWindow::openEmulatorsWindow()
{
    EmulatorsWindow ew(this);
    ew.exec();
}
