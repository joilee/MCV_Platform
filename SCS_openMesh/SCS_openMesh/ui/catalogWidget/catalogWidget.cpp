#include "catalogWidget.h"
#include <QFileDialog>
#include "Context/context.h"
#include <QJsonDocument>
#include <QJsonObject>

catalogWidget::catalogWidget(QWidget *parent)
:QTreeWidget()
{

	this->setHeaderLabels(QStringList() << QStringLiteral("��Ŀ") << QStringLiteral("����"));
	
	mItem = new QTreeWidgetItem();
	mItem->setText(0, "M");
	this->addTopLevelItem(mItem);

	cItem = new QTreeWidgetItem();
	cItem->setText(0, "C");
	this->addTopLevelItem(cItem);

	vItem = new QTreeWidgetItem();
	vItem->setText(0, "V");
	this->addTopLevelItem(vItem);
	
}

catalogWidget::~catalogWidget(){}

void catalogWidget::addParentMenu()
{
	QMenu * menu = new QMenu(this);

	QTreeWidgetItem *item = this->currentItem();
	if (item == NULL) return;

	if (item->text(0) == "M")
	{
		QAction * addModelAction = new QAction(tr("add"),this);
		menu->addAction(addModelAction);
		connect(addModelAction, SIGNAL(triggered()), this, SLOT(addModel()));
	}
	if (item->text(0) == "C")
	{
		QAction * addModelAction1 = new QAction(tr("add1"), this);
		menu->addAction(addModelAction1);
	}
	if (item->text(0) == "V")
	{
		QAction * addModelAction2 = new QAction(tr("add2"), this);
		menu->addAction(addModelAction2);
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

	//��treewidget����ʾģ����

	for (int i = 0; i < modelName.size(); i++)
	{
		if (modelName.at(i) == _name)
		{
			cout << "Info: ��ģ���Ѿ����ڣ�" << endl;
			return;
		}
	}
	modelName.append(_name);
	QTreeWidgetItem *modelChildItem = new QTreeWidgetItem();
	modelChildItem->setText(0, _name);
	mItem->addChild(modelChildItem);

	globalContext *gctx = globalContext::GetInstance();
	gctx->modelManager->loadCityModel(path.toStdString());
	gctx->modelManager->matManager->addMatertial(_m.toStdString());
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

		for (int i = 0; i < modelName.size(); i++)
		{
			if (modelName[i].toStdString() == name)
			{
				modelName.removeAt(i);
				break;
			}
		}
	}
}

