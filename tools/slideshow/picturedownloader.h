#ifndef PICTUREDOWNLOADER_H
#define PICTUREDOWNLOADER_H

#include <iostream>
#include <QDebug>
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QPixmap>
#include <QString>

class PictureDownloader : public QObject
{
    Q_OBJECT
public:
    PictureDownloader();

    void getPicture(QString addr);
public slots:
    void slotReadyRead();
    void slotFinished();
    void slotNetworkReplyError(QNetworkReply::NetworkError error);
signals:
    void downloadFinished(QPixmap *);
private:
    QNetworkAccessManager *m_network;
    QNetworkReply *m_networkReply;
    QByteArray m_buffer;
};

#endif // PICTUREDOWNLOADER_H
