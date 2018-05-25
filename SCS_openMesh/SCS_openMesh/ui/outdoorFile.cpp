#include "outdoorFile.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
outdoorFileDialog::outdoorFileDialog(QWidget *parent ){
	ui.setupUi(this);
	ui.lineEdit_name->setText(QString("map1"));
	createActions();
}

outdoorFileDialog::~outdoorFileDialog()
{

}



QStringList outdoorFileDialog::getScene2DPath()
{
	if(O_Scene2DInfoFile_paths.isEmpty())
	{
		QMessageBox::warning(this, "Error", QStringLiteral("请先导入建筑物文件！"));
		return QStringList();
	}
	return O_Scene2DInfoFile_paths;
}

QStringList  outdoorFileDialog::getSceneHeightPath()
{
	if (O_SceneHeightInfoFile_paths.isEmpty())
	{
		QMessageBox::warning(this, "Error", QStringLiteral("请先导入建筑物文件！"));
		return QStringList();
	}
	return O_SceneHeightInfoFile_paths;
}

QString outdoorFileDialog::getPlanePath()
{
	if(O_ScenePlaneHeightInfoFile_path.isEmpty())
	{
		QMessageBox::warning(this, "Error", QStringLiteral("请先导入海拔文件！"));
		return NULL;
	}
	return O_ScenePlaneHeightInfoFile_path;
}



void outdoorFileDialog::openBuildingVector()
{
	QStringList paths = QFileDialog::getOpenFileNames(this, QStringLiteral("导入场景二维信息文件"), "./", QStringLiteral(" 场景二维信息文件(*.*)"));
	if (paths.isEmpty())
		return;
	O_Scene2DInfoFile_paths = paths;
	
	ui.checkBox_2->setChecked(true);
}

void outdoorFileDialog:: openHeight()
{
	QStringList paths = QFileDialog::getOpenFileNames(this, QStringLiteral("导入场景高度信息文件"), "./", QStringLiteral(" 场景高度信息文件(*.*)"));
	if (paths.isEmpty())
		return;
	O_SceneHeightInfoFile_paths = paths;
	ui.checkBox_4->setChecked(true);
}

void outdoorFileDialog::openPlane()
{
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("导入场景地面海拔信息文件"), "./", QStringLiteral(" 场景地面海拔信息文件(*.*)"));
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
		if (O_Scene2DInfoFile_paths.empty())
			QMessageBox::warning(NULL, "Error", QStringLiteral("场景二维信息文件路径有误，请重新导入！"));
		else if (O_SceneHeightInfoFile_paths.empty())
			QMessageBox::warning(NULL, "Error", QStringLiteral("场景高度信息文件路径有误，请重新导入！"));
		else if (O_ScenePlaneHeightInfoFile_path.isEmpty())
			QMessageBox::warning(NULL, "Error", QStringLiteral("场景地面海拔信息文件路径有误，请重新导入！"));
		else
		{
			this->close();
			emit fileIsOK(mapName, O_Scene2DInfoFile_paths, O_SceneHeightInfoFile_paths, O_ScenePlaneHeightInfoFile_path);
		}
		
	}
	else
	{
		QMessageBox::warning(this, "Error",QStringLiteral("文件路径缺失，请导入所有文件！"));
	}
	return;
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

void outdoorFileDialog::onCancelButton()
{
	this->close();
	return;
}
void outdoorFileDialog::createActions()
{
	
	connect(ui.buildingButton,SIGNAL(clicked()),this,SLOT(openBuildingVector()));
	connect(ui.heightButton,SIGNAL(clicked()),this,SLOT(openHeight()));
	connect(ui.altitudeButton,SIGNAL(clicked()),this,SLOT(openPlane()));
	connect(ui.pushButton_LoadFile,SIGNAL(clicked()),this,SLOT(onOkButton()));
	connect(ui.pushButton_cancel,SIGNAL(clicked()),this,SLOT(onCancelButton()));
	connect(ui.pushButton_deleteV,SIGNAL(clicked()),this,SLOT(deleteBuilding()));
	connect(ui.pushButton_deleteH,SIGNAL(clicked()),this,SLOT(deleteHeight()));
	connect(ui.pushButton_deleteP,SIGNAL(clicked()),this,SLOT(deletePlane()));
}


