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
#include "observer/SiteCellObserver.h"
using namespace std;
#define CITY_VIEW "城市"
#define MODEL_ITEM "基站"
#define CPT_ITEM "插件"
#define  VIS_ITEM "可视化"

class catalogWidget :public QTreeWidget, public modelObserver, public ComputePluginObserver,public SiteCellObserver
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
	void updateResult(Site_Item*a);
	void updatePluginInfo(VisualPluginItem* a);

	private slots:
	void addModel();
	void deleteModel();
	void showModel();
	void addCptPlugin();

	signals:
	void modelID_ShowChanged(int id);

private:
	QTreeWidgetItem * mGlobalItem;
	QTreeWidgetItem * mLocalItem;
	QTreeWidgetItem * cItem;
	QTreeWidgetItem * vItem;
};

#endif