#pragma once
#include "GeneratedFiles/ui_log.h"
#include <QWidget>
class LogText:public QWidget
{
public:
	LogText(QWidget * parent=nullptr);
	~LogText();
	void addText(QString a);
private:
	Ui::logForm ui;
};

