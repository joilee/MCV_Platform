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

using namespace std;

scatterWidget::scatterWidget(QWidget *parent) :QDialog(parent), ui(new Ui::scatterDialog)
{
	ui->setupUi(this);
	connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(openTestFile()));
	webPage *page = new webPage(this);
	ui->preview->setPage(page);
	QWebChannel *channel = new QWebChannel(this);
	channel->registerObject(QStringLiteral("content"), &m_content);
	page->setWebChannel(channel);
	ui->preview->setUrl(QUrl("qrc:/MainWindow/Resources/echarts/index.html"));
	initData();
}



scatterWidget::~scatterWidget()
{
	delete ui;
}

void scatterWidget::openTestFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("All Files (*.*)"));
	QFile dataFile(fileName);
	if (!dataFile.open(QIODevice::ReadOnly)){
		qWarning("Couldn't open data file.");
		return;
	}
	QTextStream inData(&dataFile);
	QString text;

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

void scatterWidget::showResult()
{

}

void scatterWidget::setSourceEField(vector<EField> &_placePoint)
{
	placePoint = _placePoint;
}

void scatterWidget::initData()
{
	placePoint.clear();
	testPoint.clear();
}
