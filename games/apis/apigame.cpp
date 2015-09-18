#include "apigame.h"

ApiGame::ApiGame() :
    m_gameId(0),
    m_gameName(""),
    m_gameDevelopper(""),
    m_gamePublisher(""),
    m_overview(""),
    m_esrb(""),
    m_releaseDate(QDate()),
    m_rating(-1),
    m_players(-1),
    m_gameGenres(QStringList()),
    m_pictures(QStringList())
{
}

ApiGame::ApiGame(const ApiGame &other)
{
    m_gameId            = other.m_gameId;
    m_gameName          = other.m_gameName;
    m_gameDevelopper    = other.m_gameDevelopper;
    m_gamePublisher     = other.m_gamePublisher;
    m_overview          = other.m_overview;
    m_esrb              = other.m_esrb;
    m_releaseDate       = other.m_releaseDate;
    m_rating            = other.m_rating;
    m_players           = other.m_players;
    m_gameGenres        = other.m_gameGenres;
    m_pictures          = other.m_pictures;
}

ApiGame::~ApiGame()
{}

void ApiGame::clear()
{
    m_gameId = 0;
    m_gameName = "";
    m_gameDevelopper = "";
    m_gamePublisher = "";
    m_overview = "";
    m_esrb = "";
    m_releaseDate = QDate();
    m_rating = 0;
    m_players = 0;
    m_gameGenres.clear();
    m_pictures.clear();
}

int ApiGame::getId() const
{
    return m_gameId;
}

void ApiGame::setId(int gi)
{
    m_gameId = gi;
}

QString ApiGame::getName() const
{
    return m_gameName;
}

void ApiGame::setName(QString name)
{
    m_gameName = name;
}


QStringList ApiGame::getGenresList() const
{
    return m_gameGenres;
}

void ApiGame::setGenresList(QStringList genreList)
{
    m_gameGenres = genreList;
}

QString ApiGame::getDevelopper() const
{
    return m_gameDevelopper;
}

void ApiGame::setDevelopper(QString gd)
{
    m_gameDevelopper = gd;
}

QString ApiGame::getPublisher() const
{
    return m_gamePublisher;
}

void ApiGame::setPublisher(QString gp)
{
    m_gamePublisher = gp;
}

QDate ApiGame::getReleaseDate() const
{
    return m_releaseDate;
}

void ApiGame::setReleaseDate(QDate rd)
{
    m_releaseDate = rd;
}

float ApiGame::getRating() const
{
    return m_rating;
}

void ApiGame::setRating(float rate)
{
    m_rating = rate;
}

QStringList ApiGame::getPicturesList() const
{
    return m_pictures;
}

void ApiGame::setPicturesList(QStringList pl)
{
    m_pictures = pl;
}

QString ApiGame::getOverview() const
{
    return m_overview;
}

void ApiGame::setOverview(QString ov)
{
    m_overview = ov;
}

QString ApiGame::getEsrb() const
{
    return m_esrb;
}

void ApiGame::setEsrb(QString esrb)
{
    m_esrb = esrb;
}

int ApiGame::getPlayersNumber() const
{
    return m_players;
}

void ApiGame::setPlayersNumber(int playerNumber)
{
    m_players = playerNumber;
}
