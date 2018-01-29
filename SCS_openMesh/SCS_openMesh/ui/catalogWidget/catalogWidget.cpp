#include "catalogWidget.h"
#include <QFileDialog>
#include "Context/context.h"
#include <QJsonDocument>
#include <QJsonObject>

catalogWidget::catalogWidget(QWidget *parent)
:SiteCellObserver("siteCell"),ComputePluginObserver("catalogWidgetForPlugin"), modelObserver("catalogWidget"),QTreeWidget(parent)
{
	this->setHeaderLabels(QStringList() << QStringLiteral("项目")<< QStringLiteral("属性"));
	
	mGlobalItem = new QTreeWidgetItem(this, QStringList(QStringLiteral(CITY_VIEW)));
	this->addTopLevelItem(mGlobalItem);

	mLocalItem = new QTreeWidgetItem(this, QStringList(QStringLiteral(MODEL_ITEM)));
	this->addTopLevelItem(mLocalItem);

	cItem = new QTreeWidgetItem(this, QStringList(QStringLiteral(CPT_ITEM)));
	this->addTopLevelItem(cItem);

	vItem = new QTreeWidgetItem(this, QStringList(QStringLiteral(VIS_ITEM)));
	this->addTopLevelItem(vItem);
	
}

catalogWidget::~catalogWidget(){}

void catalogWidget::addParentMenu()
{
	QMenu * menu = new QMenu(this);

	QTreeWidgetItem *item = this->currentItem();
	if (item == NULL) return;

	if (item->text(0) == QStringLiteral(CITY_VIEW))
	{
		QAction * addModelAction = new QAction(QStringLiteral("导入城市场景"),this);
		menu->addAction(addModelAction);
		connect(addModelAction, SIGNAL(triggered()), this, SLOT(addModel()));
	}
	if (item->text(0) == QStringLiteral(CPT_ITEM))
	{
		QAction * addPluginAction= new QAction(QStringLiteral("导入插件"), this);
		menu->addAction(addPluginAction);
		connect(addPluginAction, SIGNAL(triggered()), this, SLOT(addCptPlugin()));
	}
	if (item->text(0) == QStringLiteral(VIS_ITEM))
	{

	}
	menu->exec(QCursor::pos());
}

void catalogWidget::addChildMenu()
{
	QMenu * menu = new QMenu(this);
	QTreeWidgetItem *item = this->currentItem();

	if (item->parent() == NULL)
	{
		cout << "error:no parent in treeWidget" << endl;
		return;
	}


	//暂时只写了模型板块
	if (item->parent()->text(0) == QStringLiteral(MODEL_ITEM))
	{
		QAction * deleteModelAction = new QAction(tr("delete"), this);
		menu->addAction(deleteModelAction);
		connect(deleteModelAction, SIGNAL(triggered()), this, SLOT(deleteModel()));

		QAction *showModelAction = new QAction(tr("show"), this);
		menu->addAction(showModelAction);
		connect(showModelAction, SIGNAL(triggered()), this, SLOT(showModel()));
	}


	menu->exec(QCursor::pos());
}

void catalogWidget::contextMenuEvent(QContextMenuEvent *event)
{
	this->addParentMenu();
	this->addChildMenu();
	event->accept();
}

void catalogWidget::deleleItemsUnderItem(QTreeWidgetItem * a)
{
	while (a->childCount() != 0)
	{
		QTreeWidgetItem * item = a->child(0);
		a->removeChild(item);
	}
	return;
}

void catalogWidget::addModel()
{
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("快速导入场景"), "./", QStringLiteral("场景文件 (*.json)"));
	if (path.isNull())
	{
		return;
	}

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
	QString _name;

	if (jsDoc.isObject())
	{
		QJsonObject obj = jsDoc.object();
		//检测模型
		if (obj.contains("Name"))
		{
			_name = obj.value("Name").toString();
		}
		else
		{
			cout << "error: json文件中没有模型，请检查！" << endl;
			return;
		}
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

	globalContext *gctx = globalContext::GetInstance();
	gctx->modelManager->loadCityModel(path.toStdString());
	gctx->modelManager->matManager->addMatertial(_m.toStdString());

}

void catalogWidget::deleteModel()
{
	QTreeWidgetItem *item = this->currentItem();
	if (item->parent() != NULL&&item->parent()->text(0) == QStringLiteral(MODEL_ITEM))
	{
		string name = (item->text(0)).toStdString();
		globalContext *gctx = globalContext::GetInstance();
		gctx->modelManager->deleteLocalModel(name);
	}
}

void catalogWidget::showModel()
{
	QTreeWidgetItem *item = this->currentItem();
	if (item->parent() != NULL&&item->parent()->text(0) == QStringLiteral(MODEL_ITEM))
	{
		string name = (item->text(0)).toStdString();
		int id = (item->text(1)).toInt();
		globalContext *gctx = globalContext::GetInstance();
		gctx->modelManager->setLocalShowID(id);
		emit(modelID_ShowChanged(id));
	}
}

void catalogWidget::addCptPlugin()
{
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("导入计算插件"), "./", QStringLiteral("动态链接库 (*.dll)"));
	globalContext *gctx = globalContext::GetInstance();
	gctx->cptManager->setPluginPath(path);
}

void catalogWidget::update(visualModelItem * a)
{
	cout << "Catalog 接收到局部更新的信号" << endl;
	if (a->cityNeedUpdate())
	{
		deleleItemsUnderItem(mGlobalItem);
		for (int i = 0; i < a->getGlobalName().size(); i++)
		{
			QTreeWidgetItem *modelChildItem = new QTreeWidgetItem();
			modelChildItem->setText(0, QString::fromStdString(a->getGlobalName().at(i)));
			modelChildItem->setText(1, QString::number(a->getGlobalID().at(i)));
			mGlobalItem->addChild(modelChildItem);
		}
	}
	else
	{
		deleleItemsUnderItem(mGlobalItem);
	}

	if (a->localNeedUpdate())
	{
		deleleItemsUnderItem(mLocalItem);
		for (int i =0 ; i < a->getLocalName().size();i++)
		{
			QTreeWidgetItem *modelChildItem = new QTreeWidgetItem();
			modelChildItem->setText(0, QString::fromStdString(a->getLocalName().at(i)));
			modelChildItem->setText(1, QString::number(a->getLocalModelID().at(i)));
			mLocalItem->addChild(modelChildItem);
		}
	}
	else
	{
		deleleItemsUnderItem(mLocalItem);
	}
}

void catalogWidget::updatePluginInfo(VisualPluginItem* a)
{
	cout << "Catalog 接收到plugin更新的信号" << endl;
	if (a->itemExist())
	{
		deleleItemsUnderItem(cItem);
		QTreeWidgetItem *pluginChildItem = new QTreeWidgetItem();
		pluginChildItem->setText(0, a->getPluginName());
		cItem->addChild(pluginChildItem);
	}
	else
	{
		deleleItemsUnderItem(cItem);
	}
}

void catalogWidget::updateResult(Site_Item*a)
{
	cout << "Catalog接收到result更新的信号" << endl;
	if (a->needUpdate())
	{
		map<int, Cell_Item_Vector*>::iterator iter;
		for (iter = a->getData().begin(); iter != a->getData().end(); iter++)
		{
			QTreeWidgetItem *visChildItem = new QTreeWidgetItem();
			visChildItem->setText(0, QString::number(iter->first));
			vItem->addChild(visChildItem);
		}
	}
	else
	{
		deleleItemsUnderItem(cItem);
	}

}

#include "GeneratedFiles/Release/moc_catalogWidget.cpp"
