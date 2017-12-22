#include "correctWidget.h"

correctWidget::correctWidget(QWidget *parent) :ui_correct(new Ui::correctionWidget)
{
	ui_correct->setupUi(this);
	ui_correct->tableWidget->setColumnCount(3);
	ui_correct->tableWidget->setRowCount(3);
	QStringList headers;
	headers << QStringLiteral("属性") << QStringLiteral("校正前") << QStringLiteral("校正后");
	ui_correct->tableWidget->setHorizontalHeaderLabels(headers);
	ui_correct->tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui_correct->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui_correct->tableWidget->setItem(0, 0, new QTableWidgetItem(QStringLiteral("均值")));
	ui_correct->tableWidget->setItem(1, 0, new QTableWidgetItem(QStringLiteral("标准差")));
	ui_correct->tableWidget->setItem(2, 0, new QTableWidgetItem(QStringLiteral("校正系数")));
	ui_correct->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

correctWidget::~correctWidget()
{

}

void correctWidget::setPara(double a, double b, double c,double d,double e,double f)
{
	ui_correct->tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(a)));
	ui_correct->tableWidget->setItem(1, 1, new QTableWidgetItem(QString::number(b)));
	ui_correct->tableWidget->setItem(2, 1, new QTableWidgetItem(QString::number(c)));
	ui_correct->tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(d)));
	ui_correct->tableWidget->setItem(1, 2, new QTableWidgetItem(QString::number(e)));
	ui_correct->tableWidget->setItem(2, 2, new QTableWidgetItem(QString::number(f)));
}
