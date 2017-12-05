#include "catalogWidget.h"
#include <iostream>

using namespace std;

catalogWidget::catalogWidget(QWidget *parent)
:QTreeWidget(parent)
{
	tree = new catalogTree();
}  

catalogWidget::~catalogWidget(){}

void catalogWidget::setHeaderText()
{
	tree->setHeaderLabels(QStringList() << QStringLiteral("ÏîÄ¿") << QStringLiteral("ÊôÐÔ"));
}

void catalogWidget::addItem(int flag, string text)
{
	if (flag == 0)
		tree->addTopItem(text);
	//if (flag == 1)
		//tree->addChildItem(text);
}
void catalogWidget::addParentMenu()
{
	/*QMenu * menu = new QMenu(tree);
	QAction * action = new QAction(tr("add"), tree);

	menu->addAction(action);
	menu->exec(QCursor::pos());
	menu->show();*/
	
}

void catalogWidget::addChildMenu()
{

}