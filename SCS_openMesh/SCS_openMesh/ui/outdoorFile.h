#ifndef _OUTDOORFILE_H_
#define _OUTDOORFILE_H_

#include "GeneratedFiles/ui_outdoorFile.h"
class outdoorFileDialog : public QDialog
{
	Q_OBJECT
public:
	outdoorFileDialog(QWidget *parent = 0);
	~outdoorFileDialog();
	QStringList  getScene2DPath();//������
	QStringList  getSceneHeightPath();//�߶�
	QString getPlanePath(); //���� 
	void createActions();
private slots:
		
		void openBuildingVector();
		void openHeight();
		void openPlane();
		void onOkButton();
		void deleteBuilding();
		void deleteHeight();
		void deletePlane();
		void onCancelButton();
signals:  
		void  fileIsOK(QString name,QStringList v,QStringList h,QString p);  

private:
	Ui::outdoorFile ui;
public:

	QString mapName;
	QString O_Scene2DInfoFile_path;//�������ļ�
	QStringList  O_Scene2DInfoFile_paths;
	
	QString O_SceneHeightInfoFile_path;//������߶��ļ�
	QStringList O_SceneHeightInfoFile_paths;

	QString O_ScenePlaneHeightInfoFile_path;//�����ļ�
};

#endif // !_OUTDOORFILE_H_
