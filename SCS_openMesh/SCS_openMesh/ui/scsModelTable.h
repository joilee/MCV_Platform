#pragma once
#include <QtGui> 
#include <QTableWidget>
#include "../observer/modelObserver.h"
class scsModelTable :public QTableWidget, public modelObserver
{
	Q_OBJECT
public:
	scsModelTable(QWidget* a=NULL);
	~scsModelTable();
	void update(visualModelItem*a);
private:
	 //*table;
	void clearCityItem();
	void clearLocalItem();
};

