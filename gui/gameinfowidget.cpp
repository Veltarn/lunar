#include "gameinfowidget.h"

using namespace std;

GameInfoWidget::GameInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    QSettings configurationFile(CONFIGURATION_FILE, QSettings::IniFormat);

    configurationFile.beginGroup("Slideshow");
    m_allowSlideshow = configurationFile.value("activated", false).toBool();
    configurationFile.endGroup();

    m_processManager = ProcessManager::getInstance();
    m_dbGameApi = ApiSelector::getUsedApi();

    m_textStyleSheet = "background-color: rgba(255, 255, 255, 150); padding: 3px; color: #676e76";
    m_titleStyleSheet = "background-color: rgb(255, 255, 255, 150); color: #899198";

    this->buildGui();
    this->initEvents();

    m_slideshow = Slideshow::getInstance();
    m_slideshow->initSlideshow();
}

void GameInfoWidget::buildGui()
{
    m_mainLayout    = new QGridLayout;
    m_infoLayout    = new QHBoxLayout;
    m_gameInfos     = new QGridLayout;

    m_gameTitle = new QLabel(this);
    m_gameTitle->setFixedHeight(30);

    m_gameTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_mainLayout->addWidget(m_gameTitle, 0, 0);
    m_playedTime = new QLabel(this);
    m_emulatorUsedLabel = new QLabel(this);
    m_lastPlayedTime = new QLabel(this);

    if(m_dbGameApi && m_dbGameApi->isOnlineApi())
    {
        m_gameRate = new QLabel(this);
        m_publisherLabel = new QLabel(this);
        m_esrbLabel = new QLabel(this);
        m_overviewLabel = new QLabel(this);
        m_genreGame = new QLabel(this);
        m_developerLabel = new QLabel(this);

        QSpacerItem *overviewSpacer = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);

        QFont rateFont;
        QFont gameInfoFont;

        rateFont.setPointSize(10);
        gameInfoFont.setPointSize(12);

        m_gameRate->setStyleSheet(m_textStyleSheet);
        m_developerLabel->setStyleSheet(m_textStyleSheet);
        m_publisherLabel->setStyleSheet(m_textStyleSheet);
        m_esrbLabel->setStyleSheet(m_textStyleSheet);
        m_genreGame->setStyleSheet(m_textStyleSheet);
        m_overviewLabel->setStyleSheet(m_textStyleSheet);

        m_gameRate->setFont(rateFont);

        m_developerLabel->setFont(gameInfoFont);
        m_publisherLabel->setFont(gameInfoFont);
        m_esrbLabel->setFont(gameInfoFont);
        m_genreGame->setFont(gameInfoFont);
        m_overviewLabel->setFont(gameInfoFont);
        m_gameTitle->setFixedHeight(30);

        m_gameTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        m_genreGame->setFixedHeight(30);
        m_genreGame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        m_mainLayout->addWidget(m_gameRate, 0, 1, 1, 7, Qt::AlignRight);
        m_mainLayout->addWidget(m_genreGame, 1, 0, 1, 5);

        //Layouting game info widgets
        m_gameInfos->addWidget(m_developerLabel, 0, 0, 1, 2);
        m_gameInfos->addWidget(m_publisherLabel, 1, 0, 1, 2);
        m_gameInfos->addWidget(m_esrbLabel, 2, 0, 1, 2);
        m_gameInfos->addItem(overviewSpacer, 3, 0);
        m_gameInfos->addWidget(m_overviewLabel, 4, 0, 5, 6, Qt::AlignTop);


        m_developerLabel->setFixedHeight(30);
        m_developerLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        m_publisherLabel->setFixedHeight(30);
        m_publisherLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        m_esrbLabel->setFixedHeight(30);
        m_esrbLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        //m_overviewLabel->s;
        m_overviewLabel->setWordWrap(true);
    }

    m_savesWidget = new QWidget;

    m_playButton = new Button(tr("Play"), this);
    m_playButton->hide();

    QFont titleFont;

    titleFont.setPointSize(24);
    m_gameTitle->setStyleSheet(m_titleStyleSheet);
    m_gameTitle->setFont(titleFont);
    //m_gameTitle->setStyleSheet("QLabel { background-color: #0982ef; }");

    QFont timeFont;
    timeFont.setPointSize(9);
    m_playedTime->setFont(timeFont);
    m_playedTime->setStyleSheet(m_textStyleSheet);
    m_emulatorUsedLabel->setStyleSheet(m_textStyleSheet);

    /*m_infoLayout->addWidget(m_playedTime, 1);
    m_infoLayout->addWidget(m_lastPlayedTime, 1);
    m_infoLayout->addWidget(m_playButton, 1, Qt::AlignRight);*/
    QSpacerItem *spaceInfoGame = new QSpacerItem(50, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_mainLayout->addItem(spaceInfoGame, 1, 0);
    m_mainLayout->addWidget(m_playButton, 2, 3, -1, -1, Qt::AlignTop|Qt::AlignRight);
    m_mainLayout->addWidget(m_playedTime, 2, 0);
    m_mainLayout->addWidget(m_lastPlayedTime, 2, 1);
    m_mainLayout->addWidget(m_emulatorUsedLabel, 3, 0, Qt::AlignTop);

    m_playedTime->setFixedHeight(22);
    m_playedTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_playButton->setFixedHeight(22);
    m_playButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //m_infoLayout->setContentsMargins(0, 25, 0, 0);
    //m_infoLayout->setSizeConstraint(QLayout::SetFixedSize);

    //m_mainLayout->addLayout(m_infoLayout, 2, 0, 1, 8);

    QSpacerItem *spacer = new QSpacerItem(50, 50, QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_mainLayout->addItem(spacer, 4, 0, 1, 9);

    m_mainLayout->addLayout(m_gameInfos, 5, 0, 1, 9);

    m_mainLayout->addWidget(m_savesWidget);

    this->setLayout(m_mainLayout);
}

void GameInfoWidget::clearInfos()
{
    m_gameTitle->setText("");
    m_playedTime->setText("");
    m_lastPlayedTime->setText("");

    if(m_dbGameApi && m_dbGameApi->isOnlineApi())
    {
        m_gameRate->setText("");
        m_developerLabel->setText("");
        m_publisherLabel->setText("");
        m_esrbLabel->setText("");
        m_genreGame->setText("");
        m_overviewLabel->setText("");
    }

    m_playButton->hide();

    //m_savesWidget->clearSaves();
    m_currentIndex = QModelIndex();
    this->repaint();
}

void GameInfoWidget::setGame(QModelIndex gameIndex, QString apiName)
{
    m_slideshow->stop();
    m_dbGameApi = ApiSelector::getUsedApi(); //verifying if the user switched to another api

    m_currentIndex = gameIndex;
    QModelIndex id = m_currentIndex.sibling(m_currentIndex.row(), 1);

    QSqlRecord record = m_games.find(id.data().toInt(), apiName);

    if(!record.isEmpty())
    {
        if(!record.value("remote_game_id").isNull())
        {
            getRemoteGameInfos(record.value("remote_game_id").toInt());
        }

        QString title = record.value("name").toString();
        int playedTime = record.value("playedtime").toInt();
        QString formatedTime = this->formatTimeToString(playedTime);
        //int lastPlayedTime = record.value("lasttimeplayed").toInt();

        int emulatorUsed = record.value("emulator_used").toInt();

        if(emulatorUsed != -1) {
            Emulators emulators;
            Emulator emu = emulators.getEmulator(emulatorUsed);

            QString message = QString("<b>" + tr("Emulator") + "</b>: " + emu.emulatorName());
            m_emulatorUsedLabel->setText(message);
        } else {
            m_emulatorUsedLabel->setText("");
        }

       m_gameTitle->setText(title);
       m_playedTime->setText("<b>Game time</b>: " + formatedTime);
       m_playButton->show();
    }
}

void GameInfoWidget::getRemoteGameInfos(int remoteGameId)
{
    if(m_dbGameApi && m_dbGameApi->isOnlineApi())
    {
        connect(m_dbGameApi, SIGNAL(finishedGet(ApiGame)), this, SLOT(onFinishedGet(ApiGame)));
        m_dbGameApi->get(remoteGameId);
    }
}

void GameInfoWidget::msToRegularTime(int *h, int *m, int *s, int ms)
{
    *s = ms / 1000;

    *m = *s / 60;
    *s %= 60;

    *h = *m / 60;
    *m %= 60;
}

QString GameInfoWidget::formatTimeToString(int ms)
{
    QString formattedTime = "";
    int hour = 0;
    int minute = 0;
    int seconds = 0;

    this->msToRegularTime(&hour, &minute, &seconds, ms);

    if(minute < 1 && hour < 1)
    {
        formattedTime = tr("Less than one minute");
    }
    else if(minute >= 1 && hour < 1)
    {
        QString strMinute = minute > 1 ? "minutes" : "minute";
        formattedTime = QString(QString::number(minute) + " " + strMinute + " " + tr("played"));
    }
    else
    {
        QString strHour = hour > 1 ? tr("hours") : tr("hour");
        formattedTime = QString(QString::number(hour) + " " + strHour + " " + tr("played"));
    }

    return formattedTime;
}

void GameInfoWidget::initEvents()
{
    connect(m_playButton, SIGNAL(clicked()), this, SLOT(startGame()));
}

void GameInfoWidget::startGame()
{
    if(m_currentIndex.isValid())
    {
        m_processManager->startGame(m_currentIndex);
    }
}

void GameInfoWidget::onFinishedGet(ApiGame game)
{
    //Calculating percentage of the rate
    int gameRating = game.getRating();
    int maxApiRating = m_dbGameApi->getMaxRate();

    float pcRate = (gameRating * 100) / maxApiRating;
    QString rateColorStyle = "";

    if(gameRating != -2)
    {
        if(pcRate <= 25)
        {
            rateColorStyle = "color: #dd2d11";
        }
        else if(pcRate <= 50)
        {
            rateColorStyle = "color: #eda127";
        }
        else if(pcRate <= 75)
        {
            rateColorStyle = "color: #a1ec1c";
        }
        else if(pcRate <= 100)
        {
            rateColorStyle = "color: #0da91e";
        }

        m_gameRate->setText("<b>" + QString::number(game.getRating()) + " / " + QString::number(m_dbGameApi->getMaxRate()) + "</b>");
        m_gameRate->setStyleSheet(rateColorStyle);
    }
    else
    {
        m_gameRate->setText("<b>N/A</b>");
        m_gameRate->setStyleSheet("color: #0fa0c0");
    }

    m_developerLabel->setText("<b>" + tr("Developpers") + ":</b>  " + game.getDevelopper());
    if(!game.getPublisher().isEmpty())
        m_publisherLabel->setText("<b>" + tr("Publisher") + ":</b>  " + game.getPublisher());

    m_esrbLabel->setText("<b>" + tr("ESRB") + ":</b>  " + game.getEsrb());
    m_overviewLabel->setText("<b>" + tr("Summary") + "</b>:<p>" + game.getOverview() + "</p>");

    QString genres = "";
    for(int i = 0; i < game.getGenresList().size(); ++i)
    {
        if(i > 0)
        {
            genres += " / ";
        }

        genres += game.getGenresList().at(i);
    }

    m_genreGame->setText(genres);

    m_dbGameApi->disconnect(SIGNAL(finishedGet(ApiGame)));

    if(game.getPicturesList().size() > 0 && m_allowSlideshow)
    {
        m_slideshow->setPicturesList(game.getPicturesList());
        m_slideshow->start();
    }
}
