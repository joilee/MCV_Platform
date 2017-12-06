#ifndef CATALOGTREE_H
#define CATALOGTREE_H

#include <QTreeWidget>
#include <string>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QContextMenuEvent>

using namespace std;

class catalogWidget :public QTreeWidget
{
	Q_OBJECT
public:
	catalogWidget(QWidget *parent = 0);
	~catalogWidget();
	
	void addParentMenu();
	void addChildMenu();
	void contextMenuEvent(QContextMenuEvent *event);

	private slots:
	void addModel();
	void deleteModel();

private:
	QTreeWidgetItem * mItem;
	QTreeWidgetItem * cItem;
	QTreeWidgetItem * vItem;
	QStringList modelName;
};

#endif