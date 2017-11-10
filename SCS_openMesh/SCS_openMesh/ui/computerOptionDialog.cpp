#include "computerOptionDialog.h"
#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include<QListWidget>
#include <QMessageBox>
#include <fstream>

computerOptionDialog::computerOptionDialog(QWidget *parent){

	es=new emitSource(parent);
	fp=new fieldpoint;
	sa=new simuArgument;
	 QPushButton *closeButton = new QPushButton(tr("Close"));
	 okButton=new QPushButton(tr("Update"));
	 okButton->setToolTip(QStringLiteral("更新参数"));
	 check = new QPushButton(tr("Check"));
	 check->setToolTip(QStringLiteral("检查参数是否完整"));
	 connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	 connect(okButton,SIGNAL(clicked()),this,SLOT(getPara()));
	 connect(check, SIGNAL(clicked()), this, SLOT(checkSiteAndAnte()));
	contentsWidget = new QListWidget;
	//layout
	contentsWidget->setMovement(QListView::Static);
	contentsWidget->setMaximumWidth(128);
	contentsWidget->setSpacing(12);
	//
	 pagesWidget = new QStackedWidget;
	 pagesWidget->addWidget(es);
	 pagesWidget->addWidget(fp);
	 pagesWidget->addWidget(sa);

	 //左边的三个list
	 QListWidgetItem *emitSourceList = new QListWidgetItem(contentsWidget);
	 emitSourceList->setText(QStringLiteral("发射源"));
	 emitSourceList->setTextAlignment(Qt::AlignHCenter);
	 emitSourceList->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	 QListWidgetItem* fieldpointList=new QListWidgetItem(contentsWidget);
	 fieldpointList->setText(QStringLiteral("仿真区域"));
	 fieldpointList->setTextAlignment(Qt::AlignHCenter);
	 fieldpointList->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	 QListWidgetItem* simuArgumentList=new QListWidgetItem(contentsWidget);
	 simuArgumentList->setText(QStringLiteral("仿真参数"));
	 simuArgumentList->setTextAlignment(Qt::AlignHCenter);
	 simuArgumentList->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	 connect(contentsWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));



	 //设定初始row
	 contentsWidget->setCurrentRow(0);

	  QHBoxLayout *horizontalLayout = new QHBoxLayout;
	  horizontalLayout->addWidget(contentsWidget);
	  horizontalLayout->addWidget(pagesWidget, 1);

	  QHBoxLayout *buttonsLayout = new QHBoxLayout;
	  buttonsLayout->addStretch(1);
	  buttonsLayout->addWidget(check);
	   buttonsLayout->addWidget(okButton);
	  buttonsLayout->addWidget(closeButton);

	   QVBoxLayout *mainLayout = new QVBoxLayout;
	   mainLayout->addLayout(horizontalLayout);
	   mainLayout->addLayout(buttonsLayout);
	    setLayout(mainLayout);
		setWindowTitle(tr("Option"));
}

bool computerOptionDialog::checkSiteAndAnte()
{
	if (es->siteflag==false||es->gainFlag==false)
	{
		QMessageBox::warning(this, QStringLiteral("天线参数"), QStringLiteral("缺少天线或者小区数据，请导入"));
		return false;
	}
	return true;
}

void computerOptionDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	//do nothing

	if (!current)
		current = previous;

	pagesWidget->setCurrentIndex(contentsWidget->row(current));

	return;
}

void computerOptionDialog::getPara()
{
	if (!checkSiteAndAnte())
	{
		return;
	}
	
	globalContext *gctx=globalContext::GetInstance();
	ComputePara *tmp = gctx->cptManager->getComputationPara();
	tmp->phi=es->getAngle();
	fp->getFieldPoint(tmp->leftUpX, tmp->leftUpY, tmp->rightDownX, tmp->rightDownY, tmp->precision, tmp->altitude);
	sa->getSimuArgu(tmp->reflectNumPara, tmp->refractNumPara, tmp->diffractionNumPara, tmp->isDiffractionPara, tmp->transIndex, tmp->RT_sample, tmp->RT_radius, tmp->RT_BeamNum);
	QMessageBox::information(this, QStringLiteral("计算参数"), QStringLiteral("参数设置成功"), QMessageBox::Yes , QMessageBox::Yes);
}


