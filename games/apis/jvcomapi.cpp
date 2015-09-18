#include "jvcomapi.h"

using namespace std;

JvComApi::JvComApi(QObject *parent) :
    AbstractApi(parent)
{
    m_apiName = "jeuxvideo_dot_com";
    m_onlineApi = true;
    m_authenticationRequired = true;
    m_host = "http://ws.jeuxvideo.com/";
    m_getPath = "01.jeux/";
    m_searchPath = "search/";
    m_username = "appandr";
    m_password = "e32!cdf";
    m_detailedFilePath = "01.jeux/details/";
    m_ratePath = "03.test/detail/";
    m_picturesPath = "01.jeux/screen/";
    m_networkReply = NULL;
    m_maxRate = 20;

    m_network = new QNetworkAccessManager(this);
    m_apiAuthenticator.setUser(m_username);
    m_apiAuthenticator.setPassword(m_password);    

    connect(m_network, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationSlot(QNetworkReply*,QAuthenticator*)));
}

JvComApi::~JvComApi()
{

}

void JvComApi::search(QString gameName)
{
    if(!gameName.isEmpty())
    {
        QUrl url;
        url.setUrl(m_host + m_searchPath + gameName);
        QNetworkRequest request;
        request.setUrl(url);

        //Wiping all data from m_xmlReader
        m_xmlBuffer.clear();
        m_networkReply = m_network->get(request);
        connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
        connect(m_networkReply, SIGNAL(finished()), this, SLOT(slotFinishedSearch()));
        connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotNetworkReplyError(QNetworkReply::NetworkError)));
    }
}

void JvComApi::get(int gameId)
{
    qDebug() << gameId << endl;
    if(gameId != 0)
    {
        m_gameIdToGet = gameId;
        m_currentGame.setId(m_gameIdToGet);
        QUrl url;
        url.setUrl(m_host + m_getPath + QString::number(gameId) + ".xml");
        QNetworkRequest request;
        request.setUrl(url);

        qDebug() << "Downloading simple game file on " << url.url() << endl;
        m_xmlBuffer.clear();
        m_networkReply = m_network->get(request);
        connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
        connect(m_networkReply, SIGNAL(finished()), this, SLOT(slotFinishedGet()));
        connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotNetworkReplyError(QNetworkReply::NetworkError)));
    }
}

void JvComApi::slotReadyRead()
{
    m_xmlBuffer.append(m_networkReply->readAll());
}

void JvComApi::slotFinishedSearch()
{
    m_networkReply->deleteLater();
    m_networkReply = NULL;

    ApiGameList agl = parseSearchResult();

    m_xmlBuffer.clear();
    emit finishedSearch(agl);
}

void JvComApi::slotFinishedGet()
{
    qDebug() << "done" << endl;
    m_networkReply->deleteLater();
    m_networkReply = NULL;
    //Parsing simple game file results
    parseGetResult();

    //Starting details download
    QUrl url;
    url.setUrl(m_host + m_detailedFilePath +  QString::number(m_gameIdToGet) + ".xml");

    QNetworkRequest request(url);
    qDebug() << "Downloading detailed game file on " << url.url() << endl;
    m_xmlBuffer.clear();
    m_networkReply = m_network->get(request);
    connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_networkReply, SIGNAL(finished()), this, SLOT(slotFinishedGetDetails()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotNetworkReplyError(QNetworkReply::NetworkError)));
}

void JvComApi::slotFinishedGetDetails()
{
    m_networkReply->deleteLater();
    m_networkReply = NULL;

    //Parsing detailed game file
    parseGetResult();

    //Starting pictures download
    QUrl url;
    url.setUrl(m_host + m_picturesPath + QString::number(m_gameIdToGet) + ".xml");

    QNetworkRequest request(url);

    qDebug() << "Downloading game pictures on " << url.url() << endl;
    m_xmlBuffer.clear();
    m_networkReply = m_network->get(request);
    connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_networkReply, SIGNAL(finished()), this, SLOT(slotFinishedGetPictures()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotNetworkReplyError(QNetworkReply::NetworkError)));
}

void JvComApi::slotFinishedGetPictures()
{
    m_networkReply->deleteLater();
    m_networkReply = NULL;

    parseGetResult();
    m_xmlBuffer.clear();

    emit finishedGet(m_currentGame);
    m_currentGame.clear();
}

void JvComApi::authenticationSlot(QNetworkReply *nr, QAuthenticator *authenticator)
{
    qDebug() << "Server require authentication..." << endl;
    Q_UNUSED(nr);
    *authenticator = m_apiAuthenticator;
    qDebug() << "Authenticating with: " << m_apiAuthenticator.user() << ":" << m_apiAuthenticator.password() << endl;
}

void JvComApi::parseGetResult()
{
    QDomDocument dom;

    QString errorMsg = "";
    int errorLine = -1;
    int errorColumn = -1;

    if(!dom.setContent(m_xmlBuffer, &errorMsg, &errorLine, &errorColumn ) )
    {
        qDebug() << "Unable to read buffer (xml error)" << endl << errorMsg << " l." << errorLine << endl;
    }

    if(dom.firstChild().nodeName() == "jeu")
    {
        parseSimpleGameFileResult(&dom);
    }
    else if(dom.firstChild().nodeName() == "details_jeu")
    {
        parseDetailedGameFile(&dom);
    }
    else if(dom.firstChild().nodeName() == "screenshots")
    {
        parseGameScreenshots(&dom);
    }
}

void JvComApi::parseGameScreenshots(QDomDocument *dom)
{
    QDomNodeList n = dom->elementsByTagName("url_screenshot");
    QStringList listScreens;
    for(int i = 0; i < n.size(); ++i)
    {
        QDomNode a = n.at(i);
        QDomNode text = a.firstChild();

        listScreens.push_back(text.nodeValue());
    }

    m_currentGame.setPicturesList(listScreens);
}

void JvComApi::parseDetailedGameFile(QDomDocument *dom)
{
    QDomElement node = dom->firstChildElement().firstChildElement();

    //According to the current version of the api, the first node is the summary
    if(node.nodeName() == "resume")
    {
        QDomNode content = node.firstChild();
        QString overview = content.toCDATASection().data();
        m_currentGame.setOverview(overview);
    }

    //Must be only one element
    QDomNode articlesTag = dom->elementsByTagName("articles").at(0);
    QDomElement articlesChildren = articlesTag.firstChildElement();
    bool foundTest = false;
    while(!articlesChildren.isNull() && !foundTest)
    {
        QDomElement article = articlesChildren.firstChildElement();
        QDomNode content = article.firstChild();

        if(content.nodeValue() == "test")
        {
            //@todo Changer ça...
            QDomNode articleElmt;
            articleElmt = article.nextSibling();
            articleElmt = articleElmt.nextSibling();

            QDomNode contentTag = articleElmt.firstChild();
            int rate = contentTag.nodeValue().toInt();
            m_currentGame.setRating(rate);

            foundTest = true;
        }

        articlesChildren = articlesChildren.nextSiblingElement();
    }

    if(!foundTest)
        m_currentGame.setRating(-2); // Allows to determine if no test has been perfomed on this game
}

void JvComApi::parseSimpleGameFileResult(QDomDocument *dom)
{
    QDomElement node = dom->firstChildElement().firstChildElement();

    while(!node.isNull())
    {
        QDomNode content;
        if(node.nodeName() == "titre")
        {
            content = node.firstChild();
            QString title = "";

            title = content.toCDATASection().data();
            m_currentGame.setName(title);
        }
        else if(node.nodeName() == "date_sortie")
        {
            QStringList months;
            months << "janvier" << "fevrier" << "mars" << "avril" << "mai" << "juin" << "juillet" << "août" << "septembre" << "octobre" << "novembre" << "decembre";
            content = node.firstChild();
            QString strDate = content.toCDATASection().data();

            QStringList tabArray = strDate.split(" ");

            if(tabArray.size() == 3)
            {
                //Getting each part of the date
                int day = tabArray.at(0).toInt();
                int month = 0;
                //Getting the month
                for(int i = 0; i < months.size(); ++i)
                {
                    if(months.at(i) == tabArray.at(1).toLower())
                    {
                        month = i + 1;
                    }
                }

                int year = tabArray.at(2).toInt();
                QDate date(year, month, day);
                m_currentGame.setReleaseDate(date);
            }
            else
            {
                int month = 0;
                for(int i = 0; i < months.size(); ++i)
                {
                    if(months.at(i) == tabArray.at(0).toLower())
                    {
                        month = i + 1;
                    }
                }
            }
        }
        else if(node.nodeName() == "developpeur")
        {
            content = node.firstChild();
            QString developpeur = content.toCDATASection().data();

            m_currentGame.setDevelopper(developpeur);
        }
        else if(node.nodeName() == "editeur")
        {
            content = node.firstChild();
            QString editeur = content.toCDATASection().data();

            m_currentGame.setPublisher(editeur);
        }
        else if(node.nodeName() == "type")
        {
            content = node.firstChild();
            QStringList listeTypes;
            if(!content.toCDATASection().data().isEmpty())
            {
                listeTypes = content.toCDATASection().data().split(" / ");
            }

            m_currentGame.setGenresList(listeTypes);
        }
        else if(node.nodeName() == "classification")
        {
            content = node.firstChild();
            QString esrb = content.toCDATASection().data();

            m_currentGame.setEsrb(esrb);
        }

        node = node.nextSiblingElement();
    }
    //qDebug() << m_currentGame.getName() << endl << m_currentGame.getDevelopper() << endl <<m_currentGame.getPublisher() << endl << m_currentGame.getGenresList() << endl;
}

ApiGameList JvComApi::parseSearchResult()
{
    ApiGameList agl;

    QDomDocument dom;
    dom.setContent(m_xmlBuffer);
    QDomElement rootNode = dom.firstChildElement();

    QDomNodeList searchNode = rootNode.elementsByTagName("recherche");

    if(!searchNode.isEmpty())
    {
        //It will be only one <recherche> node
        QDomNode sNode = searchNode.at(0);
        QDomNodeList list = sNode.childNodes();
        QString cdataContents = list.at(0).toCDATASection().data();
        cdataContents = this->cleanDirtyTags(cdataContents);
        cdataContents = "<root>" + cdataContents + "</root>";
        QDomDocument searchDom;
        searchDom.setContent(cdataContents);

        QDomElement node = searchDom.firstChildElement().firstChildElement();
        QDomNodeList l = searchDom.childNodes();

        while(!node.isNull())
        {
            QString idValue = node.attribute("id");
            if(!idValue.isEmpty() && idValue == "resultat")
            {
                QDomElement resultatChildren = node.firstChildElement();
                bool pcFound = false;
                bool webFound = false;
                QString currentCategory = "";
                while(!resultatChildren.isNull() && (!pcFound || !webFound))
                {
                    QString attributeValue = resultatChildren.attribute("class");

                    if(!attributeValue.isEmpty() && attributeValue == "m_pc")
                    {
                        currentCategory = attributeValue;
                        pcFound = true;
                    }
                    else if(!attributeValue.isEmpty() && attributeValue == "m_web")
                    {
                        currentCategory = attributeValue;
                        webFound = true;
                    }
                    else if(!attributeValue.isEmpty() && attributeValue != "liste_liens")
                    {
                        currentCategory = "";
                    }

                    if(currentCategory != "" && attributeValue == "liste_liens")
                    {
                        QDomElement pcGameList = resultatChildren.firstChildElement();

                        while(!pcGameList.isNull())
                        {
                            QDomElement aTag = pcGameList.firstChildElement();
                            aTag.normalize();
                            QString href = aTag.attribute("href");
                            int lastSlashPos = href.lastIndexOf("/");
                            int extensionDotPos = href.lastIndexOf(".");

                            href = href.remove(extensionDotPos, href.size() - 1);
                            href = href.remove(0, lastSlashPos + 1);

                            bool ok;
                            int gameId = href.toInt(&ok, 10);

                            QDomNode text = aTag.firstChild();
                            QString gameName = text.nodeValue();

                            ApiGame ag;
                            ag.setId(gameId);
                            ag.setName(gameName);

                            agl.push_back(ag);

                            pcGameList = pcGameList.nextSiblingElement();
                        }
                    }

                    resultatChildren = resultatChildren.nextSiblingElement();
                }
            }

            node = node.nextSiblingElement();
        }
    }
    else
    {
        throw QString(tr("Unable to parse search results"));
    }

    return agl;
}

QString JvComApi::cleanDirtyTags(QString data)
{
    int firstStrong = data.indexOf("<strong>");
    int firstEndStrong = data.indexOf("</strong>");
    int offset = firstEndStrong - (firstStrong) + QString("</strong>").size();
    QString ret = data.replace(firstStrong, offset, "<!--Trolol-->");

    return ret;
}

void JvComApi::slotNetworkReplyError(QNetworkReply::NetworkError error)
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
