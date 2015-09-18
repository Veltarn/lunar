#include "slideshow.h"

void Slideshow::initSlideshow()
{
    m_configurationFile = new QSettings(CONFIGURATION_FILE, QSettings::IniFormat);

    m_configurationFile->beginGroup("Slideshow");

    m_delayBetweenSlides = m_configurationFile->value("delay", 5).toInt() * 1000; //Convert secs to msecs
    m_cacheDuration = m_configurationFile->value("cacheDuration", 10).toInt();
    m_configurationFile->endGroup();

    m_pictureDownloader = new PictureDownloader;
    //m_slidesCache.clear();
    m_currentSlide = NULL;
    m_currentIndex = 0;
    //m_delayBetweenSlides = 5000;
    m_playing = false;

    m_slideshowTimer.setInterval(m_delayBetweenSlides);
    QDir cachePath;

    if(!cachePath.exists("cache/"))
    {
        cachePath.mkdir("cache/");
    }
}


void Slideshow::start()
{
    if(!m_playing)
    {
        connect(&m_slideshowTimer, SIGNAL(timeout()), this, SLOT(onTick()));
        m_slideshowTimer.start();
        m_playing = true;
    }

}

void Slideshow::stop()
{
    if(m_playing)
    {        
        m_currentIndex = 0;
        m_playing = false;
        m_slideshowTimer.stop();
        m_slideshowTimer.disconnect(SIGNAL(timeout()));
    }
}

void Slideshow::setInterval(int msec)
{
    m_slideshowTimer.setInterval(msec);
    m_delayBetweenSlides = msec;
}

int Slideshow::getInterval() const
{
    return m_delayBetweenSlides;
}

void Slideshow::onTick()
{
    if(m_playing)
    {
        //Verifying if the index has reached the size of the paths list
        if(m_currentIndex == m_picturesPathList.size())
        {
            //Setting m_currentIndex to 0 to avoid buffer overflow
            m_currentIndex = 0;
        }

        /*
         *  Stop timer before processing of new picture
         *  In the case that the picture have to be
         *  downloaded, download time could be superior to "m_delayBetweenSlides"
         *  and another onTick slot will be called with the same risks...
         */
        m_slideshowTimer.stop();

        m_currentSlide = processSliding();

        //if the slide doesn't NULL, we restart the slideshow
        // if the slide is NULL, the program will wait for downloadFinished signal and then, restart the slideshow
        if(m_currentSlide != NULL)
        {
            //Restarting timer
            m_slideshowTimer.start();
                qDebug() << m_currentSlide->width() << endl;
            //cleanOldPictures();
            m_currentIndex++;
            emit slideChanged(m_currentSlide);
        }
    }
}

void Slideshow::setPicturesList(QStringList picturesList)
{
    m_picturesPathList = picturesList;
}

QPixmap *Slideshow::processSliding()
{
    if(m_currentSlide != NULL)
    {
        delete m_currentSlide;
        m_currentSlide = NULL;
    }
    QString currentPath = m_picturesPathList.at(m_currentIndex);
    QFileInfo urlInfo(currentPath);
    QFileInfo localFileInfo("cache/" + urlInfo.fileName());
/*
    //If the picture is in RAM cache
    if(m_slidesCache.find(urlInfo.fileName(), m_currentSlide))
    {
        qDebug() << "Access to RAM cache" << endl;
        return m_currentSlide;
    }
    else */if(localFileInfo.exists()) //Else if the picture is in local disk cache
    {
        qDebug() << "Access to disk cache" << endl;
        QPixmap *p = new QPixmap;
        p->load("cache/" + urlInfo.fileName());

        //Caching new pixmap in the RAM
        //m_slidesCache.insert(urlInfo.fileName(), *p);

        return p;
    }
    else //If previous conditions has failed, we download the picture, and make disk and ram cache
    {
        qDebug() << "Downloading picture" << endl;
        m_pictureDownloader->getPicture(currentPath);
        connect(m_pictureDownloader, SIGNAL(downloadFinished(QPixmap*)), this, SLOT(onPictureDownloadFinished(QPixmap*)));
        return NULL; //Returns NULL pointer
    }

    // ... dangeureux
    return new QPixmap;
}

void Slideshow::cleanOldPictures()
{
    /*int lastIndex = m_currentIndex - 1;
    if(lastIndex < 0)
        lastIndex = m_picturesPathList.size() - 1;

    QString urlLastPic = m_picturesPathList.at(lastIndex);
    qDebug() << m_picturesPathList.at(m_currentIndex) << " " << urlLastPic << endl;
    QFileInfo lastPic(urlLastPic);
    QPixmap *p = NULL;
    if(m_slidesCache.find(lastPic.fileName(), p))
    {
        qDebug() << "test" << endl;
        m_slidesCache.remove(lastPic.fileName());
        delete p;
    }*/
}

void Slideshow::onPictureDownloadFinished(QPixmap *pix)
{
    if(pix)
    {
        QString currentPath = m_picturesPathList.at(m_currentIndex);
        QFileInfo pathInfo(currentPath);

        m_currentSlide = pix;
        //Caching pixmap on memory
        //m_slidesCache.insert(pathInfo.fileName(), *m_currentSlide);
        if(m_cacheDuration > 0)
            m_currentSlide->save("cache/" + pathInfo.fileName());

        m_slideshowTimer.start();
        m_currentIndex++;
        m_pictureDownloader->disconnect(SIGNAL(downloadFinished(QPixmap *)));
        //cleanOldPictures();
        emit slideChanged(m_currentSlide);
    }
}

void Slideshow::onDestroyed()
{

}
