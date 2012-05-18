#include "webpage.h"

#include <QWebFrame>
#include <QWebPage>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include "networkaccessmanager.h"

class CustomPage : public QWebPage{
	
	Q_OBJECT
	
public:
	CustomPage(WebPage *parent = 0) : QWebPage(parent), m_webPage(parent)
	{	
		m_userAgent = QWebPage::userAgentForUrl(QUrl());
	}
	
	QString userAgentForUrl(const QUrl &url) const {
		Q_UNUSED(url);
		return m_userAgent;
	}
	
private:
	QString m_userAgent;
	 WebPage *m_webPage;
	
	friend class WebPage;
};

WebPage::WebPage(QObject * parent) : QObject(parent){
	m_webPage = new CustomPage(this);
        m_webPage->setNetworkAccessManager(new NetworkAccessManager(this));
    m_mainFrame = m_webPage->mainFrame();

    connect(m_webPage, SIGNAL(loadStarted()), SIGNAL(loadStarted()));
	connect(m_webPage, SIGNAL(loadFinished(bool)), SLOT(finished(bool)));

}

WebPage::~WebPage(){}

void WebPage::setUserAgent(const QString & userAgent){
	m_webPage->m_userAgent = userAgent;
}

void WebPage::finished(bool ok){
	emit loadFinished( ok ? "success" : "failed");
}

void WebPage::openUrl(const QString &address)
{
	m_webPage->triggerAction(QWebPage::Stop);

	
	QWebSettings *opt = m_webPage->settings();

    opt->setAttribute(QWebSettings::AutoLoadImages, false);
    opt->setAttribute(QWebSettings::PluginsEnabled, false);
    opt->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);

    m_mainFrame->load(QNetworkRequest(QUrl(address)), QNetworkAccessManager::GetOperation);
}

QVariant WebPage::evaluate(const QString &script){
	QString function = "(" + script + ")()";
    return m_mainFrame->evaluateJavaScript(function);
}

void WebPage::reset()
{
    m_webPage->networkAccessManager()->setCookieJar(new QNetworkCookieJar(this));
}

#include "webpage.moc"
