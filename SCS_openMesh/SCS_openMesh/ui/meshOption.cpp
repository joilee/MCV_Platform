﻿#include "meshOption.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>
#include "Context/context.h"
#include <QMessageBox>
void meshOptionDialog::getValue(vector<Vector3d> &_center,double &_range) {
    _center=center;
    _range=range;
}

void meshOptionDialog::enableSingleMode(bool flag) {
    centerXLE->setEnabled(flag);
    centerYLE->setEnabled(flag);
    centerZLE->setEnabled(flag);
    refreshButton->setEnabled(flag);
    cbo_XYZ->setEnabled(flag);

}

void meshOptionDialog::enableMultiMode(bool flag) {

}

void meshOptionDialog::onOkbutton() {
    if (modelNum==ModelNum::SINGLE_MODEL) {
        if (centerXLE->text().isEmpty() || centerYLE->text().isEmpty() || centerZLE->text().isEmpty() || rangeLE->text().isEmpty()) {
            QMessageBox::critical(this, QStringLiteral("输入"), QStringLiteral("中心点或者范围输入为空，请重新输入"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        double x = centerXLE->text().toDouble();
        double y = centerYLE->text().toDouble();
        double z = centerZLE->text().toDouble();
        range = rangeLE->text().toDouble();
        center.clear();
        center.push_back(Vector3d(x, y, z));
        inputFlag = true;
    } else if (modelNum==ModelNum::MULTI_MODEL) {
        if (rangeLE->text().isEmpty()) {
            QMessageBox::critical(this, QStringLiteral("输入"), QStringLiteral("范围输入为空，请重新输入"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        //将site的数据全部加入到center中
        globalContext *globalCtx = globalContext::GetInstance();
		int len = globalCtx->cptManager->getComputationPara()->Sites.size();
        if ( len!= 0) {
			center.clear();
            for (int i = 0; i < len; i++) {
				center.push_back(globalCtx->cptManager->getSitePosition(i));
            }
        } else {
            QMessageBox::critical(this, QStringLiteral("基站"), QStringLiteral("基站为空，请导入站点文件"), QMessageBox::Yes, QMessageBox::Yes);
        }
    }
	QString s;
	s.append(QStringLiteral("成功添加"));
	s.append(QString::number(center.size())).append(QStringLiteral("个剖分中心点"));


	QMessageBox::information(this, QStringLiteral("剖分选项"), s);
    this->close();
}

meshOptionDialog::meshOptionDialog(QWidget *parent): QDialog(parent) {

    QRegExp rx("^[-+]?[0-9]+(\.[0-9]+)?$");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    center.clear();
    range=0;
    inputFlag=false;

    centerXLE=new QLineEdit;
    centerXLE->setValidator(pReg);
    centerYLE=new QLineEdit;
    centerYLE->setValidator(pReg);
    centerZLE=new QLineEdit;
    centerZLE->setValidator(pReg);
    rangeLE=new QLineEdit;
    rangeLE->setValidator(new QIntValidator(0,9999999,this));
    this->setWindowTitle(QStringLiteral("剖分选项"));
    okbutton=new QPushButton(QStringLiteral("确定"));

    exitbutton=new QPushButton(QStringLiteral("取消"));
    cbo_XYZ=new QComboBox();
    cbo_XYZ->addItem(QStringLiteral("自定义"));
    refreshButton=new QPushButton(QStringLiteral("刷新"));
    refreshButton->setFixedWidth(40);

    meshModeGroup = new QButtonGroup(this);
    singleModelMode = new QRadioButton(QStringLiteral("单站点"));
    MultiModelMode = new QRadioButton(QStringLiteral("多站点"));
    meshModeGroup->addButton(singleModelMode,1);
    meshModeGroup->addButton(MultiModelMode,2);
    singleModelMode->setChecked(true);
    this->setTheLayout();


    createActions();
}




void meshOptionDialog::createActions() {
    connect(this->okbutton,SIGNAL(clicked()),this,SLOT(onOkbutton()));
    connect(this->exitbutton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(this->refreshButton,SIGNAL(clicked()),this,SLOT(onRefreshButton()));
    connect(cbo_XYZ,SIGNAL(currentIndexChanged(int)),this,SLOT(dynamicLoadSite(int)));
    connect(meshModeGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(meshModeButtonToggled(int, bool)));
}


/************************************************************************/
/* 查看全局变量中的sites小区数量，更新下拉框                                          */
/************************************************************************/
void meshOptionDialog::onRefreshButton() {
    globalContext *globalCtx=globalContext::GetInstance();

    if (globalCtx->cptManager->getComputationPara()->Sites.size()!=0) {
        cbo_XYZ->blockSignals(true);
        std::vector<Site> &tmpSite = globalCtx->cptManager->getSite();
        cbo_XYZ->clear();
        cbo_XYZ->addItem(QStringLiteral("自定义"));
        for(int i=0; i<tmpSite.size(); i++) {
            stringstream stream;
            stream<<tmpSite[i].Site_Name;
            string tmp=stream.str();
            QString sitename=QString::fromStdString(tmp);
            cbo_XYZ->addItem(QStringLiteral("小区")+sitename);
        }
        cbo_XYZ->blockSignals(false);
    }
}
void meshOptionDialog::dynamicLoadSite(int index) {
    if (index==0) {
        centerXLE->setReadOnly(false);
        centerYLE->setReadOnly(false);
        centerZLE->setReadOnly(false);
        return;
    } else {
        globalContext *globalCtx=globalContext::GetInstance();
        std::vector<Site> &tmpSite = globalCtx->cptManager->getSite();
        Vector3d AP_postion=tmpSite[index-1].Site_Antennas[0].position;
        centerXLE->setText(QString::number(AP_postion.x));
        centerYLE->setText(QString::number(AP_postion.y,'g',7));
        centerZLE->setText(QString::number(AP_postion.z));
        centerXLE->setReadOnly(true);
        centerYLE->setReadOnly(true);
        centerZLE->setReadOnly(true);
    }
    return;
}

void meshOptionDialog::meshModeButtonToggled(int id, bool flag) {
    int mode = meshModeGroup->checkedId();
    modelNum = ModelNum(mode);
    switch (id) {
    case 1:
        enableSingleMode(flag);
        break;
    case 2:
        enableMultiMode(flag);
        break;
    default:
        break;
    }
}

meshOptionDialog::~meshOptionDialog() {

}
void meshOptionDialog::setTheLayout() {
    firstGroup=new QGroupBox(QStringLiteral("剖分范围"));

    QHBoxLayout* minusOneLayout = new QHBoxLayout;
    minusOneLayout->addWidget(singleModelMode);
    minusOneLayout->addWidget(MultiModelMode);

    QHBoxLayout* zerolayout=new QHBoxLayout;
    QLabel *label0=new QLabel(QStringLiteral("来源"));
    zerolayout->addWidget(label0);
    zerolayout->addWidget(cbo_XYZ);
    zerolayout->addWidget(refreshButton);

    QHBoxLayout *firstLayout=new QHBoxLayout;
    QLabel * label1=new QLabel(QStringLiteral("中心点"));
    QLabel * label2=new QLabel(QStringLiteral("X:"));
    QLabel * label3=new QLabel(QStringLiteral("Y:"));
    QLabel * label4=new QLabel(QStringLiteral("Z:"));
    firstLayout->addWidget(label1);
    firstLayout->addWidget(label2);
    firstLayout->addWidget(centerXLE);
    firstLayout->addWidget(label3);
    firstLayout->addWidget(centerYLE);
    firstLayout->addWidget(label4);
    firstLayout->addWidget(centerZLE);

    QLabel * label5=new QLabel(QStringLiteral("剖分范围"));
    QHBoxLayout *secondLayout=new QHBoxLayout;
    secondLayout->addWidget(label5);
    secondLayout->addWidget(rangeLE);
    QVBoxLayout *v1=new QVBoxLayout;
    v1->addLayout(minusOneLayout);
    v1->addLayout(zerolayout);
    v1->addLayout(firstLayout);
    v1->addLayout(secondLayout);
    firstGroup->setLayout(v1);

    QHBoxLayout *thirdlayout=new QHBoxLayout;
    thirdlayout->addWidget(okbutton);
    thirdlayout->addWidget(exitbutton);
    thirdlayout->setAlignment(Qt::AlignRight);



    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(firstGroup);
    mainLayout->addLayout(thirdlayout);
    setLayout(mainLayout);
}