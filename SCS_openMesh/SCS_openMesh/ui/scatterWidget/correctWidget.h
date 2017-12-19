#pragma once
#include <QtWidgets/QWidget>

#include "GeneratedFiles/ui_correctionWidget.h"

class correctWidget:public QWidget
{
public:
	correctWidget(QWidget *parent=0);
	~correctWidget();
	void setPara(double a, double b, double c, double d, double e, double f);
private:
	Ui::correctionWidget *ui_correct;
};

