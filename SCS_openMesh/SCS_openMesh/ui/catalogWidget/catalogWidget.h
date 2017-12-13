#ifndef CATALOGTREE_H
#define CATALOGTREE_H

#include <QTreeWidget>
#include <string>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QContextMenuEvent>
#include "observer/modelObserver.h"
#include "observer/LocalModelObserver.h"
#include "observer/PluginObserver.h"
using namespace std;

class catalogWidget :public QTreeWidget, public modelObserver, public ComputePluginObserver
{
	Q_OBJECT
public:
	catalogWidget(QWidget *parent = NULL);
	~catalogWidget();
	
	void addParentMenu();
	void addChildMenu();
	void contextMenuEvent(QContextMenuEvent *event);
	void deleleItemsUnderItem(QTreeWidgetItem * a);
	void update(visualModelItem * a);
	void updatePluginInfo(VisualPluginItem* a);

	private slots:
	void addModel();
	void deleteModel();
	void addCptPlugin();
private:
	QTreeWidgetItem * mItem;
	QTreeWidgetItem * cItem;
	QTreeWidgetItem * vItem;
};

#endif