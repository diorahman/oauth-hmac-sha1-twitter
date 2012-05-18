#ifndef PAGE_H
#define PAGE_H

#include <QObject>


class WebPage;

class Page : public QObject{
	
	Q_OBJECT
	
	Q_PROPERTY(QString script READ script WRITE setScript NOTIFY scriptChanged);
	
public:
	Page(QObject * parent = 0);
	~Page();
	
	void setScript(const QString & script);
	QString script() const;
	
signals:
        void result(const QString & jsonString, int index = -1);
	void started();
	void scriptChanged();
	
public slots:
        void load(const QString & address, int listIndex);
        void eval();
        void setUserAgent(const QString & userAgent);
        void reset();
	
private slots:
	void loadFinished(const QString & status);
	void loadStarted();
		
private:
	WebPage * m_page;
	QString m_script;
        int m_listIndex;
};

#endif
