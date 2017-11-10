#include "scsModelTree.h"
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QApplication>
#include <QFileDialog>

#include "../../Model/abstractModelFactory.h"

namespace scs_GUI
{
	scsModelTree::scsModelTree()
		:scsTreeItem()
	{
		init();
	}
	scsModelTree::scsModelTree(scsTree *parent)
		: scsTreeItem(parent)
	{
		init();
	}

	scsModelTree::scsModelTree(scsTreeItem *parent)
		: scsTreeItem(parent)
	{
		init();
	}

	scsModelTree::~scsModelTree()
	{}

	void scsModelTree::init()
	{
		this->setCheckable(false);
		retranslate();
	}

	//////////////////////////////////////////////////////////////////////////
	// scsTreeItem Interface
	void scsModelTree::changeSelectionTo(scsTreeItem *pItem)
	{
		cout << "	void scsModelTree::changeSelectionTo(scsTreeItem *pItem)" << endl;
	}

	void scsModelTree::itemSelectionChanged(QList<scsTreeItem *> pItems)
	{
		cout << "void scsModelTree::itemSelectionChanged(QList<scsTreeItem *> pItems)" << endl;
		//选出pItems中属于本棵树的Item
		QList<scsTreeItem*> pItemFiltered;
		QListIterator<scsTreeItem*> it(pItems);
		scsTreeItem* tmp;
		while (it.hasNext())
		{
			tmp = it.next();
			if (tmp->getParentItem()->getID() == this->m_iID)
			{
				pItemFiltered.push_back(tmp);
			}
		}

		//做其他处理
	}

	void scsModelTree::itemChanged(scsTreeItem * item, int column)
	{
		cout << "scsModelTree::itemChanged(scsTreeItem * item, int column)" << endl;
	}

	void scsModelTree::showContextMenu(const QPoint & position, scsTreeItem *pItem)
	{

		cout << "	void scsModelTree::showContextMenu(const QPoint & position, scsTreeItem *pItem)" << endl;
		//if (pItem->getID() == this->getID()) return;

		//set current component


		QMenu menu;
		QAction *action_create = new QAction(QApplication::translate("scsModelTree", "Create Group", 0), this);
		menu.addAction(action_create);
		QObject::connect(action_create, SIGNAL(triggered()), this, SLOT(slotActionCreate()));

		if (pItem->getParentItem() != this) // The model root item cannot be deleted
		{
			QAction *action_delete = new QAction(QApplication::translate("scsModelTree", "Delete Group", 0), this);
			menu.addAction(action_delete);
			QObject::connect(action_delete, SIGNAL(triggered()), this, SLOT(slotActionDelete()));
		}
		else
		{
			QAction *action_clear = new QAction(QApplication::translate("scsModelTree", "Clear Model", 0), this);
			menu.addAction(action_clear);
			QObject::connect(action_clear, SIGNAL(triggered()), this, SLOT(slotActionDelete()));
		}

		QAction *action_export = new QAction(QApplication::translate("scsModelTree", "Export Model", 0), this);
		menu.addAction(action_export);
		QObject::connect(action_export, SIGNAL(triggered()), this, SLOT(slotActionExport()));

		//根据选中的节点类型设置右键菜单项是否可用


		menu.exec(QCursor::pos());
	}
	void scsModelTree::slotActionDelete()
	{
	
	}

	void scsModelTree::slotActionExport()
	{
	
	}
	void scsModelTree::slotActionCreate()
	{

	}
	QString scsModelTree::getFilterString(const vector<pair<string, string>>& formatInfo)
	{
		vector<pair<string, string>>::const_iterator it = formatInfo.begin();

		QString exts = QString::fromLocal8Bit(it->first.c_str());
		exts.replace(';', " *.");
		exts.prepend("*.");
		QString filter = QString::fromLocal8Bit(it->second.c_str()) + " (" + exts + ")";

		for (++it; it != formatInfo.end(); ++it)
		{
			filter.append(";;");
			exts = QString::fromLocal8Bit(it->first.c_str());
			exts.replace(';', " *.");
			exts.prepend("*.");
			filter.append(QString::fromLocal8Bit(it->second.c_str()) + " (" + exts + ")");
		}
		return filter;
	}
	void scsModelTree::itemToMove(scsTreeItem* from, scsTreeItem* to)
	{
	
	}
	void scsModelTree::retranslate()
	{
		this->setText(0, QApplication::translate("scsModelTree", "Model Tree", 0));
	}
}