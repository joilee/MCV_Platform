#ifndef CATALOGTREE_H
#define CATALOGTREE_H

#include <QTreeWidget>
#include <string>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QContextMenuEvent>
#include "observer/modelObserver.h"
#include "observer/LocalModelObserver.h"

using namespace std;

class catalogWidget :public QTreeWidget, public modelObserver
{
	Q_OBJECT
public:
	catalogWidget(QWidget *parent = NULL);
	~catalogWidget();
	
	void addParentMenu();
	void addChildMenu();
	void contextMenuEvent(QContextMenuEvent *event);

	void update(visualModelItem * a);

	private slots:
	void addModel();
	void deleteModel();

private:
	QTreeWidgetItem * mItem;
	QTreeWidgetItem * cItem;
	QTreeWidgetItem * vItem;
};

#endif