#pragma once

#include <QWebEnginePage>
#include <QDesktopServices>
class webPage : public QWebEnginePage
{
	Q_OBJECT

public:
	explicit webPage(QObject *parent) :QWebEnginePage(parent){};
	~webPage(){};
protected:
	bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame){
		// Only allow qrc:/index.html.
		if (url.scheme() == QString("qrc"))
			return true;
		QDesktopServices::openUrl(url);
		return false;
	}
};
