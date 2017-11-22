#include "computeOptionPage.h"
#include <qwidget.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "../../Context/context.h"

//构造函数
emitSource::emitSource(QWidget* parent) :antennaObserver("emit Source")
{
	angleOfNorth=90;

	//first group layout
	firstGroup=new QGroupBox(tr("emit source"));

	//firstLayout
	QLabel  *angleLable=new QLabel(QStringLiteral("正北方向在场景坐标系中的phi角度[0,360]"));
	angleEdit= new QLineEdit;
	angleEdit->setText("90");
	QHBoxLayout *firstLayout=new QHBoxLayout;
	firstLayout->addWidget(angleLable);
	firstLayout->addWidget(angleEdit);

	QLabel  *zhandian=new QLabel(QStringLiteral("导入站点参数信息"));
	loadSitesButton=new QPushButton(QStringLiteral("添加"));
	QHBoxLayout *secondLayout=new QHBoxLayout;
	secondLayout->addWidget(zhandian);
	secondLayout->addWidget(loadSitesButton);

	sitesTreewidget=new QTreeWidget;
	QStringList headers;
	headers <<QStringLiteral("小区(site*)") ;
	sitesTreewidget->setHeaderLabels(headers);


	QLabel *zengyi=new QLabel(QStringLiteral("天线方向增益文件"));
	loadTransAntennaButton=new QPushButton(QStringLiteral("添加"));
	QHBoxLayout *thirdLayout=new QHBoxLayout;
	thirdLayout->addWidget(zengyi);
	thirdLayout->addWidget(loadTransAntennaButton);

	AntennaList=new QListWidget;
	
	QVBoxLayout *total=new QVBoxLayout;
	total->addLayout(firstLayout);
	total->addLayout(secondLayout);
	total->addWidget(sitesTreewidget);
	total->addLayout(thirdLayout);
	total->addWidget(AntennaList);

	firstGroup->setLayout(total);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(firstGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	connect(loadTransAntennaButton, SIGNAL(clicked()), this, SLOT(openTransAntennas_DirGain()));
	connect(loadSitesButton, SIGNAL(clicked()), this, SLOT(openTransAntenna_ParamFile()));

	siteflag = false;
	gainFlag = false;
}

void emitSource::openTransAntennas_DirGain()
{
	QStringList paths = QFileDialog::getOpenFileNames(this, QStringLiteral("批量导入天线方向增益文件"), "./", QStringLiteral("txt  天线方向增益文件 (*.txt)"));
	if (paths.isEmpty())
		return;
	globalContext *glbctx = globalContext::GetInstance();
	glbctx->cptManager->openTransAntennas_DirGain(paths);
	gainFlag = true;
	return;
}

void emitSource::openTransAntenna_ParamFile()
{
	sitesTreewidget->clear();
	//检测是否导入场景
	globalContext *globalCtx = globalContext::GetInstance();
	if (!globalCtx->modelManager->checkCityExist())
	{
		QMessageBox::warning(this, QStringLiteral("发射天线设置"), QStringLiteral("请先加载场景"));
		return;
	}
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("导入发射天线（站点）参数信息文件"), "./", QStringLiteral("csv 发射天线（站点）参数信息文件 (*.csv)"));
	if (path.isEmpty())
		return;
	globalCtx->cptManager->openTransAntenna_ParamFile(path);
	siteflag = true;
	return;
}
double emitSource::getAngle()
{
	return angleOfNorth;
}

//寻找一个小区是否在treewidget
bool  emitSource::findSite(QString a)
{
	QTreeWidgetItemIterator  it(sitesTreewidget);
	bool res = false;
	while (*it) { // text(0)是QTreeWidgetItem的函数，即第零列的文字
		qDebug() <<"info:qtreeWidget Site"<< (*it)->text(0);
		if ((*it)->text(0) == a)
		{
			res = true;
			break;
		}
		++it;
	}
	return res;
}

QTreeWidgetItem * emitSource::getItem(QString a)
{
	QTreeWidgetItemIterator  it(sitesTreewidget);
	while (*it) { // text(0)是QTreeWidgetItem的函数，即第零列的文字
		if ((*it)->text(0) == a)
		{
			return *it;
		}
		++it;
	}
	return NULL;
}

bool emitSource::findAnte(string a)
{
	QTreeWidgetItemIterator  it(sitesTreewidget);
	bool res = false;
	while (*it) { // text(0)是QTreeWidgetItem的函数，即第零列的文字
		qDebug() << "info: qtreeWidget antenna" << (*it)->text(0);
		if ((*it)->text(0) == QString::fromStdString(a))
		{
			res = true;
			break;
		}
		++it;
	}
	return res;
}
void emitSource::update(visualAntennaItem*a)
{
	//遍历a中所有的小区以及基站，看是否有对应的项目，如果没有则要插入

	//step1 遍历小区，如果没有则要插入
	set<int> site = a->getSites();
	set<int>::iterator it = site.begin();
	for (; it != site.end();it++)
	{
		QString aname = QString("Site") +QString::number (*it);
		if (!findSite(aname))
		{
			QTreeWidgetItem* site_root = new QTreeWidgetItem(sitesTreewidget, QStringList(aname));
			sitesTreewidget->addTopLevelItem(site_root);
		}
	}

	//对每个基站进行遍历,如果没有找到，就找到它的父结点
	map<int, SiteAntenna*> mp = a->getMap();
	map<int, SiteAntenna*>::iterator mIt = mp.begin();
	for (; mIt != mp.end();mIt++)
	{
		set<string>::iterator sIt = mIt->second->ante.begin();
		QString anteName = QString("Site") + QString::number(mIt->second->id);
		QTreeWidgetItem *parent = getItem(anteName);
		for (; sIt != mIt->second->ante.end();sIt++)
		{
			string cellname = "Cell" + *(sIt);
			if (!findAnte(cellname))
			{
				QTreeWidgetItem *leaf = new QTreeWidgetItem(parent, QStringList(QString::fromStdString(cellname)));
				parent->addChild(leaf);
			}
		}
	}

}

void fieldpoint::computingModeButtonsToggled(int id, bool flag)
{
	//根据选中的button，设置计算类型
	int mode = simuModeGroup->checkedId();
	globalContext *globalCtx = globalContext::GetInstance();
	globalCtx->cptManager->setCptCatagory(mode);
	qDebug("id = %d, status = %d, tmp = %d", id, flag, mode);
	switch (id)
	{
	case 1:
		enableRangeMode(flag);
		break;
	case 2:
		enablePointMode(flag);
		break;
	case 3:
		enableCustomMode(flag);
		break;
	default:
		break;
	}
}

void fieldpoint::openNo_SimplaneReceiverFile()
{
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("打开非仿真面设置的接收点文件"), "./", QStringLiteral("csv 非仿真面接收点文件 (*.csv)"));
	if (path.isEmpty())
		return;
	globalContext *globalCtx = globalContext::GetInstance();
	if (!globalCtx->modelManager->checkCityExist())
	{
		QMessageBox::warning(this, QStringLiteral("发射天线设置"), QStringLiteral("请先加载场景"));
		return;
	}
	globalCtx->cptManager->openNo_simplaneReceiver(path.toStdString());
	filePath->setText(path);

	return;
}

void fieldpoint::enableRangeMode(bool flag)
{
	leftupXinput->setEnabled(flag);
	leftupYinput->setEnabled(flag);
	rightbottomXinput->setEnabled(flag);
	rightbottomYinput->setEnabled(flag);
}

void fieldpoint::enablePointMode(bool flag)
{
	filePath->setEnabled(flag);
	loadReceieverPointFile->setEnabled(flag);
}

void fieldpoint::enableCustomMode(bool flag)
{

}

void fieldpoint::getFieldPoint(double &lx,double &ly,double &rx,double &ry,double &pre,double &alti)
{
	lx=leftupXinput->text().toDouble();
	ly=leftupYinput->text().toDouble();
	rx=rightbottomXinput->text().toDouble();
	ry=rightbottomYinput->text().toDouble();
	pre=Precisioninput->text().toDouble();
	alti=Altitudeinput->text().toDouble();
}
fieldpoint::fieldpoint(QWidget* parent) :modelObserver("scFieldPointRange"), QWidget(parent)
{
	//初始化成员变量
	RecePointFilePath="";
	leftupX=0;
	leftupY=0;
	rightbottomX=0;
	rightbottomY=0;
	predictPrecision=5;
	predictAltitude=5;

	//布局
	QGroupBox *firstgroupbox=new QGroupBox(QStringLiteral("仿真区域（单位：m）"));
	rangeRadioButton = new QRadioButton(QStringLiteral("仿真面计算"));
	pointRadioButton = new QRadioButton(QStringLiteral("接收点计算"));
	customRadioButton = new QRadioButton(QStringLiteral("自定义计算"));
	simuModeGroup = new QButtonGroup(this);
	simuModeGroup->addButton(rangeRadioButton,1);
	simuModeGroup->addButton(pointRadioButton,2);
	simuModeGroup->addButton(customRadioButton,3);
	rangeRadioButton->setChecked(true);


	//h4属于接收点模式
	QLabel* label4=new QLabel(QStringLiteral("导入非仿真面接收点位置"));
	QHBoxLayout *h4=new QHBoxLayout;
	filePath=new QLineEdit;
	filePath->setReadOnly(true);
	loadReceieverPointFile=new QPushButton(QStringLiteral("导入"));
	h4->addWidget(label4);
	h4->addWidget(filePath);
	h4->addWidget(loadReceieverPointFile);

	//h5属于仿真面模式
	QLabel* label5=new QLabel(QStringLiteral("左上角（X，Y）"));
	leftupXinput=new QLineEdit;
	leftupYinput=new QLineEdit;
	leftupXinput->setReadOnly(true);
	leftupYinput->setReadOnly(true);
	QHBoxLayout *h5=new QHBoxLayout;
	h5->addWidget(label5);
	h5->addWidget(leftupXinput);
	h5->addWidget(leftupYinput);

	QLabel* label6=new QLabel(QStringLiteral("右下角（X，Y）"));
	rightbottomXinput=new QLineEdit;
	rightbottomYinput=new QLineEdit;
	rightbottomXinput->setReadOnly(true);
	rightbottomYinput->setReadOnly(true);
	QHBoxLayout*h6=new QHBoxLayout;
	h6->addWidget(label6);
	h6->addWidget(rightbottomXinput);
	h6->addWidget(rightbottomYinput);

	QLabel *label7=new QLabel(QStringLiteral("预测精度"));
	Precisioninput=new QLineEdit;
	QHBoxLayout *h7=new QHBoxLayout;
	h7->addWidget(label7);
	h7->addWidget(Precisioninput);

	QLabel *label8=new QLabel(QStringLiteral("预测高度"));
	Altitudeinput=new QLineEdit;
	QHBoxLayout *h8=new QHBoxLayout;
	h8->addWidget(label8);
	h8->addWidget(Altitudeinput);

	QVBoxLayout *total=new QVBoxLayout;
	total->addWidget(rangeRadioButton);
	total->addLayout(h5);
	total->addLayout(h6);

	total->addWidget(pointRadioButton);
	total->addLayout(h4);

	total->addLayout(h7);
	total->addLayout(h8);

	firstgroupbox->setLayout(total);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(firstgroupbox);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	leftupXinput->setText("0");
	leftupYinput->setText("0");
	rightbottomXinput->setText("0");
	rightbottomYinput->setText("0");
	Precisioninput->setText("5");
	Altitudeinput->setText("5");

	connect(simuModeGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(computingModeButtonsToggled(int, bool)));
	connect(loadReceieverPointFile, SIGNAL(clicked()), this, SLOT(openNo_SimplaneReceiverFile()));

	enableCustomMode(false);
	enablePointMode(false);
	enableRangeMode(true);

}

void fieldpoint::update(visualModelItem*a)
{
	//判断局部场景是否存在
	if (!a->isLocalSceneEmpty())//存在
	{
		leftupXinput->setText(QString("%1").arg(a->getlocalMin()[0], 0, 'f', 3));
		leftupYinput->setText(QString("%1").arg(a->getlocalMax()[1], 0, 'f', 3));
		rightbottomXinput->setText(QString("%1").arg(a->getlocalMax()[0], 0, 'f', 3));
		rightbottomYinput->setText(QString("%1").arg(a->getlocalMax()[1], 0, 'f', 3));
	}
	else
	{
		leftupXinput->setText("0");
		leftupYinput->setText("0");
		rightbottomXinput->setText("0");
		rightbottomYinput->setText("0");
	}
}

simuArgument::simuArgument(QWidget* parent)
{
	reflectNum=0;
	refractNum=0;
	diffractionNum=0;
	isDiffraction=false;

	QGroupBox *firstgroupbox=new QGroupBox(QStringLiteral("仿真参数"));
	QLabel *label1=new QLabel(QStringLiteral("反射次数"));
	QLabel*label2=new QLabel(QStringLiteral("透射次数"));
	QLabel*label2_5=new QLabel(QStringLiteral("透射损耗"));
	QLabel*label3=new QLabel(QStringLiteral("绕射"));
	QLabel*label4=new QLabel(QStringLiteral("绕射次数"));
	QLabel*label5=new QLabel(QStringLiteral("采样密度"));
	QLabel*label6=new QLabel(QStringLiteral("采样球半径"));
	QLabel*label7=new QLabel(QStringLiteral("Beam数量"));

	lineEdit_RT_ReflectCount=new QLineEdit;
	lineEdit_RT_RefractCount=new QLineEdit;
	lineEdit_RT_Diffraction=new QLineEdit;
	comboBox_transmission=new QComboBox;
	comboBox_diffraction=new QComboBox;
	lineEdit_RT_sample=new QLineEdit;
	lineEdit_RT_sample_radius=new QLineEdit;
	lineEdit_RT_sampleBeamNum=new QLineEdit;


	comboBox_diffraction->insertItem(0,QStringLiteral("不考虑绕射"));
	comboBox_diffraction->insertItem(1,QStringLiteral("考虑绕射"));

	comboBox_transmission->insertItem(0,QStringLiteral("经验值"));
	comboBox_transmission->insertItem(1,QStringLiteral("理论公式计算"));

	QHBoxLayout* h1=new QHBoxLayout;
	h1->addWidget(label1);
	h1->addWidget(lineEdit_RT_ReflectCount);

	QHBoxLayout* h2=new QHBoxLayout;
	h2->addWidget(label2);
	h2->addWidget(lineEdit_RT_RefractCount);

	QHBoxLayout *h2_5=new QHBoxLayout;
	h2_5->addWidget(label2_5);
	h2_5->addWidget(comboBox_transmission);

	QHBoxLayout *h3=new QHBoxLayout;
	h3->addWidget(label3);
	h3->addWidget(comboBox_diffraction);

	QHBoxLayout*h4=new QHBoxLayout;
	h4->addWidget(label4);
	h4->addWidget(lineEdit_RT_Diffraction);

	QHBoxLayout*h5=new QHBoxLayout;
	h5->addWidget(label5);
	h5->addWidget(lineEdit_RT_sample);

	QHBoxLayout*h6=new QHBoxLayout;
	h6->addWidget(label6);
	h6->addWidget(lineEdit_RT_sample_radius);

	QHBoxLayout*h7=new QHBoxLayout;
	h7->addWidget(label7);
	h7->addWidget(lineEdit_RT_sampleBeamNum);

	QVBoxLayout *total=new QVBoxLayout;
	total->addLayout(h1);
	total->addLayout(h2);
	total->addLayout(h2_5);
	total->addLayout(h3);
	total->addLayout(h4);
	total->addLayout(h5);
	total->addLayout(h6);
	total->addLayout(h7);

	firstgroupbox->setLayout(total);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(firstgroupbox);
	mainLayout->addStretch(1);


	lineEdit_RT_Diffraction->setText("0");
	lineEdit_RT_sampleBeamNum->setText("0");

	setLayout(mainLayout);
}

void simuArgument::getSimuArgu(double &reflect,double &refract,double &diffract,bool &isDiff,int transIndex,double &sample,double &radius,double &beamNum)
{
	refract=lineEdit_RT_ReflectCount->text().toDouble();
	refract=lineEdit_RT_ReflectCount->text().toDouble();
	diffract=lineEdit_RT_RefractCount->text().toDouble();
	if (comboBox_diffraction->currentIndex()==0)
	{
		isDiff=false;
	}else
		isDiff=true;
	transIndex=comboBox_transmission->currentIndex();
	sample=lineEdit_RT_sample->text().toDouble();
	radius=lineEdit_RT_sample_radius->text().toDouble();
	beamNum=lineEdit_RT_sampleBeamNum->text().toDouble();
}