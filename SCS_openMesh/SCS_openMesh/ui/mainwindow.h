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

#include "Model/abstractModel.h"
#include "Model/abstractModelFactory.h"
#include "Model/cityModel/cityModel.h"
#include "Model/cityModel/cityModelFactory.h"
#include "scsModelTable.h"



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
	emxModel* getEmxModel();
	void setModelName(int id,QString name);//设置treewidget中的名字
	void outputLog(QString source);
	void setProgress(int value);

private slots:
	void openOutdoorFile();
	void computerOption();
	void loadAllFile(QString _name,QStringList _v,QStringList _h,QString _p);
	void showAll();  
	void showLocal();
	void openNo_SimplaneReceiverFile();
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
	void saveSimuPlane();
	void loadSimuPlane();
	void setDrawPointMode(bool);
	void setDrawLineMode(bool);
	void setDrawFaceMode(bool);
	void generateModelPara();
	void generateCptpara();

private:
	void load_Material(string path);

	//内部界面控件
private:
	Ui::MainWindowClass ui;
	QTreeWidgetItem* modelTW;
	QTreeWidgetItem* computeTW;
	QTreeWidgetItem* visualTW;


	outdoorFileDialog*  M_outdoorFileDialog;
	computerOptionDialog* M_computeroptionDialog;

	meshOptionDialog *mod;

	scsModelTable* modelTable;
	//内部变量
 private:
	 QString material_path;

	 /************************************************************************/
	 /* 整个场景 建筑物                                                                     */
	 /************************************************************************/

	 /************************************************************************/
	 /* 局部场景 三角面片                                                                     */
	 /************************************************************************/
	 QString objName;
	 QString OBJFile_path;
 
	 /************************************************************************/
	 /* plugin 参数                                                                     */
	 /************************************************************************/
	 QString plugin_file_path;

	 void init();
	 void createActions();
};
#endif // MAINWINDOW_H
