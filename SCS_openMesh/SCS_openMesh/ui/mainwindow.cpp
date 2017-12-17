#include "mainwindow.h"
#include <QtWidgets/QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include "Antenna/receiver.h"
#include <QObject>
#include <util/stringUtil.h>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	init();
	createActions();
}

MainWindow::~MainWindow()
{
	if (scatter!=nullptr)
	{
		delete scatter;
		scatter = nullptr;
	}
	if (mod!=nullptr)
	{
		delete mod;
		mod = nullptr;
	}
	if (modelTable!=nullptr)
	{
		delete modelTable;
		modelTable = nullptr;
	}
	if (M_computeroptionDialog!=nullptr)
	{
		delete M_computeroptionDialog;
		M_computeroptionDialog = nullptr;
	}
	if (M_outdoorFileDialog!=nullptr)
	{
		delete M_outdoorFileDialog;
		M_outdoorFileDialog = nullptr;
	}
}

void MainWindow::init()
{
	this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

	M_outdoorFileDialog=new outdoorFileDialog(this);
	M_computeroptionDialog=new computerOptionDialog(this);

	mod=new meshOptionDialog(this);
	scatter = new scatterWidget(this);

	//将表格嵌入到dockwidget
	modelTable = new scsModelTable;
	modelTable->showMaximized();
	ui.dockWidget_Property->setWidget(modelTable);

	globalContext *gctx = globalContext::GetInstance();
	gctx->modelManager->getModelSubject()->attach(modelTable);
	gctx->modelManager->getLocalSubject()->attach(M_computeroptionDialog->fp);
	gctx->modelManager->getLocalSubject()->attach(modelTable);
	gctx->cptManager->getSubject()->attach(M_computeroptionDialog->es);

	catalog = new catalogWidget;
	ui.dockWidget_outline->setWidget(catalog);
	gctx->modelManager->getModelSubject()->attach(catalog);
	gctx->cptManager->getPluginSubject()->attach(catalog);
	
	ui.progressBar->setRange(0,100);

	//设置对话框的默认数据
	M_computeroptionDialog->sa->lineEdit_RT_ReflectCount->setText(QString::number(2));
	M_computeroptionDialog->sa->lineEdit_RT_RefractCount->setText(QString::number(1));
	M_computeroptionDialog->sa->lineEdit_RT_RefractCount->setReadOnly(true);
	M_computeroptionDialog->sa->lineEdit_RT_sample->setText(QString::number(1));
	M_computeroptionDialog->sa->lineEdit_RT_sample_radius->setText(QString::number(1.0));


}

void  MainWindow::openOutdoorFile()
{
	if (M_outdoorFileDialog==NULL)
	{
		M_outdoorFileDialog=new outdoorFileDialog(this);
	}
	M_outdoorFileDialog->exec();

}

void MainWindow::computerOption()
{
	if (M_computeroptionDialog==NULL)
	{
		M_computeroptionDialog=new computerOptionDialog(this);
	}
	M_computeroptionDialog->exec();
}

void MainWindow::setProgress(int value)
{
	ui.progressBar->setValue(value);
}

void MainWindow::createActions()
{
	connect(ui.action_3, SIGNAL(triggered()), this, SLOT(openOutdoorFile()));
	connect(ui.computerOption, SIGNAL(triggered()), this, SLOT(computerOption()));
	connect(M_outdoorFileDialog, SIGNAL(fileIsOK(QString, QStringList, QStringList, QString)), this, SLOT(loadAllFile(QString, QStringList, QStringList, QString)));
	connect(ui.action_ShowAll, SIGNAL(triggered()), this, SLOT(showAll()));
	connect(ui.action_localscene, SIGNAL(triggered()), this, SLOT(showLocal()));
	connect(ui.action_obj, SIGNAL(triggered()), this, SLOT(loadObj()));
	connect(ui.action_9, SIGNAL(triggered()), this, SLOT(setMaterial()));
	connect(ui.action_matFile, SIGNAL(triggered()), this, SLOT(open_material()));
	connect(ui.action_6, SIGNAL(triggered()), this, SLOT(setMeshOption()));
	connect(ui.action_startMesh, SIGNAL(triggered()), this, SLOT(meshAll()));
	connect(ui.action_saveLocal, SIGNAL(triggered()), this, SLOT(saveLocalScene()));
	connect(ui.action_loadPlugin, SIGNAL(triggered()), this, SLOT(loadPlugin()));
	connect(ui.action_run, SIGNAL(triggered()), this, SLOT(run()));
	connect(ui.action_json, SIGNAL(triggered()), this, SLOT(quickLoadJson()));
	connect(ui.action_showPoint, SIGNAL(triggered(bool)), this, SLOT(setDrawPointMode(bool)));
	connect(ui.action_showLine, SIGNAL(triggered(bool)), this, SLOT(setDrawLineMode(bool)));
	connect(ui.action_showFace, SIGNAL(triggered(bool)), this, SLOT(setDrawFaceMode(bool)));
	connect(ui.action_GenerateModelPara, SIGNAL(triggered()), this, SLOT(generateModelPara()));
	connect(ui.action_scatter, SIGNAL(triggered()), this, SLOT(showScatterWidget()));
	connect(ui.action_saveResult, SIGNAL(triggered()), this, SLOT(saveAllResult()));
	connect(ui.horizontalSlider_Scene_Alpha, SIGNAL(valueChanged(int)), this, SLOT(setModelAlpha(int)));
	connect(catalog, SIGNAL(modelID_ShowChanged(int)), this, SLOT(setShowLineEdit(int)));
}

void MainWindow::generateModelPara()
{
	globalContext *globalCtx = globalContext::GetInstance();
	if (!globalCtx->modelManager->checkLocalExist())
	{
		outputLog(QStringLiteral("error: 没有局部场景，生成模型参数失败！"));
		return;
	}
	globalCtx->modelManager->setModelPara();
	outputLog(QStringLiteral("success: 生成模型参数成功！"));
}

/**
  * @Method:    saveAllResult
  * @Note: 	保存所有仿真文件
  * @Author:    Li Gen
  * @ Date:     2017/12/13
  * @Returns:   void
*/
void MainWindow::saveAllResult()
{
	
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择保存文件夹"),
		"C:",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

	globalContext *globalCtx = globalContext::GetInstance();
	EFieldContainer* visualDataContainer = globalCtx->visualManager->getContainer();
	if (!visualDataContainer->isDataExist())
	{
		qDebug()<< "error: no simulation data" << endl;
		QMessageBox::warning(this, "Error", QStringLiteral("error: 没有仿真结果文件"));
		return;
	}

	vector<int> siteIDs = visualDataContainer->getAllSiteID();

	QProgressDialog process(this);
	process.setLabelText(QStringLiteral("保存仿真文件中"));
	process.setRange(0, siteIDs.size());
	process.setModal(true);
	process.setCancelButtonText(tr("cancel"));
	process.setValue(0);

	for (int i = 0; i < siteIDs.size(); i++)
	{
		vector<int> pcis = visualDataContainer->getPCIsBySiteID(siteIDs[i]);
		for (int j = 0; j < pcis.size(); j++)
		{
			QString filePath = dir.append("_Site").append(QString::number(siteIDs[i])).append("_Cell");
			filePath.append(QString::number(pcis[j])).append(".json");
			globalCtx->visualManager->saveCellFile(filePath, pcis[j], siteIDs[i]);
			qDebug ()<< "save cell success ! the pci is" << pcis[j] << endl;
		}

		process.setValue(i + 1);
		if (process.wasCanceled())
		{
			break;
		}
		qDebug() << "------------save Site success!  site id " << siteIDs[i] << "-----------------" << endl;
	}

	return;
}

void MainWindow::setShowLineEdit(int id)
{
	ui.lineEdit_ModelID->setText(QString::number(id));
}

void MainWindow::setModelAlpha(int a)
{
	ui.simuArea->setModelAlpha(a);
}

void MainWindow::setDrawPointMode(bool flag)
{
	//用二进制表达状态
	ui.simuArea->setPoint(flag);
}
void MainWindow::setDrawLineMode(bool flag)
{
	//用二进制表达状态
	ui.simuArea->setLine(flag);
}
void MainWindow::setDrawFaceMode(bool flag)
{
	//用二进制表达状态
	ui.simuArea->setFace(flag);
}




void MainWindow::saveLocalScene()
{
	
}


void MainWindow::open_material()
{
	
	QString path = QFileDialog::getOpenFileName(this,QStringLiteral("打开材质文件"),"./",QStringLiteral("txt 材质文件 (*.txt)"));
	load_Material(path.toStdString());
}
void MainWindow:: load_Material(string path)
{
	globalContext *globalCtx = globalContext::GetInstance();
	globalCtx->modelManager->matManager->addMatertial(path);
}
void MainWindow::setMeshOption()
{
	if (mod == NULL)
	{
		mod = new meshOptionDialog(this);
	}
	mod->exec();
}

void MainWindow::setMaterial()
{
	bool ok;
	int i = QInputDialog::getInt(this, QStringLiteral("材质信息"),
		QStringLiteral("编号:"), 10, 0, 78, 1, &ok);
	if (ok)
	{
		globalContext *globalCtx = globalContext::GetInstance();
		globalCtx->modelManager->matManager->setDefault(i);
	}
		
	outputLog(QString(QStringLiteral("设置材质编号为：")+QString::number(i,10)));
}



/************************************************************************/
/* 根据路径，读取文件，存放到对应的变量中                                              */
/************************************************************************/
void MainWindow::loadAllFile(QString _name,QStringList _v,QStringList _h,QString _p){
	
}

void MainWindow::outputLog(QString source)
{
	ui.textBrowser->append(source);
}


/************************************************************************/
/*    展示全部场景                                                                                       */
/************************************************************************/
void MainWindow::showAll()
{

}

void MainWindow::showLocal()
{
	
}

void MainWindow::showScatterWidget()
{
	scatter->show();
}

/************************************************************************/
/* 导入obj文件                                                                     */
/************************************************************************/
void MainWindow::loadObj()
{
	
}

/************************************************************************/
/* 剖分选中的地面和建筑物                                                                     */
/************************************************************************/
void MainWindow::meshAll()
{
	setProgress(0);
	globalContext *gctx = globalContext::GetInstance();
	if (!gctx->modelManager->checkCityExist())
	{
		outputLog(QStringLiteral("没有导入建筑物文件，请先导入相关文件。"));
		return;
	}
	else if ((gctx->modelManager->checkLocalExist()))
	{
		outputLog(QStringLiteral("局部场景已有模型，请先删除。"));
		return;
	}

	vector< Vector3d> center;
	vector<int> siteID;
	double range;
	//要不要加一个判断，防止错误？
	if (mod->inputFlag)
	{
		mod->getValue(center,siteID,range);
	}
	else
	{
		outputLog(QStringLiteral("没有设置剖分参数"));
		return;
	}
	gctx->modelManager->generateLocalModel(center,siteID, range);
}






void MainWindow::loadPlugin()
{
	globalContext *gctx = globalContext::GetInstance();
	QString path = QFileDialog::getOpenFileName(this,QStringLiteral("加载算法插件"),"./dll_plugins/","*.dll");  
	if (!path.isEmpty())
	{
		gctx->cptManager->setPluginPath(path);
		outputLog(QStringLiteral("插件路径设置成功"));
	}else
	{
		outputLog(QStringLiteral("插件路径设置失败"));
	}
}

void MainWindow::deletePlugin()
{
	
}

void MainWindow::run()
{
	QObject* object ;
	globalContext *gctx = globalContext::GetInstance();
	QString  plugin_file_path = gctx->cptManager->getPluginPath();
	QPluginLoader loader( plugin_file_path); 

	if ((object=loader.instance())!=NULL)
	{
		ComputeInterface * pluginTemp=qobject_cast<ComputeInterface*>(object);
		if (pluginTemp)
		{
			outputLog(QStringLiteral("开始运行计算函数"));	
			pluginTemp->runAlgorithm(gctx->modelManager->getModelPara(),gctx->cptManager->getComputationPara(),gctx->visualManager->getVisualPara());
		    outputLog(QStringLiteral("结束计算"));
			gctx->visualManager->setContainerData();
			outputLog(QStringLiteral("显示结果"));
		}
	}
}


/************************************************************************/
/* 读取json文件    参考 http://bbs.itheima.com/thread-331656-1-1.html */
/************************************************************************/
void MainWindow::quickLoadJson()
{

	QString path = QFileDialog::getOpenFileName(this,QStringLiteral("快速导入场景"),"./",QStringLiteral("场景文件 (*.json)"));
	if (path.isNull())
	{
		return;
	}
	globalContext *gctx = globalContext::GetInstance();
	gctx->modelManager->loadCityModel(path.toStdString());

	//获取json文件所在文件夹的目录,为读材质文件准备好路径
	QDir jsonPath(path);
	jsonPath.cdUp();
	QString fatherDirectory = jsonPath.path();
	fatherDirectory.append("/");

	QFile file(path);
	file.open(QIODevice::ReadWrite);
	QByteArray json = file.readAll();
	QJsonDocument jsDoc;
	jsDoc = QJsonDocument::fromJson(json);

	QString _m;
	if (jsDoc.isObject())
	{
		QJsonObject obj = jsDoc.object();
		//检测材料
		if (obj.contains("Material"))
		{
			QJsonValue name_value = obj["Material"];
			if (name_value.isString())
			{
				_m = fatherDirectory + name_value.toString();
			}
		}
		else
		{
			cout << "error: json中没有材质文件，请检查！" << endl;
		}
	}
	load_Material(_m.toStdString());
}