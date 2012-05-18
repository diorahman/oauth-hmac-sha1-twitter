#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QObject>
#include <QVariant>

class CustomPage;
class QWebFrame;

class WebPage : public QObject
{
	Q_OBJECT
	
public:
	WebPage(QObject * parent = 0);
	~WebPage();
		
signals:
	void loadStarted();
	void loadFinished(const QString &status);
		
public slots:
	void setUserAgent(const QString & userAgent);
	void openUrl(const QString &address);
	QVariant evaluate(const QString &script);

    void reset();
	
private slots:
	void finished(bool ok);
		
private:
	CustomPage * m_webPage;
	QWebFrame * m_mainFrame;
	
	friend class CustomPage;
	
};

#endif
