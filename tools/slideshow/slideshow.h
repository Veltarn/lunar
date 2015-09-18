#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <iostream>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QPixmapCache>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QSettings>
#include "../../constants.h"
#include "picturedownloader.h"
#include "../singleton.h"

class Slideshow : public Singleton<Slideshow>
{
    Q_OBJECT
public:
    void initSlideshow();
    void setPicturesList(QStringList picturesList);

    void start();
    void stop();
    void setInterval(int msec);
    int getInterval() const;

public slots:
    void onTick();
    void onPictureDownloadFinished(QPixmap *pix);

signals:
    void slideChanged(QPixmap *);
protected:
    virtual void onDestroyed();
private:
    QPixmap *processSliding();
    void cleanOldPictures();
private:
    PictureDownloader *m_pictureDownloader;
    QStringList m_picturesPathList;
    //QPixmapCache m_slidesCache;
    QPixmap *m_currentSlide;
    int m_currentIndex;
    int m_delayBetweenSlides;
    int m_cacheDuration;
    QTimer m_slideshowTimer;
    bool m_playing;

    QSettings *m_configurationFile;
};

#endif // SLIDESHOW_H
