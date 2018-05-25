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
	setWindowIcon(QIcon("UEES.ico"));
	pro = NULL;
}

MainWindow::~MainWindow()
{
	if (scatter != nullptr)
	{
		delete scatter;
		scatter = nullptr;
	}
	if (mod != nullptr)
	{
		delete mod;
		mod = nullptr;
	}
	if (modelTable != nullptr)
	{
		delete modelTable;
		modelTable = nullptr;
	}
	if (M_computeroptionDialog != nullptr)
	{
		delete M_computeroptionDialog;
		M_computeroptionDialog = nullptr;
	}
	if (M_outdoorFileDialog != nullptr)
	{
		delete M_outdoorFileDialog;
		M_outdoorFileDialog = nullptr;
	}
	if (pro != NULL)
	{
		pro->terminate();
		pro->waitForFinished();
		delete pro;
	}
}

void MainWindow::init()
{
	this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

	M_outdoorFileDialog = new outdoorFileDialog(this);
	M_computeroptionDialog = new computerOptionDialog(this);

	mod = new meshOptionDialog(this);
	scatter = new scatterWidget(this);

	//将表格嵌入到dockwidget
	modelTable = new scsModelTable;
	modelTable->showMaximized();
	ui.dockWidget_Property->setWidget(modelTable);
	QStringList headers;
	headers << QStringLiteral("属性") << QStringLiteral("值");
	modelTable->setHorizontalHeaderLabels(headers);

	globalContext *gctx = globalContext::GetInstance();
	gctx->modelManager->getModelSubject()->attach(modelTable);
	gctx->modelManager->getLocalSubject()->attach(M_computeroptionDialog->fp);
	gctx->modelManager->getLocalSubject()->attach(modelTable);
	gctx->cptManager->getSubject()->attach(M_computeroptionDialog->es);

	catalog = new catalogWidget;
	ui.dockWidget_outline->setWidget(catalog);
	gctx->modelManager->getModelSubject()->attach(catalog);
	gctx->cptManager->getPluginSubject()->attach(catalog);
	gctx->visualManager->getSiteSubject()->attach(catalog);

	ui.progressBar->setRange(0, 100);

	//设置对话框的默认数据
	M_computeroptionDialog->sa->lineEdit_RT_ReflectCount->setText(QString::number(2));
	M_computeroptionDialog->sa->lineEdit_RT_RefractCount->setText(QString::number(1));
	M_computeroptionDialog->sa->lineEdit_RT_RefractCount->setReadOnly(true);
	M_computeroptionDialog->sa->lineEdit_RT_sample->setText(QString::number(1));
	M_computeroptionDialog->sa->lineEdit_RT_sample_radius->setText(QString::number(1.0));


}

void MainWindow::createActions()
{
	connect(ui.action_matFile, SIGNAL(triggered()), this, SLOT(open_material()));
	connect(ui.action_3, SIGNAL(triggered()), this, SLOT(openOutdoorFile()));
	connect(M_outdoorFileDialog, SIGNAL(fileIsOK(QString, QStringList, QStringList, QString)), this, SLOT(loadAllFile(QString, QStringList, QStringList, QString)));
	connect(ui.action_obj, SIGNAL(triggered()), this, SLOT(loadObj()));
	connect(ui.action_saveLocal, SIGNAL(triggered()), this, SLOT(saveLocalScene()));
	connect(ui.action_saveResult, SIGNAL(triggered()), this, SLOT(saveAllResult()));
	connect(ui.action_loadResult, SIGNAL(triggered()), this, SLOT(loadResult()));
	connect(ui.action_startMesh, SIGNAL(triggered()), this, SLOT(meshAll()));
	connect(ui.computerOption, SIGNAL(triggered()), this, SLOT(computerOption()));
	connect(ui.action_9, SIGNAL(triggered()), this, SLOT(setMaterial()));
	connect(ui.action_loadPlugin, SIGNAL(triggered()), this, SLOT(loadPlugin()));
	connect(ui.action_deletePlugin, SIGNAL(triggered()), this, SLOT(deletePlugin()));
	connect(ui.action_run, SIGNAL(triggered()), this, SLOT(run()));
	connect(ui.action_json, SIGNAL(triggered()), this, SLOT(quickLoadJson()));
	connect(ui.action_showPoint, SIGNAL(triggered(bool)), this, SLOT(setDrawPointMode(bool)));
	connect(ui.action_showLine, SIGNAL(triggered(bool)), this, SLOT(setDrawLineMode(bool)));
	connect(ui.action_showFace, SIGNAL(triggered(bool)), this, SLOT(setDrawFaceMode(bool)));
	connect(ui.action_scatter, SIGNAL(triggered()), this, SLOT(showScatterWidget()));
	connect(ui.horizontalSlider_Scene_Alpha, SIGNAL(valueChanged(int)), this, SLOT(setModelAlpha(int)));
	connect(ui.horizontalSlider_Plane_Alpha, SIGNAL(valueChanged(int)), this, SLOT(setPlaneAlpha(int)));
	connect(catalog, SIGNAL(modelID_ShowChanged(int)), this, SLOT(setShowLineEdit(int)));
	connect(catalog, SIGNAL(planeID_Changed(int, int)), this, SLOT(setSimuPlane(int, int)));
	connect(scatter, SIGNAL(sendFlag(bool)), this, SLOT(updateScatter(bool)));
	connect(mod, SIGNAL(sendFlag(bool)), this, SLOT(setMeshOptionLog(bool)));
	connect(M_computeroptionDialog, SIGNAL(sendFlag(bool)), this, SLOT(setCalOptionLog(bool)));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(openManual()));
}

//打开材质文件
void MainWindow::open_material()
{
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("打开材质文件"), "./", QStringLiteral("材质文件 (*.txt)"));
	load_Material(path.toStdString());

	globalContext *gctx = globalContext::GetInstance();
	if (gctx->modelManager->matManager->materialExisted())
	{
		outputLog(QString(QStringLiteral("Success: 材质信息导入成功!")));
	}
}

//打开场景文件加载对话框
void  MainWindow::openOutdoorFile()
{
	globalContext *gctx = globalContext::GetInstance();
	if (!gctx->modelManager->matManager->materialExisted())
	{
		QMessageBox::warning(this, "Info", QStringLiteral("缺少材质信息，请先加载材质文件！"));
		return;
	}

	if (M_outdoorFileDialog == NULL)
	{
		M_outdoorFileDialog = new outdoorFileDialog(this);
	}
	M_outdoorFileDialog->exec();

}

//根据路径读取场景文件
void MainWindow::loadAllFile(QString _name, QStringList _v, QStringList _h, QString _p)
{
	globalContext *globalCtx = globalContext::GetInstance();
	string name = _name.toStdString();
	string p = _p.toStdString();

	vector<string> v;
	for (int i = 0; i < _v.size(); i++)
	{
		v.push_back(_v[i].toStdString());
	}

	vector<string> h;
	for (int i = 0; i < _h.size(); i++)
	{
		h.push_back(_h[i].toStdString());
	}

	globalCtx->modelManager->loadCityModel(name, v, h, p);
	outputLog(QString(QStringLiteral("Success: 室外场景导入成功!")));
	return;
}

//导入obj文件
void MainWindow::loadObj()
{
	globalContext *gctx = globalContext::GetInstance();
	if (!gctx->modelManager->matManager->materialExisted())
	{
		QMessageBox::warning(this, "Info", QStringLiteral("缺少材质信息，请先加载材质文件！"));
		return;
	}
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("导入Obj模型"), ".", "*.obj");
	if (!path.isEmpty())
	{
		gctx->modelManager->loadGlobalObj(path.toStdString());
		outputLog(QStringLiteral("Success: Obj模型导入成功!"));
	}
}


// 读取json文件(参考 http://bbs.itheima.com/thread-331656-1-1.html )
void MainWindow::quickLoadJson()
{
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("快速导入场景"), "./", QStringLiteral("场景文件 (*.json)"));
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
			outputLog(QStringLiteral("Error: json文件中缺少材质文件信息，请检查!"));
		}
	}
	load_Material(_m.toStdString());
	outputLog(QString(QStringLiteral("Success: 室外场景(json文件)导入成功!")));

	ui.action_showLine->setChecked(true);
	ui.action_showFace->setChecked(true);
	ui.tabWidget_Dispaly->setCurrentIndex(0);
}

//保存局部场景
void MainWindow::saveLocalScene()
{
	globalContext *globalCtx = globalContext::GetInstance();

	int id = globalCtx->modelManager->getLocalShowID();
	if (id < 0)
	{
		QMessageBox::warning(this, "Error", QStringLiteral("未生成局部场景！"));
		return;
	}
	else
	{
		QString path = QFileDialog::getSaveFileName(this, QStringLiteral("保存局部场景"), "./", QStringLiteral("OBJ文件(*.obj)"));
		cityLocalModel *tmp = (cityLocalModel *)globalCtx->modelManager->getLocalModelByID(id);
		tmp->writeToObj(path.toStdString());
		outputLog(QString(QStringLiteral("Success: 局部场景保存成功!")));
	}

}

//保存仿真结果
void MainWindow::saveAllResult()
{
	globalContext *globalCtx = globalContext::GetInstance();
	EFieldContainer* visualDataContainer = globalCtx->visualManager->getContainer();
	if (!visualDataContainer->isDataExist())
	{
		qDebug() << "error: no simulation data" << endl;
		QMessageBox::warning(this, "Error", QStringLiteral("缺少仿真结果，请先进行仿真计算！"));
		return;
	}

	//选择文件夹
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择保存文件夹"),
		".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	vector<int> siteIDs = visualDataContainer->getAllSiteID();

	/*QProgressDialog process(this);
	process.setLabelText(QStringLiteral("保存仿真文件中"));
	process.setRange(0, siteIDs.size());
	process.setModal(true);
	process.setCancelButtonText(tr("cancel"));
	process.setValue(0);*/

	for (int i = 0; i < siteIDs.size(); i++)
	{
		//根据siteID对文件进行命名
		QString filePath = dir.append('/').append("Site").append(QString::number(siteIDs[i])).append(".dat");
		globalCtx->visualManager->saveSiteFile(filePath, siteIDs[i]);
		qDebug() << "save site success ! the siteID is" << i << endl;

		/*process.setValue(i + 1);
		if (process.wasCanceled())
		{
		break;
		}*/
		qDebug() << "------------save Site success!  site id " << siteIDs[i] << "-----------------" << endl;

		QString s = QStringLiteral("Success: id为");
		s.append(QString::number(siteIDs[i])).append(QStringLiteral("的站点仿真结果保存成功!"));
		outputLog(QString(s));
	}
	return;
}

//导入仿真结果
void MainWindow::loadResult()
{
	result_path = QFileDialog::getOpenFileName(this, QStringLiteral("导入仿真结果"), "./", QStringLiteral("仿真结果 (*.dat)"));
	if (!result_path.isEmpty())
		outputLog(QString(QStringLiteral("Success: 仿真结果导入成功!")));
}

//剖分建筑物
void MainWindow::meshAll()
{
	globalContext *gctx = globalContext::GetInstance();
	if (!gctx->modelManager->checkCityExist())
	{
		QMessageBox::warning(this, "Error", QStringLiteral("缺少建筑物信息，请先导入室外场景！"));
		outputLog(QStringLiteral("Error: 缺少建筑物信息，请先导入室外场景!"));
		return;
	}
	else if ((gctx->modelManager->checkLocalExist()))
	{
		QMessageBox::information(this, "Info", QStringLiteral("局部场景已存在，请先删除！"));
		outputLog(QStringLiteral("Info: 局部场景已存在，请先删除!"));
		return;
	}
	setMeshOption();

	setProgress(0);
	vector< Vector3d> center;
	vector<int> siteID;
	double range;

	if (mod->inputFlag)
	{	
		mod->getValue(center, siteID, range);
		if (siteID.empty())
		{
			for (int i = 0; i < center.size(); i++)
			{
				siteID.push_back(i + 1);
			}
		}

		if (gctx->modelManager->getFirstCity()->getType() == ModelType::CITY)
		{
			gctx->modelManager->generateLocalModel(center, siteID, range);
		}
		else if (gctx->modelManager->getFirstCity()->getType() == ModelType::OBJ)
		{
			gctx->modelManager->loadLocalObj(center, range, siteID);
		}

		ui.action_showLine->setChecked(true);
		ui.action_showFace->setChecked(true);

		gctx->modelManager->setModelPara();
		if (gctx->modelManager->getModelPara()->SiteModels.size() != 0)
			outputLog(QStringLiteral("Info: 模型剖分完成!"));
		ui.tabWidget_Dispaly->setCurrentIndex(1);

		mod->inputFlag = false;
	}
}

//打开剖分参数设置对话框
void MainWindow::setMeshOption()
{
	if (mod == NULL)
	{
		mod = new meshOptionDialog(this);
	}	
	mod->exec();
}

//设置材料编号
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

	outputLog(QString(QStringLiteral("Info: 设置材质编号为") + QString::number(i, 10)));
}

//打开计算设置对话框
void MainWindow::computerOption()
{
	if (M_computeroptionDialog == NULL)
	{
		M_computeroptionDialog = new computerOptionDialog(this);
	}
	M_computeroptionDialog->exec();
}

//暂时未用
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

//显示当前模型的编号
void MainWindow::setShowLineEdit(int id)
{
	ui.lineEdit_ModelID->setText(QString::number(id));
	outputLog(QString(QStringLiteral("Info: 当前显示模型的id为") + QString::number(id, 10)));
}

//设置场景透明度
void MainWindow::setModelAlpha(int a)
{
	ui.ModelView->setModelAlpha(a);
	ui.simuArea->setModelAlpha(a);
	ui.simuPlane->setModelAlpha(a);
	//outputLog(QString(QStringLiteral("设置局部模型透明度为：") + QString::number(a, 10)));
}

//设置仿真面透明度
void MainWindow::setPlaneAlpha(int a)
{
	ui.simuPlane->setPlaneAlpha(a);
}

//显示仿真面的站点id和pci
void MainWindow::setSimuPlane(int siteID, int PCI)
{
	ui.lineEdit_SimuPlane_siteID->setText(QString::number(siteID));
	ui.lineEdit_SimuPlane_PCI->setText(QString::number(PCI));
	outputLog(QString(QStringLiteral("Info: 当前仿真面的站点id为") + QString::number(siteID, 10)) + QString(QStringLiteral(",PCI为") + QString::number(PCI, 10)));
	ui.simuPlane->setPCI(siteID, PCI);
}

//设置绘制模式
void MainWindow::setDrawPointMode(bool flag)
{
	//用二进制表达状态
	//ui.simuArea->setPoint(flag);
	ui.ModelView->setPoint(flag);
	ui.simuArea->setPoint(flag);
	if (flag)
		outputLog(QString(QStringLiteral("Info: 设置绘制模式为点模式!")));

}
void MainWindow::setDrawLineMode(bool flag)
{
	//用二进制表达状态
	ui.ModelView->setLine(flag);
	ui.simuArea->setLine(flag);

	if (flag)
		outputLog(QString(QStringLiteral("Info: 设置绘制模式为线框模式!")));
}
void MainWindow::setDrawFaceMode(bool flag)
{
	//用二进制表达状态
	ui.ModelView->setFace(flag);
	ui.simuArea->setFace(flag);
	if (flag)
		outputLog(QString(QStringLiteral("Info: 设置绘制模式为面模式!")));
}


void MainWindow::load_Material(string path)
{

	globalContext *globalCtx = globalContext::GetInstance();
	globalCtx->modelManager->matManager->addMatertial(path);
}

void MainWindow::outputLog(QString source)
{
	ui.textBrowser->append(source);
}

void MainWindow::showScatterWidget()
{
	globalContext *globalCtx = globalContext::GetInstance();
	if ((int)globalCtx->cptManager->getComputationPara()->computeEnum == 1)
	{
		QMessageBox::warning(NULL, "Info", QStringLiteral("基于仿真面的计算不能显示散点折线图！"));
		return;
	}
	scatter->show();	
	if (!globalCtx->visualManager->getContainer()->isDataExist() && result_path.length() == 0)
	{
		QMessageBox::warning(NULL, "Error", QStringLiteral("缺少仿真数据，请先做仿真计算！"));
		return;
	}
	if (result_path.length() == 0)
	{
		scatter->openMeasuredFile();
		scatter->showSimuResult();
	}
	else
	{
		scatter->showLoadResult(result_path);
		result_path.clear();
	}
}


void MainWindow::loadPlugin()
{
	globalContext *gctx = globalContext::GetInstance();
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("加载算法插件"), "./dll_plugins/", "*.dll");
	if (!path.isEmpty())
	{
		gctx->cptManager->setPluginPath(path);
		outputLog(QStringLiteral("Success: 算法插件加载成功!"));
	}
	/*else
	{
	outputLog(QStringLiteral("插件路径设置失败"));
	}*/
}

void MainWindow::deletePlugin()
{
	globalContext *gctx = globalContext::GetInstance();
	gctx->cptManager->deletePluginPath();
	outputLog(QStringLiteral("Success: 算法插件卸载成功!"));
}

void MainWindow::run()
{

	QObject* object;
	globalContext *gctx = globalContext::GetInstance();
	QString  plugin_file_path = gctx->cptManager->getPluginPath();
	QPluginLoader loader(plugin_file_path);

	if ((object = loader.instance()) != NULL)
	{
		ComputeInterface * pluginTemp = qobject_cast<ComputeInterface*>(object);
		if (pluginTemp)
		{
			outputLog(QStringLiteral("Info: 开始运行计算函数!"));
			pluginTemp->runAlgorithm(gctx->modelManager->getModelPara(), gctx->cptManager->getComputationPara(), gctx->visualManager->getVisualPara());
			gctx->visualManager->setContainerData();
			outputLog(QStringLiteral("Info: 仿真计算结束,可查看结果!"));
		}
		ui.tabWidget_Dispaly->setCurrentIndex(2);
	}
}

void MainWindow::updateScatter(bool flag)
{
	if (flag)
	{
		delete scatter;
		scatter = new scatterWidget(this);
		connect(scatter, SIGNAL(sendFlag(bool)), this, SLOT(updateScatter(bool)));
	}
}

void MainWindow::setProgress(int value)
{
	ui.progressBar->setValue(value);
}

void MainWindow::setMeshOptionLog(bool flag)
{
	if (flag)
		outputLog(QStringLiteral("Success: 剖分参数设置成功!"));
}

void MainWindow::setCalOptionLog(bool flag)
{
	if (flag)
		outputLog(QStringLiteral("Success: 计算参数设置成功!"));
}

void MainWindow::openManual()
{
	if (pro != NULL)
	{
		pro->terminate();
		pro->waitForFinished();
		delete pro;
	}
	pro = new QProcess(this);
	QStringList list("UEES_Help.chm");
	pro->start("hh.exe", list);
}
