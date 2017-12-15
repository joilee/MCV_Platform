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

#define MODEL_ITEM "Model"
#define CPT_ITEM "Computation"
#define  VIS_ITEM "Visualization"

class catalogWidget :public QTreeWidget, public modelObserver, public ComputePluginObserver
{
	Q_OBJECT
public:
	catalogWidget(QWidget *parent = NULL);
	~catalogWidget();
	
	/**
	  * @Method:    addParentMenu
	  * @Note: 	增加
	  * @Author:    Li Gen
	  * @ Date:     2017/12/15
	  * @Returns:   void
	*/
	void addParentMenu();

	/**
	  * @Method:    addChildMenu
	  * @Note: 	
	  * @Author:    Li Gen
	  * @ Date:     2017/12/15
	  * @Returns:   void
	*/
	void addChildMenu();
	void contextMenuEvent(QContextMenuEvent *event);
	void deleleItemsUnderItem(QTreeWidgetItem * a);
	void update(visualModelItem * a);
	void updatePluginInfo(VisualPluginItem* a);

	private slots:
	void addModel();
	void deleteModel();
	void showModel();
	void addCptPlugin();
private:
	QTreeWidgetItem * mItem;
	QTreeWidgetItem * cItem;
	QTreeWidgetItem * vItem;
};

#endif