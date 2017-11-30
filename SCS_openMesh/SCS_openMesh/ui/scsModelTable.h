#pragma once
#include <QtGui> 
#include <QTableWidget>
#include "../observer/modelObserver.h"
#include "observer/LocalModelObserver.h"
class scsModelTable :public QTableWidget, public modelObserver,public LocalModelObserver
{
	Q_OBJECT
public:
	scsModelTable(QWidget* a=NULL);
	~scsModelTable();
	void update(visualModelItem*a);
	void updateLocal(LocalModelItem*a);
private:
	 //*table;
	void clearCityItem();
	void clearLocalItem();
};

