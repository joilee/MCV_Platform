#include "LogText.h"

LogText::LogText(QWidget * parent)
{
	ui.setupUi(this);
}

LogText::~LogText()
{
}

void LogText::addText(QString a)
{
	ui.logBrowser->append(a);
	QApplication::processEvents();
}
