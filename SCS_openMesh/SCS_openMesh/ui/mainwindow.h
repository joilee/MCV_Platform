#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "GeneratedFiles/ui_mainwindow.h"
#include "outdoorFile.h"
#include "InfoPage.h"
#include "computerOptionDialog.h"

#include "meshOption.h"
#include "plugin/PluginInterface.h"
#include <QtWidgets/QMessageBox>
#include "Context/context.h"

#include "Model/abstractModel/abstractModel.h"
#include "Model/abstractModel/abstractModelFactory.h"
#include "Model/cityModel/cityModel.h"
#include "Model/cityModel/cityModelFactory.h"
#include "scsModelTable.h"
#include "catalogWidget/catalogWidget.h"

#include "ui/scatterWidget/scatterWidget.h"


class QAction; 
struct building;

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();


public:
	void outputLog(QString source);
	void setProgress(int value);

private slots:
	void openOutdoorFile();
	void computerOption();
	void loadAllFile(QString _name,QStringList _v,QStringList _h,QString _p);
	void showScatterWidget();
	void loadObj();
	void setMaterial();
	void open_material();
	void setMeshOption();
	void meshAll();
	void saveLocalScene();
	void loadPlugin();
	void deletePlugin();
	void run();
	void quickLoadJson();
	void setDrawPointMode(bool);
	void setDrawLineMode(bool);
	void setDrawFaceMode(bool);
	void generateModelPara();
	void saveAllResult();
	void setShowLineEdit(int);
	void setModelAlpha(int);
	void setPlaneAlpha(int);
	void setSimuPlane(int, int);
	void loadResult();
	void updateScatter(bool);
	void setMeshOptionLog(bool);
	void setCalOptionLog(bool);
	void openManual();
private:
	void load_Material(string path);

	//内部界面控件
private:
	Ui::MainWindowClass ui;

	outdoorFileDialog*  M_outdoorFileDialog;
	computerOptionDialog* M_computeroptionDialog;

	meshOptionDialog *mod;
	scatterWidget *scatter;
	scsModelTable* modelTable;
	catalogWidget * catalog;
	QProcess *pro;
	//内部变量
 private:
	 QString material_path;

	 /************************************************************************/
	 /* 局部场景 三角面片                                                                     */
	 /************************************************************************/
	 QString objName;
	 QString OBJFile_path;
	 QString result_path;
 
	 void init();
	 void createActions();

};
#endif // MAINWINDOW_H
