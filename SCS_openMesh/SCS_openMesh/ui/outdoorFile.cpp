#include "outdoorFile.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
outdoorFileDialog::outdoorFileDialog(QWidget *parent ){
	ui.setupUi(this);
	createActions();
}

outdoorFileDialog::~outdoorFileDialog()
{

}



QStringList   outdoorFileDialog::getScene2DPath()
{
	if(O_Scene2DInfoFile_paths.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��ý������ļ�·��"),QString::fromLocal8Bit("���ȵ��뽨�����ļ���"));
		return QStringList();
	}
	return O_Scene2DInfoFile_paths;
}

QStringList  outdoorFileDialog::getSceneHeightPath()
{
	if (O_SceneHeightInfoFile_paths.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��ý�����߶��ļ�·��"),QString::fromLocal8Bit("���ȵ��뽨�����ļ���"));
		return QStringList();
	}
	return O_SceneHeightInfoFile_paths;
}

QString outdoorFileDialog::getPlanePath()
{
	if(O_ScenePlaneHeightInfoFile_path.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��ú����ļ�·��"),QString::fromLocal8Bit("���ȵ��뺣�����ļ���"));
		return NULL;
	}
	return O_ScenePlaneHeightInfoFile_path;
}



void outdoorFileDialog::openBuildingVector()
{
	QStringList paths = QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("���볡��vector�ļ�"),"./",QString::fromLocal8Bit(" ������ά��Ϣ�ļ� "));
	if (paths.isEmpty())
		return;
	O_Scene2DInfoFile_paths = paths;
	
	ui.checkBox_2->setChecked(true);
}

void outdoorFileDialog:: openHeight()
{
	QStringList paths = QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("���볡��vector�ļ�"),"./",QString::fromLocal8Bit(" �����߶���Ϣ�ļ�"));
	if (paths.isEmpty())
		return;
	O_SceneHeightInfoFile_paths = paths;
	ui.checkBox_4->setChecked(true);
}

void outdoorFileDialog::openPlane()
{
	QString path = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("������溣�� raster�ļ�"),"./",QString::fromLocal8Bit(" �������溣����Ϣ�ļ�"));
	if (path.isEmpty())
		return;
	O_ScenePlaneHeightInfoFile_path = path;
	ui.checkBox_3->setChecked(true);
}

void outdoorFileDialog::onOkButton()
{
	mapName=ui.lineEdit_name->text();
	if (ui.checkBox_2->isChecked()&&ui.checkBox_3->isChecked()&&ui.checkBox_4->isChecked()&& !mapName.isEmpty())
	{
		emit fileIsOK(mapName,O_Scene2DInfoFile_paths,O_SceneHeightInfoFile_paths,O_ScenePlaneHeightInfoFile_path);
		this->close();
	}else
	{
		QMessageBox::warning(this, QStringLiteral("�ļ�����"),QStringLiteral("�ļ�·��ȱʧ���뵼�������ļ���"));
	}

	return ;
}

void outdoorFileDialog::deleteBuilding()
{
	O_Scene2DInfoFile_paths.clear();
	ui.checkBox_2->setChecked(false);
}

void outdoorFileDialog::deleteHeight()
{
	O_SceneHeightInfoFile_paths.clear();
	ui.checkBox_4->setChecked(false);
}

void outdoorFileDialog::deletePlane()
{
	O_ScenePlaneHeightInfoFile_path.clear();
	ui.checkBox_3->setChecked(false);
}
void outdoorFileDialog::createActions()
{
	
	connect(ui.buildingButton,SIGNAL(clicked()),this,SLOT(openBuildingVector()));
	connect(ui.heightButton,SIGNAL(clicked()),this,SLOT(openHeight()));
	connect(ui.altitudeButton,SIGNAL(clicked()),this,SLOT(openPlane()));
	connect(ui.pushButton_LoadFile,SIGNAL(clicked()),this,SLOT(onOkButton()));
	connect(ui.pushButton_cancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(ui.pushButton_deleteV,SIGNAL(clicked()),this,SLOT(deleteBuilding()));
	connect(ui.pushButton_deleteH,SIGNAL(clicked()),this,SLOT(deleteHeight()));
	connect(ui.pushButton_deleteP,SIGNAL(clicked()),this,SLOT(deletePlane()));
}