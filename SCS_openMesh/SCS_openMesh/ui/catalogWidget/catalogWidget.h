#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QString>
#include "catalogTree.h"
#include <string>
//#include <QContextMenuEvent>

using namespace std;

class catalogWidget : public QTreeWidget
{
public:
	catalogWidget(QWidget *parent = 0);
	~catalogWidget();

	void setHeaderText();

	void addItem(int flag, string text);

	void addParentMenu();
	void addChildMenu();

private:
	catalogTree * tree;
};

#endif