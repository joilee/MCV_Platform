#include "catalogWidget.h"
#include <QFileDialog>
#include "Context/context.h"
#include <QJsonDocument>
#include <QJsonObject>

catalogWidget::catalogWidget(QWidget *parent)
:ComputePluginObserver("catalogWidgetForPlugin"), modelObserver("catalogWidget"),QTreeWidget(parent)
{

	this->setHeaderLabels(QStringList() << QStringLiteral("��Ŀ"));
	
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
		QAction * addModelAction = new QAction(QStringLiteral("������г���"),this);
		menu->addAction(addModelAction);
		connect(addModelAction, SIGNAL(triggered()), this, SLOT(addModel()));
	}
	if (item->text(0) == "Computation")
	{
		QAction * addPluginAction= new QAction(QStringLiteral("������"), this);
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

	//��ʱֻд��ģ�Ͱ��
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
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("���ٵ��볡��"), "./", QStringLiteral("�����ļ� (*.json)"));
	if (path.isNull())
	{
		return;
	}

	//��ȡjson�ļ������ļ��е�Ŀ¼,Ϊ�������ļ�׼����·��
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
		//���ģ��
		if (obj.contains("Name"))
		{
			_name = obj.value("Name").toString();
		}
		else
		{
			cout << "error: json�ļ���û��ģ�ͣ����飡" << endl;
			return;
		}
		//������
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
			cout << "error: json��û�в����ļ������飡" << endl;
		}
	}

	globalContext *gctx = globalContext::GetInstance();
	gctx->modelManager->loadCityModel(path.toStdString());
	gctx->modelManager->matManager->addMatertial(_m.toStdString());

	//��treewidget����ʾģ����
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
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("���������"), "./", QStringLiteral("��̬���ӿ� (*.dll)"));
	globalContext *gctx = globalContext::GetInstance();
	gctx->cptManager->setPluginPath(path);
}

void catalogWidget::update(visualModelItem * a)
{
	cout << "Catalog ���յ��ֲ����µ��ź�" << endl;
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
	cout << "Catalog ���յ�plugin���µ��ź�" << endl;
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
