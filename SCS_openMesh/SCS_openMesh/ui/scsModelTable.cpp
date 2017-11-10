#include "scsModelTable.h"
#include <QHeaderView>
#include <sstream>  

scsModelTable::scsModelTable(QWidget* a ) :modelObserver("scsModelTable"), QTableWidget(a)
{
	//QWidget::showMaximized();
//	this = new QTableWidget(this);
	QStringList headers;
	headers << "属性" << "值";
	this->setHorizontalHeaderLabels(headers);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setVisible(false);   //隐藏列表头  
	this->setRowCount(15);
	this->setColumnCount(2);
	this->setItem(0, 0, new QTableWidgetItem(QStringLiteral("建筑物数量")));
	this->setItem(1, 0, new QTableWidgetItem(QStringLiteral("凹建筑物")));
	this->setItem(2, 0, new QTableWidgetItem(QStringLiteral("城市场景Xmin")));
	this->setItem(3, 0, new QTableWidgetItem(QStringLiteral("城市场景Xmax")));
	this->setItem(4, 0, new QTableWidgetItem(QStringLiteral("城市场景Ymin")));
	this->setItem(5, 0, new QTableWidgetItem(QStringLiteral("城市场景Ymax")));
	this->setItem(6, 0, new QTableWidgetItem(QStringLiteral("城市场景Zmin")));
	this->setItem(7, 0, new QTableWidgetItem(QStringLiteral("城市场景Zmax")));
	this->setItem(8, 0, new QTableWidgetItem(QStringLiteral("局部场景三角面片数量")));
	this->setItem(9, 0, new QTableWidgetItem(QStringLiteral("局部场景Xmin")));
	this->setItem(10, 0, new QTableWidgetItem(QStringLiteral("局部场景Xmax")));
	this->setItem(11, 0, new QTableWidgetItem(QStringLiteral("局部场景Ymin")));
	this->setItem(12, 0, new QTableWidgetItem(QStringLiteral("局部场景Ymax")));
	this->setItem(13, 0, new QTableWidgetItem(QStringLiteral("局部场景Zmin")));
	this->setItem(14, 0, new QTableWidgetItem(QStringLiteral("局部场景Zmax")));

}

scsModelTable::~scsModelTable()
{

}

void scsModelTable::clearCityItem()
{
	this->setItem(0, 1, new QTableWidgetItem(QString("")));
	this->setItem(1, 1, new QTableWidgetItem(QString("")));
	this->setItem(2, 1, new QTableWidgetItem(QString("")));
	this->setItem(3, 1, new QTableWidgetItem(QString("")));
	this->setItem(4, 1, new QTableWidgetItem(QString("")));
	this->setItem(5, 1, new QTableWidgetItem(QString("")));
	this->setItem(6, 1, new QTableWidgetItem(QString("")));
	this->setItem(7, 1, new QTableWidgetItem(QString("")));
}

void scsModelTable::clearLocalItem()
{
	this->setItem(8, 1, new QTableWidgetItem(QString("")));
	this->setItem(9, 1, new QTableWidgetItem(QString("")));
	this->setItem(10, 1, new QTableWidgetItem(QString("")));
	this->setItem(11, 1, new QTableWidgetItem(QString("")));
	this->setItem(12, 1, new QTableWidgetItem(QString("")));
	this->setItem(13, 1, new QTableWidgetItem(QString("")));
	this->setItem(14, 1, new QTableWidgetItem(QString("")));
}

void scsModelTable::update(visualModelItem*a)
{
	cout << "ModelTable 接收到更新的信号" << endl;
	if (a->getFlag()[0]!="")
	{
	//	cout << this->rowCount() << " " << this->columnCount() << endl;
	//	this->setItem(0, 1, new QTableWidgetItem(a->getBuildingNum()));
		this->setItem(0, 1, new QTableWidgetItem(QString::number(a->getBuildingNum())));
		this->setItem(1, 1, new QTableWidgetItem(QString::number(a->getConcaveNum())));
		this->setItem(2, 1, new QTableWidgetItem(QString("%1").arg(a->getcityMin()[0], 0, 'f', 3)));
		this->setItem(3, 1, new QTableWidgetItem(QString("%1").arg(a->getcityMin()[1], 0, 'f', 3)));
		this->setItem(4, 1, new QTableWidgetItem(QString("%1").arg(a->getcityMin()[2], 0, 'f', 3)));
		this->setItem(5, 1, new QTableWidgetItem(QString("%1").arg(a->getcityMax()[0], 0, 'f', 3)));
		this->setItem(6, 1, new QTableWidgetItem(QString("%1").arg(a->getcityMax()[1], 0, 'f', 3)));
		this->setItem(7, 1, new QTableWidgetItem(QString("%1").arg(a->getcityMax()[2], 0, 'f', 3)));
	}
	else
	{
		clearCityItem();
	}

	if (a->getFlag()[1] != "")
	{
		this->setItem(8, 1, new QTableWidgetItem(QString::number(a->getLocalFaceNum())));
		this->setItem(9, 1, new QTableWidgetItem(QString("%1").arg(a->getlocalMin()[0], 0, 'f', 3)));
		this->setItem(10, 1, new QTableWidgetItem(QString("%1").arg(a->getlocalMax()[0], 0, 'f', 3)));
		this->setItem(11, 1, new QTableWidgetItem(QString("%1").arg(a->getlocalMin()[1], 0, 'f', 3)));
		this->setItem(12, 1, new QTableWidgetItem(QString("%1").arg(a->getlocalMax()[1], 0, 'f', 3)));
		this->setItem(13, 1, new QTableWidgetItem(QString("%1").arg(a->getlocalMin()[2], 0, 'f', 3)));
		this->setItem(14, 1, new QTableWidgetItem(QString("%1").arg(a->getlocalMax()[2], 0, 'f', 3)));
	}
	else
	{
		clearLocalItem();
	}
	this->show();
	cout << "ModelTable数据已经更新" << endl;
}