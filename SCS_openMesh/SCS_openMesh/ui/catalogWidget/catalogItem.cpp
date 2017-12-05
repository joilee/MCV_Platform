#include "catalogItem.h"
#include <QString>

catalogItem::catalogItem()
:QTreeWidgetItem()
{
	this->item = new QTreeWidgetItem();
	this->id = 0;
}

catalogItem::catalogItem(int id, string text)
:QTreeWidgetItem()
{
	this->item = new QTreeWidgetItem();
	this->id = id;
	this->setText(0,QString::fromStdString(text));
}

catalogItem::~catalogItem(){}

int catalogItem::getID()
{
	return this->id;
}

void catalogItem::setID(int id)
{
	this->id = id;
}