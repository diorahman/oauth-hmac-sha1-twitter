#ifndef TWITTER_H
#define TWITTER_H

#include <QObject>
#include "oauth.h"
#include "oauth_types.h"

class Page;
class QJson;

class Twitter : public OAuth
{
    Q_OBJECT
public:
    explicit Twitter(QObject *parent = 0);
    
    Q_INVOKABLE void testResource();
    
private slots:
    void onRequestTokenReceived(const Params & params, const QString & raw);
    void onAccessTokenReceived(const Params & params, const QString & raw);
    void onResourceReceived(const QString & resource);
    void authorize(const QString & username, const QString & password, const QString & oauthToken);

    void pageReply(const QString & result);

private:
    Page * m_page;
    QJson * m_parser;

    QString m_tempOAuthToken;
    QString m_tempOAuthTokenSecret;

    QString m_oAuthToken;
    QString m_oAuthTokenSecret;

    
};

#endif // TWITTER_H
