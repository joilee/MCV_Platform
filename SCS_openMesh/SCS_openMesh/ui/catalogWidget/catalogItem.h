#ifndef CATALOGITEM_H
#define CATALOGITEM_H

#include <QTreeWidgetItem>
#include <string>

using namespace std;

class catalogItem :public QTreeWidgetItem
{
public:
	catalogItem();
	catalogItem(int id, string text);
	~catalogItem();

	int getID();
	void setID(int id);

private:
	QTreeWidgetItem * item;
	int id;
};
#endif