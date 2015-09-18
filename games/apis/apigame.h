#ifndef APIGAME_H
#define APIGAME_H

#include <QList>
#include <QDate>
#include <QString>
#include <QStringList>


class ApiGame
{
public:
    ApiGame();
    ApiGame(const ApiGame &other);
    ~ApiGame();

    //Clear all data
    void clear();

    int getId() const;
    void setId(int gi);

    QString getName() const;
    void setName(QString name);

    QStringList getGenresList() const;
    void setGenresList(QStringList genreList);

    QString getDevelopper() const;
    void setDevelopper(QString gd);

    QString getPublisher() const;
    void setPublisher(QString gp);

    QDate getReleaseDate() const;
    void setReleaseDate(QDate rd);

    float getRating() const;
    void setRating(float rate);

    QStringList getPicturesList() const;
    void setPicturesList(QStringList pl);

    QString getOverview() const;
    void setOverview(QString ov);

    QString getEsrb() const;
    void setEsrb(QString esrb);

    int getPlayersNumber() const;
    void setPlayersNumber(int playerNumber);

private:
    int             m_gameId;
    QString         m_gameName;
    QString         m_gameDevelopper;
    QString         m_gamePublisher;
    QString         m_overview;
    QString         m_esrb;
    QDate           m_releaseDate;
    float           m_rating;
    int             m_players;
    QStringList     m_gameGenres;
    QStringList     m_pictures;
};
typedef QList<ApiGame> ApiGameList;
#endif // APIGAME_H
