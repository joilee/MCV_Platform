#ifndef CATALOGTREE_H
#define CATALOGTREE_H

#include <map>
#include <QTreeWidget>
#include <string>
#include "catalogItem.h"
using namespace std;

class catalogTree :public QTreeWidget
{
public:
	catalogTree(QWidget *parent = 0);
	~catalogTree();

	catalogItem * findItem(int id);
	void addTopItem(string text);
	void addChildItem(int id, string text);

private:
	map<int, catalogItem*> treeList;
	int treeSize;
	QTreeWidget * tree;


};

#endif