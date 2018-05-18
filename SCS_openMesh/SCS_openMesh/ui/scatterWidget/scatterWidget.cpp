#include "scatterWidget.h"
#include "webPage.h"
#include "Document.h"

#include <sstream>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QTextStream>
#include <QJsonDocument>
#include <QWebChannel>
#include <QMessageBox>
#include "Context/context.h"
using namespace std;

scatterWidget::scatterWidget(QWidget *parent) :QDialog(parent), ui(new Ui::scatterDialog)
{
	ui->setupUi(this);
	//dataWidget = new correctWidget(this);

	//	connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(openMeasuredFile()));
	//	connect(this->ui->correct_Button,SIGNAL(clicked()), this, SLOT(showPara()));
	//	connect(this->ui->showResult_Button, SIGNAL(clicked()), this, SLOT(showResult()));
	webPage *page = new webPage(this);
	ui->preview->setPage(page);
	QWebChannel *channel = new QWebChannel(this);
	channel->registerObject(QStringLiteral("content"), &m_content);
	page->setWebChannel(channel);
	ui->preview->setUrl(QUrl("qrc:/MainWindow/Resources/echarts/index.html"));
	//	modeGroup = new QButtonGroup(this);
	//	modeGroup->addButton(ui->radioButton, 1);
	//	modeGroup->addButton(ui->radioButton_2, 2);
	//	ui->radioButton->setChecked(true);
	//	connect(modeGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(modeButtonToggled(int, bool)));
	//initData();
}

scatterWidget::~scatterWidget()
{
	delete ui;
}

void scatterWidget::openTestFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("All Files (*.*)"));
	QFile dataFile(fileName);
	if (!dataFile.open(QIODevice::ReadOnly)){
		qWarning("Couldn't open data file.");
		return;
	}
	QTextStream inData(&dataFile);
	QString text;
	QJsonArray dataArray;
	int id, x, y;
	float z, rsrp, calcfield, correction_calcfield;
	text = inData.readLine();

	while (!inData.atEnd())
	{
		text = inData.readLine();
		istringstream linestream(text.toStdString());

		linestream >> id >> x >> y >> z >> rsrp >> calcfield >> correction_calcfield;

		QJsonObject tempObject;
		tempObject.insert("ID", id);
		tempObject.insert("RSRP", rsrp);
		tempObject.insert("Correction_CalcField", correction_calcfield);
		dataArray.append(tempObject);
	}
	m_content.setSendText(dataArray);
}

void scatterWidget::openMeasuredFile()
{
	globalContext *globalCtx = globalContext::GetInstance();
	string fileName = globalCtx->cptManager->getMeasuredFilePath();
	//QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("csv Files (*.csv)"));
	//	ui->lineEdit->setText(fileName);
	globalCtx->visualManager->loadMeasuredFile(fileName);
}

void scatterWidget::showSimuResult()
{
	//int mode = modeGroup->checkedId();
	QJsonArray dataArray;

	//校正
	Vector3d before, after;
	globalContext *globalCtx = globalContext::GetInstance();
	globalCtx->visualManager->correct(before, after);
	//dataWidget->setPara(before.x, before.y, before.z, after.x, after.y, after.z);

	//推送数据到前端网页
	vector<double> beforeVector = globalCtx->visualManager->getBeforeVec();
	vector<double> measruedVector = globalCtx->visualManager->getMeasured();
	vector<double> afterVector = globalCtx->visualManager->getAfterVec();

	//生成json
	for (int i = 0; i < beforeVector.size() && i < measruedVector.size() && i < afterVector.size(); i++)
	{
		QJsonObject tempObject;
		tempObject.insert("ID", i);
		tempObject.insert("RSRP", measruedVector[i]);
		tempObject.insert("Before", beforeVector[i]);
		tempObject.insert("Correction", afterVector[i]);
		dataArray.append(tempObject);
	}
	
	QJsonObject tempObject1;
	tempObject1.insert("ID", -1);
	tempObject1.insert("RSRP", round(before.x*100)/100.0);
	tempObject1.insert("Before", round(before.y*100)/100.0);
	tempObject1.insert("Correction", round(before.z*100)/100.0);
	dataArray.append(tempObject1);

	QJsonObject tempObject2;
	tempObject2.insert("ID", -2);
	tempObject2.insert("RSRP", round(after.x*100)/100.0);
	tempObject2.insert("Before", round(after.y*100)/100.0);
	tempObject2.insert("Correction", round(after.z*100)/100.0);
	dataArray.append(tempObject2);

	m_content.setSendText(dataArray);
}

void scatterWidget::showLoadResult(QString path)
{
	globalContext *globalCtx = globalContext::GetInstance();
	ifstream in;
	in.open(path.toStdString(), ios::in);

	if (!in.is_open())
	{
		cout << "仿真文件打开失败！" << endl;
		return;
	}

	vector<double>beforeVector;
	vector<double>afterVector;
	vector<double>measuredVector;

	double before_mean;
	double before_deviation;
	double before_coefficient;
	double after_mean;
	double after_deviation;
	double after_coefficient;

	//读取文件头信息，判断是仿真面还是接收点
	string line;
	while (getline(in, line))
	{
		if (line.length() == 0)
			break;
		string fieldName;
		int fieldValue;

		istringstream istrm(line);
		istrm >> fieldName >> fieldValue;

		if (fieldName == "Computation_type" && fieldValue == 1)
		{
			QMessageBox::warning(NULL, "Error", QStringLiteral("仿真面缺少实测数据，无法显示！"));
			return;
		}
		if (fieldName == "Computation_type" && fieldValue == 2)
		{
			in >> fieldName >> before_mean >> after_mean;
			getline(in, line);
			in >> fieldName >> before_deviation >> after_deviation;
			getline(in, line);
			in >> fieldName >> before_coefficient >> after_coefficient;
			getline(in, line);
		}
	}
	
	//读取实测数据和仿真结果
	getline(in, line);       //跳过数据头部分(id,x,y,z,pci,real,simu)
	int id, pci;
	Vector3d position;
	double real, simu;
	while (!in.eof())
	{
		in >> id >> position.x >> position.y >> position.z >> pci >> real >> simu;
		measuredVector.push_back(real);
		beforeVector.push_back(0);
		afterVector.push_back(simu);
	}

	//生成json
	QJsonArray dataArray;
	for (int i = 0;  i < measuredVector.size(); i++)
	{
		QJsonObject tempObject;
		tempObject.insert("ID", i);
		tempObject.insert("RSRP", measuredVector[i]);
		tempObject.insert("Before", beforeVector[i]);
		tempObject.insert("Correction", afterVector[i]);
		dataArray.append(tempObject);
	}

	QJsonObject tempObject1;
	tempObject1.insert("ID", -1);
	tempObject1.insert("RSRP", round( before_mean* 100) / 100.0);
	tempObject1.insert("Before", round(before_deviation * 100) / 100.0);
	tempObject1.insert("Correction", round(before_coefficient * 100) / 100.0);
	dataArray.append(tempObject1);

	QJsonObject tempObject2;
	tempObject2.insert("ID", -2);
	tempObject2.insert("RSRP", round(after_mean * 100) / 100.0);
	tempObject2.insert("Before", round(after_deviation * 100) / 100.0);
	tempObject2.insert("Correction", round(after_coefficient * 100) / 100.0);
	dataArray.append(tempObject2);

	m_content.setSendText(dataArray);
}

void scatterWidget::closeEvent(QCloseEvent *e)
{
	emit sendFlag(true);
}

//void scatterWidget::showPara()
//{
//	dataWidget->show();
//}

//void scatterWidget::modeButtonToggled(int id, bool flag)
//{
//	int mode = modeGroup->checkedId();
//	switch (id)
//	{
//	case 1:
//		enableShowMode(flag);
//		break;
//	case 2:
//		enableCorrectMode(flag);
//		break;
//	}
//}

//void scatterWidget::initData()
//{
//
//}

//void scatterWidget::enableShowMode(bool a)
//{
//	//	ui->comboBox_Site->setEnabled(a);
//}

//void scatterWidget::enableCorrectMode(bool a)
//{
//	//	ui->lineEdit->setEnabled(a);
//	//	ui->pushButton->setEnabled(a);
//	//	ui->correct_Button->setEnabled(a);
//}
