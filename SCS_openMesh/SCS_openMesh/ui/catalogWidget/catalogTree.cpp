#include "catalogTree.h"

catalogTree::catalogTree(QWidget *parent)
:QTreeWidget()
{
	treeSize = -1;
}

catalogTree::~catalogTree(){}

void catalogTree::addTopItem(string text)
{
	treeSize++;
	catalogItem * item = new catalogItem(treeSize, text);
	tree->addTopLevelItem(item);
	treeList[treeSize] = item;
}

void catalogTree::addChildItem(int id, string text)
{
	treeSize++;
	catalogItem * item = new catalogItem(treeSize, text);
	treeList.at(id)->addChild(item);
	treeList[treeSize] = item;
}

catalogItem * catalogTree::findItem(int id)
{
	return treeList.at(id);
}

