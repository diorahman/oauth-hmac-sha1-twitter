#include "twitter.h"
#include <QDebug>
#include "page.h"

#include <QFile>
#include <QFileInfo>

#include "QJson/qjson.h"

#include "oauth_helper.h"

const static QString TwitterRequestTokenUrl("https://api.twitter.com/oauth/request_token");
const static QString TwitterAuthorizeUrl("https://api.twitter.com/oauth/authorize");
const static QString TwitterAccessTokenUrl("https://api.twitter.com/oauth/access_token");
const static QString UserAgent("Mozilla/5.0 (Symbian/3; Series60/5.2 NokiaN8-00/013.016; Profile/MIDP-2.1 Configuration/CLDC-1.1 ) AppleWebKit/525 (KHTML, like Gecko) Version/3.0 BrowserNG/7.2.8.10 3gpp-gba");

const static QString TwitterAuthorizeScript("function(){function login(username, password){document.querySelector('#username_or_email').value=username;document.querySelector('#password').value=password;document.querySelector('#allow').click(); return {'state': 'login'}} var obj = {'state':'unknown'}; if(document.querySelector('#username_or_email') && !(document.querySelector('div.notice'))) {obj = login('%1', '%2')} else if(document.querySelector('div.notice') && (document.querySelector('div.notice').innerHTML.indexOf('Redirecting') == -1)){ obj = {'state' : 'error', 'data' : document.querySelector('div.notice').innerHTML} } else { obj = {'state': 'verified', 'data': document.querySelector('div.notice').querySelector('a').href.split('oauth_verifier=')[1]}} obj.service = 'twitter'; return JSON.stringify(obj);}");

Twitter::Twitter(QObject *parent) :
    OAuth(parent)
{
    m_page = new Page(this);
    m_parser = new QJson(this);
    connect(this, SIGNAL(requestTokenReceived(Params,QString)), this, SLOT(onRequestTokenReceived(Params,QString)));
    connect(this, SIGNAL(accessTokenReceived(Params,QString)), this, SLOT(onAccessTokenReceived(Params,QString)));
    connect(this, SIGNAL(resourceReceived(QString)), this, SLOT(onResourceReceived(QString)));
    connect(m_page, SIGNAL(result(QString)), this, SLOT(pageReply(QString)));
}

void Twitter::testResource()
{
    //m_oAuthToken = "17058778-5NPWqWK0SneVGEzZbFfIyulBVfKpRd4YV5gMB9J8";
    //m_oAuthTokenSecret = "3kGtuFZ5ZPNyxu6guO1qaC5d3Da8i1vnEehWAeAFg";

    m_oAuthToken = "222985359-LODFDMHKy4gYU1NncP5PqYqrKKZxdLCIBmhw7jR3";
    m_oAuthTokenSecret = "DwA9I6gjj1E0QoGZhYqASoBGH0THuS13VNQjZNaho0M";

    this->setOauthToken(m_oAuthToken);
    this->setOauthTokenSecret(m_oAuthTokenSecret);

    Params data;

    data.insert("status", "while(true){}");
    resource("https://api.twitter.com/1/statuses/update.json", "POST", data);

    data.clear();

    data.insert("include_entities","true");
    resource("https://api.twitter.com/1/statuses/home_timeline.json", "GET", data);
}

void Twitter::testUploadImage()
{

    m_oAuthToken = "222985359-LODFDMHKy4gYU1NncP5PqYqrKKZxdLCIBmhw7jR3";
    m_oAuthTokenSecret = "DwA9I6gjj1E0QoGZhYqASoBGH0THuS13VNQjZNaho0M";

    this->setOauthToken(m_oAuthToken);
    this->setOauthTokenSecret(m_oAuthTokenSecret);

    QFile file("/Users/diorahman/Develop/hack/OAuthTest/images/qt.png");
    file.open(QIODevice::ReadOnly);

    Part imagePart;
    imagePart.contentDisposition = "name=\"media[]\"; filename=\"1234.png\"";
    imagePart.contentType = "application/octet-stream";
    imagePart.data = file.readAll();

    Part statusPart;
    statusPart.contentDisposition = "name=\"status\"";
    statusPart.contentType = "";
    statusPart.data = "Abrakadabra";

    Parts parts;
    parts.insert("image", imagePart);
    parts.insert("status", statusPart);

    QByteArray data = Helper::buildMultipartBody(this->mutipartBoundary(), parts);

     resource("https://upload.twitter.com/1/statuses/update_with_media.json", "POST", Params(), data);
}

void Twitter::onRequestTokenReceived(const Params &params, const QString & raw)
{
    qDebug() << "Request Token";
    qDebug() << raw;

    this->setOauthToken(m_oAuthToken);
    this->setOauthTokenSecret(m_oAuthTokenSecret);

    m_tempOAuthToken = params.value("oauth_token");
    m_tempOAuthTokenSecret = params.value("oauth_token_secret");

    authorize("","", m_tempOAuthToken);

}

void Twitter::onAccessTokenReceived(const Params &params, const QString &raw)
{
    qDebug() << "Access Token";
    qDebug() << raw;

    m_oAuthToken = params.value("oauth_token");
    m_oAuthTokenSecret = params.value("oauth_token_secret");

    this->setOauthToken(m_oAuthToken);
    this->setOauthTokenSecret(m_oAuthTokenSecret);

    qDebug() << m_oAuthToken;
    qDebug() << m_oAuthTokenSecret;

    Params data;

    data.insert("status", "while(true){}");
    resource("https://api.twitter.com/1/statuses/update.json", "POST", data);

    data.clear();

    data.insert("include_entities","true");
    resource("https://api.twitter.com/1/statuses/home_timeline.json", "GET", data);
}

void Twitter::onResourceReceived(const QString &resource)
{
    qDebug() << "Resource";
    qDebug() << resource;
}

void Twitter::authorize(const QString & username, const QString & password, const QString & oauthToken)
{
    m_page->reset();
    m_page->setUserAgent(UserAgent);
    m_page->setScript(TwitterAuthorizeScript.arg(username).arg(password));
    m_page->load(TwitterAuthorizeUrl + QString("?oauth_token=%1").arg(oauthToken), -1);
}

void Twitter::pageReply(const QString & result)
{
    QVariant v = m_parser->parse(result.toAscii());

    QVariantMap m = v.toMap();
    qDebug() << m.value("state", "");
    qDebug() << m.value("service", "");
    qDebug() << m.value("data", "");

    if(m.value("state", "").toString() == "verified"){
        accessToken(TwitterAccessTokenUrl, m_tempOAuthToken, m_tempOAuthTokenSecret,  m.value("data", "").toString());
    }

    qDebug() << result;

}
