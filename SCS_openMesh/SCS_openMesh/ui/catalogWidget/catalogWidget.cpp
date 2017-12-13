#include "catalogWidget.h"
#include <QFileDialog>
#include "Context/context.h"
#include <QJsonDocument>
#include <QJsonObject>

catalogWidget::catalogWidget(QWidget *parent)
:ComputePluginObserver("catalogWidgetForPlugin"), modelObserver("catalogWidget"),QTreeWidget(parent)
{

	this->setHeaderLabels(QStringList() << QStringLiteral("项目"));
	
	mItem = new QTreeWidgetItem();
	mItem->setText(0, "Model");
	this->addTopLevelItem(mItem);

	cItem = new QTreeWidgetItem();
	cItem->setText(0, "Computation");
	this->addTopLevelItem(cItem);

	vItem = new QTreeWidgetItem();
	vItem->setText(0, "Visualization");
	this->addTopLevelItem(vItem);
	
}

catalogWidget::~catalogWidget(){}

void catalogWidget::addParentMenu()
{
	QMenu * menu = new QMenu(this);

	QTreeWidgetItem *item = this->currentItem();
	if (item == NULL) return;

	if (item->text(0) == "Model")
	{
		QAction * addModelAction = new QAction(QStringLiteral("导入城市场景"),this);
		menu->addAction(addModelAction);
		connect(addModelAction, SIGNAL(triggered()), this, SLOT(addModel()));
	}
	if (item->text(0) == "Computation")
	{
		QAction * addPluginAction= new QAction(QStringLiteral("导入插件"), this);
		menu->addAction(addPluginAction);
		connect(addPluginAction, SIGNAL(triggered()), this, SLOT(addCptPlugin()));
	}
	if (item->text(0) == "Visualization")
	{
		//QAction * addModelAction2 = new QAction(tr("add2"), this);
		//menu->addAction(addModelAction2);
	}
	menu->exec(QCursor::pos());
}

void catalogWidget::addChildMenu()
{
	QMenu * menu = new QMenu(this);
	QTreeWidgetItem *item = this->currentItem();

	if (item->parent() == NULL)
		return;

	//暂时只写了模型板块
	QAction * deleteModelAction = new QAction(tr("delete"), this);
	menu->addAction(deleteModelAction);
	connect(deleteModelAction, SIGNAL(triggered()), this, SLOT(deleteModel()));

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

	//在treewidget中显示模型名
	/*QTreeWidgetItem *modelChildItem = new QTreeWidgetItem();
	modelChildItem->setText(0, _name);
	mItem->addChild(modelChildItem);*/
}

void catalogWidget::deleteModel()
{
	QTreeWidgetItem *item = this->currentItem();
	if (item->parent() != NULL)
	{
		string name = (item->text(0)).toStdString();
		delete item;
		globalContext *gctx = globalContext::GetInstance();
		gctx->modelManager->deleteModel(name);
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
	if (a->needUpdate())
	{
		deleleItemsUnderItem(mItem);
		for (int i = 0; i < a->getName().size(); i++)
		{
			QTreeWidgetItem *modelChildItem = new QTreeWidgetItem();
			modelChildItem->setText(0, QString::fromStdString(a->getName().at(i)));
			mItem->addChild(modelChildItem);
		}
	}
	else
	{
		deleleItemsUnderItem(mItem);
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
