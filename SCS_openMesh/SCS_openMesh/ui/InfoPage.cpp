#include "InfoPage.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

BuildingInfoPage::BuildingInfoPage(QWidget *parent)
{
	init();
	setView();
}

void BuildingInfoPage::setValue(int c,int b,Vector3d bmin,Vector3d bmax)
{
	concaveBuilding=c;
	buildingCount=b;
	boundingMin=bmin;
	boundingMax=bmax;
	setLineEdit();
}

void BuildingInfoPage::init(){
	buildingCount=0;
	concaveBuilding=0;
	boundingMin=Vector3d(0,0,0);
	boundingMax=Vector3d(0,0,0);
	buildingLE=new QLineEdit;buildingLE->setFocusPolicy(Qt::NoFocus);
	concaveLE=new QLineEdit;concaveLE->setFocusPolicy(Qt::NoFocus);
	XminLE=new QLineEdit;XminLE->setFocusPolicy(Qt::NoFocus);
	YminLE=new QLineEdit;YminLE->setFocusPolicy(Qt::NoFocus);
	ZminLE=new QLineEdit;ZminLE->setFocusPolicy(Qt::NoFocus);
	XmaxLE=new QLineEdit;XmaxLE->setFocusPolicy(Qt::NoFocus);
	YmaxLE=new QLineEdit;YmaxLE->setFocusPolicy(Qt::NoFocus);
	ZmaxLE=new QLineEdit;ZmaxLE->setFocusPolicy(Qt::NoFocus);
}

void BuildingInfoPage::setView()
{
	firstGroup=new QGroupBox(QStringLiteral("建筑物"));
	QHBoxLayout *firstLayout=new QHBoxLayout;
	QLabel * label1=new QLabel(QStringLiteral("建筑物数量"));
	firstLayout->addWidget(label1);
	firstLayout->addWidget(buildingLE);
	
	QHBoxLayout *secondLayout=new QHBoxLayout;
	QLabel *label2=new QLabel(QStringLiteral("凹建筑物"));
	secondLayout->addWidget(label2);
	secondLayout->addWidget(concaveLE);

	QVBoxLayout *v1=new QVBoxLayout;
	v1->addLayout(firstLayout);
	v1->addLayout(secondLayout);
	firstGroup->setLayout(v1);

	secondGroup=new QGroupBox("Bounding Box");

	QHBoxLayout *thirdlayout=new QHBoxLayout;
	QLabel *label3=new QLabel("Xmin");
	QLabel *label4=new QLabel("Xmax");

	thirdlayout->addWidget(label3);
	thirdlayout->addWidget(XminLE);
	thirdlayout->addWidget(label4);
	thirdlayout->addWidget(XmaxLE);

	QHBoxLayout *fourthlayout=new QHBoxLayout;
	QLabel *label5=new QLabel("Ymin");
	QLabel *label6=new QLabel("Ymax");

	fourthlayout->addWidget(label5);
	fourthlayout->addWidget(YminLE);
	fourthlayout->addWidget(label6);
	fourthlayout->addWidget(YmaxLE);

	QHBoxLayout *fifthlayout=new QHBoxLayout;
	QLabel *label7=new QLabel("Zmin");
	QLabel *label8=new QLabel("Zmax");

	fifthlayout->addWidget(label7);
	fifthlayout->addWidget(ZminLE);
	fifthlayout->addWidget(label8);
	fifthlayout->addWidget(ZmaxLE);

	QVBoxLayout *v2=new QVBoxLayout;
	v2->addLayout(thirdlayout);
	v2->addLayout(fourthlayout);
	v2->addLayout(fifthlayout);

	secondGroup->setLayout(v2);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(firstGroup);
	//mainLayout->addStretch(1);
	mainLayout->addWidget(secondGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

void BuildingInfoPage::setLineEdit(){
	buildingLE->setText(QString::number(buildingCount));
	concaveLE->setText(QString::number(concaveBuilding));
	XminLE->setText(QString::number(int(boundingMin.x)));
	YminLE->setText(QString::number(int(boundingMin.y)));
	ZminLE->setText(QString::number(int(boundingMin.z)));
	XmaxLE->setText(QString::number(int(boundingMax.x)));
	YmaxLE->setText(QString::number(int(boundingMax.y)));
	ZmaxLE->setText(QString::number(int(boundingMax.z)));
}


 localPage::localPage(QWidget *parent)
{
	init();
	setView();
}
void localPage::init()
{
	triangleNum=0;
	boundingMin=Vector3d(0,0,0);
	boundingMin=Vector3d(0,0,0);
	triLE=new QLineEdit;triLE->setFocusPolicy(Qt::NoFocus);
	XminLE=new QLineEdit;XminLE->setFocusPolicy(Qt::NoFocus);
	YminLE=new QLineEdit;YminLE->setFocusPolicy(Qt::NoFocus);
	ZminLE=new QLineEdit;ZminLE->setFocusPolicy(Qt::NoFocus);
	XmaxLE=new QLineEdit;XmaxLE->setFocusPolicy(Qt::NoFocus);
	YmaxLE=new QLineEdit;YmaxLE->setFocusPolicy(Qt::NoFocus);
	ZmaxLE=new QLineEdit;ZmaxLE->setFocusPolicy(Qt::NoFocus);
}
void localPage::setParametre(int num,Vector3d minNum,Vector3d maxNum)
{
	boundingMin=minNum;
	boundingMax=maxNum;
	triangleNum=num;
	setLineEditLocal();
}

void localPage::setLineEditLocal()
{
	triLE->setText(QString::number(triangleNum));
	XminLE->setText(QString::number(int(boundingMin.x)));
	YminLE->setText(QString::number(int(boundingMin.y)));
	ZminLE->setText(QString::number(int(boundingMin.z)));
	XmaxLE->setText(QString::number(int(boundingMax.x)));
	YmaxLE->setText(QString::number(int(boundingMax.y)));
	ZmaxLE->setText(QString::number(int(boundingMax.z)));
}
void localPage::setView()
{
	firstGroup=new QGroupBox(QStringLiteral("地面模型"));
	QHBoxLayout *firstLayout=new QHBoxLayout;
	QLabel * label1=new QLabel(QStringLiteral("三角面片数量"));
	firstLayout->addWidget(label1);
	firstLayout->addWidget(triLE);

	QHBoxLayout *thirdlayout=new QHBoxLayout;
	QLabel *label3=new QLabel("Xmin");
	QLabel *label4=new QLabel("Xmax");

	thirdlayout->addWidget(label3);
	thirdlayout->addWidget(XminLE);
	thirdlayout->addWidget(label4);
	thirdlayout->addWidget(XmaxLE);

	QHBoxLayout *fourthlayout=new QHBoxLayout;
	QLabel *label5=new QLabel("Ymin");
	QLabel *label6=new QLabel("Ymax");

	fourthlayout->addWidget(label5);
	fourthlayout->addWidget(YminLE);
	fourthlayout->addWidget(label6);
	fourthlayout->addWidget(YmaxLE);

	QHBoxLayout *fifthlayout=new QHBoxLayout;
	QLabel *label7=new QLabel("Zmin");
	QLabel *label8=new QLabel("Zmax");

	fifthlayout->addWidget(label7);
	fifthlayout->addWidget(ZminLE);
	fifthlayout->addWidget(label8);
	fifthlayout->addWidget(ZmaxLE);

	QVBoxLayout *v2=new QVBoxLayout;
	v2->addLayout(firstLayout);
	v2->addLayout(thirdlayout);
	v2->addLayout(fourthlayout);
	v2->addLayout(fifthlayout);
	firstGroup->setLayout(v2);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(firstGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}