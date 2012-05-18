#include "page.h"
#include "webpage.h"
#include <QDebug>

Page::Page(QObject * parent) : QObject (parent), m_script("function(){}"){
	
	m_page = new WebPage(this);
	connect(m_page, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
	connect(m_page, SIGNAL(loadFinished(QString)), this, SLOT(loadFinished(QString)));
}

void Page::loadFinished(const QString & status){
	
    QString res = "{\"message\":\"failed\"}";

        qDebug() << "status" << status;
	
	if(status == "success"){
		res = m_page->evaluate(m_script).toString();
                if(res.isEmpty()) res = "{\"message\":\"empty\"}";
	}
	
        qDebug() << "res: " << res;

        emit result(res, m_listIndex);
}

void Page::loadStarted(){
	qDebug() << "load started";
}

void Page::load(const QString & address, int listIndex){
        qDebug() << "load page" << address;
	m_page->openUrl(address);

        m_listIndex = listIndex;
}

Page::~Page(){}

void Page::setScript(const QString & script){
	if(script != m_script){
		m_script = script == "" ? "function(){}" : script;
		emit scriptChanged();
	}

        qDebug() << "script: " << m_script;
}
QString Page::script() const{
    return m_script;
}

void Page::eval()
{
    QString res = "{'message':'failed'}";
    res = m_page->evaluate(m_script).toString();
    if(res.isEmpty()) res = "{'message':'empty'}";
    qDebug() << "res: " << res;
    emit result(res, m_listIndex);
}

void Page::setUserAgent(const QString &userAgent)
{
    m_page->setUserAgent(userAgent);
}

void Page::reset()
{
    m_page->reset();
}

