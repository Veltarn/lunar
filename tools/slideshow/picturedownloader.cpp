#include "picturedownloader.h"

PictureDownloader::PictureDownloader()
{
    m_network = new QNetworkAccessManager(this);
    m_networkReply = NULL;
}

void PictureDownloader::getPicture(QString addr)
{
    QUrl url(addr);

    QNetworkRequest request(url);

    m_buffer.clear();

    m_networkReply = m_network->get(request);

    connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_networkReply, SIGNAL(finished()), this, SLOT(slotFinished()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotNetworkReplyError(QNetworkReply::NetworkError)));
}

void PictureDownloader::slotReadyRead()
{
    m_buffer.append(m_networkReply->readAll());
}

void PictureDownloader::slotFinished()
{
    m_networkReply->deleteLater();
    m_networkReply = NULL;

    QPixmap *picture = new QPixmap;
    picture->loadFromData(m_buffer);

    emit downloadFinished(picture);
    m_buffer.clear();
}

void PictureDownloader::slotNetworkReplyError(QNetworkReply::NetworkError error)
{
    if(error != QNetworkReply::NoError)
    {
        QString errorMsg = "";
        switch(error)
        {
            case QNetworkReply::ConnectionRefusedError:
                errorMsg = tr("Server refused connection");
            break;

            case QNetworkReply::RemoteHostClosedError:
                errorMsg = tr("Connection to the server has been interrupted during transfer operations");
            break;

            case QNetworkReply::HostNotFoundError:
                errorMsg = tr("Server not found");
            break;

            case QNetworkReply::TimeoutError:
                errorMsg = tr("Timeout error");
            break;

            case QNetworkReply::SslHandshakeFailedError:
                errorMsg = tr("Unable to establish SSL link");
            break;

            case QNetworkReply::TemporaryNetworkFailureError:
                errorMsg = tr("Network connection has been lost");
            break;

            case QNetworkReply::ProxyConnectionRefusedError:
                errorMsg = tr("Proxy server has refused the connection");
            break;

            case QNetworkReply::ProxyConnectionClosedError:
                errorMsg = tr("Connection to proxy server has been lost before the end of the transfer operations");
            break;

            case QNetworkReply::ProxyNotFoundError:
                errorMsg = tr("Proxy not found");
            break;

            case QNetworkReply::ProxyTimeoutError:
                errorMsg = tr("Unable to join the proxy (Timeout error)");
            break;

            case QNetworkReply::ProxyAuthenticationRequiredError:
                errorMsg = tr("Proxy needs an authentication");
            break;

            case QNetworkReply::ContentAccessDenied:
                errorMsg = tr("Access denied");
            break;

            case QNetworkReply::ContentNotFoundError:
                errorMsg = tr("File not found");
            break;

            case QNetworkReply::AuthenticationRequiredError:
                errorMsg = tr("Authentication required to access to the server");
            break;

            case QNetworkReply::ProtocolUnknownError:
                errorMsg = tr("Unknown network error");
            break;

            case QNetworkReply::ProtocolFailure:
            errorMsg = tr("Protocol error(e399)");
            break;

            default:
                errorMsg = tr("An unhandled error has occured");
            break;
        }
        qDebug() << errorMsg << endl;
    }
}
