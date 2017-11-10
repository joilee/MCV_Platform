#include "scsTreeItem.h"
#include "scsTree.h"

#include <stack>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

namespace scs_GUI{
	scsTreeItem::scsTreeItem()
		:QTreeWidgetItem()
	{
		m_iID = -1;
		m_pParentTree = NULL;
		setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	}

	scsTreeItem::scsTreeItem(scsTree *parent)
		:QTreeWidgetItem((QTreeWidget*)parent)
	{
		m_pParentTree = parent;
		if (parent != NULL)
			m_iID = parent->addTopItem(this);
		else
			m_iID = -1;
		//initProperty();
		setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	}

	scsTreeItem::scsTreeItem(scsTreeItem *parent)
		:QTreeWidgetItem(parent)
	{
		if (parent != NULL) m_pParentTree = parent->getParentTree();
		if (m_pParentTree != NULL)
			m_iID = m_pParentTree->addItem(this, parent->getID());
		else
			m_iID = -1;
		//initProperty();
		setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	}

	scsTreeItem::~scsTreeItem()
	{}

	scsTree* scsTreeItem::getParentTree()
	{
		return m_pParentTree;
	}

	scsTreeItem* scsTreeItem::getParentItem()
	{
		QTreeWidgetItem* pItem = dynamic_cast<QTreeWidgetItem*>(this)->parent();
		if (pItem == NULL) return NULL;
		return dynamic_cast<scsTreeItem*>(pItem);
	}

	scsTreeItem* scsTreeItem::getChildItem(int idx)
	{
		return dynamic_cast<scsTreeItem*>(child(idx));
	}

	scsTreeItem* scsTreeItem::getChildItem(const QString& name)
	{
		scsTreeItem *pItem;
		for (int i = this->childCount() - 1; i >= 0; i--)
		{
			pItem = getChildItem(i);
			if (name == pItem->text(0)) return pItem;
		}
		return NULL;
	}

	void scsTreeItem::setParentTree(scsTree *parent)
	{
		this->m_pParentTree = parent;
	}

	int scsTreeItem::addSubItem(scsTreeItem *pItem)
	{
		return dynamic_cast<scsTree *>(this->treeWidget())->addItem(pItem, m_iID);
	}

	int scsTreeItem::addSubItem(scsTreeItem *pItem, scsTreeItem *pParentItem)
	{
		return dynamic_cast<scsTree *>(this->treeWidget())->addItem(pItem, pParentItem->getID());
	}

	int scsTreeItem::addSubItem(scsTreeItem *pItem, scsTreeItem *pParentItem, scsTreeItem *pPrecedingItem)
	{
		return dynamic_cast<scsTree *>(this->treeWidget())->addItem(pItem, pParentItem->getID(), pPrecedingItem->getID());
	}

	scsTreeItem * scsTreeItem::getSelectedItem() const
	{
		//this->treeWidget() Returns the tree widget that contains the item.
		QList<scsTreeItem *> selectedItemList = dynamic_cast<scsTree *>(this->treeWidget())->getSelectedItems();
		scsTreeItem* pSelected = NULL;
		scsTreeItem* pItem = NULL;
		QListIterator<scsTreeItem*> iter(selectedItemList);
		scsTreeItem* parent_tmp = NULL;
		while (iter.hasNext())
		{
			pItem = iter.next();
			parent_tmp = pItem->getParentItem();
			while (parent_tmp != NULL)
			{
				if (parent_tmp == this)
				{
					pSelected = pItem;
					return pSelected;
				}
				parent_tmp = parent_tmp->getParentItem();
			}
		}
		return pSelected;
	}

	QList<scsTreeItem*> scsTreeItem::getSelectedItems() const
	{
		return m_pParentTree->getSelectedItems();
	}
	void scsTreeItem::removeItem(scsTreeItem *pItem)
	{
		dynamic_cast<scsTree *>(this->treeWidget())->removeItem(pItem->getID());
	}

	int scsTreeItem::getID()
	{
		return m_iID;
	}

	void scsTreeItem::setID(int ID)
	{
		m_iID = ID;
	}

	scsTreeItem* scsTreeItem::getItemFromPath(const vector<string> &componentPath)
	{
		//如果当前子树没有子节点，则返回NULL
		if (this->childCount() < 1)
		{
			return NULL;
		}

		vector<string>::const_iterator pos = componentPath.begin();
		//assert(!pos->compare(item->text(0)));
		QTreeWidgetItem *item = NULL;
		for (int i = this->childCount() - 1; i >= 0; i--)
		{
			item = this->child(i);
			if ((*pos) == item->text(0).toStdString()) break;
		}

		if (componentPath.size() == 1) {
			return static_cast<scsTreeItem*>(item);
		}

		for (++pos; pos < componentPath.end() && item != NULL; ++pos) {
			QTreeWidgetItem * childItem = NULL;
			for (int j = item->childCount() - 1; j >= 0; j--)
			{
				childItem = item->child(j);
				if (!pos->compare(childItem->text(0).toStdString()))
					break;
			}
			item = childItem;
		}
		return static_cast<scsTreeItem*>(item);
	}

	vector<string> scsTreeItem::getPathToRoot(scsTreeItem* pItem)
	{
		std::stack<string> strStack;
		string path(pItem->text(0).toStdString());
		strStack.push(path);

		if (pItem == NULL)
			pItem = this;

		QTreeWidgetItem* pParentItem = dynamic_cast<QTreeWidgetItem*>(pItem)->parent();

		//从pItem向上一直到根，把路径上每个节点的名称压入strStack
		while (pParentItem != NULL && static_cast<scsTreeItem*>(pParentItem)->getID() != m_iID) {
			path = pParentItem->text(0).toStdString();
			strStack.push(path);
			pParentItem = pParentItem->parent();
		}

		vector<string> result;
		while (!strStack.empty()) {//反转strStack中的路径，变为从根节点到pItem
			result.push_back(strStack.top());
			strStack.pop();
		}
		return result;
	}

	bool scsTreeItem::hasChildrenChecked()
	{
		for (int i = childCount() - 1; i >= 0; i--)
		{
			if (child(i)->checkState(0) != Qt::Unchecked)//有child处于check状态
			{
				return true;
			}
		}
		return false;
	}

	void scsTreeItem::setCheckStateRecursive(Qt::CheckState state)
	{
		//屏蔽所在Tree发送signal
		//因为此函数会改变其他节点的check state，不屏蔽会不断发送itemChanged信号
		m_pParentTree->blockSignals(true);
		if (flags() & Qt::ItemIsUserCheckable)
		{
			this->setCheckState(0, state);
			//设置子节点的状态为state
			this->setChildCheckStateRecursive(state);
			//更改父节点的状态
			this->updateParentCheckState();
		}
		m_pParentTree->blockSignals(false);
	}

	void scsTreeItem::expandToItem(scsTreeItem* pItem)
	{
		m_pParentTree->scrollToItem(dynamic_cast<QTreeWidgetItem*>(pItem));
		m_pParentTree->setSelectedItem(pItem);
	}
	
	void scsTreeItem::removeAllChildren()
	{
		for (int i = childCount() - 1; i >= 0; i--)
		{
			removeItem(getChildItem(i));
		}
	}

	bool scsTreeItem::contains(QString itemName)
	{
		scsTreeItem *pItem;
		for (int i = this->childCount() - 1; i >= 0; i--)
		{
			pItem = getChildItem(i);
			if (itemName == pItem->text(0)) return true;
		}
		return false;
	}

	void scsTreeItem::setCheckable(bool able)
	{
		if (able){
			setFlags(flags() | Qt::ItemIsUserCheckable);
		}
		else{
			setFlags(flags() & ~Qt::ItemIsUserCheckable);
		}
	}

	void scsTreeItem::setChildCheckStateRecursive(Qt::CheckState state)
	{
		scsTreeItem* tmp;
		for (int i = childCount() - 1; i >= 0; i--)
		{
			tmp = getChildItem(i);
			if (tmp->flags() & Qt::ItemIsUserCheckable)
			{
				tmp->setCheckState(0, state);
				tmp->setChildCheckStateRecursive(state);
			}
		}
	}

	void scsTreeItem::updateParentCheckState()
	{
		scsTreeItem* pParentItem = getParentItem();
		if (pParentItem != NULL)
		{
			pParentItem->updateCheckState();
			pParentItem->updateParentCheckState();
		}
	}

	void scsTreeItem::updateCheckState()
	{
		if (flags() & Qt::ItemIsUserCheckable)
		{
			//检查所有子节点的状态，根据检查结果设置当前节点的状态
			int checked = 0;
			bool partiallyChecked = false;
			for (int i = childCount() - 1; i >= 0; i--)
			{
				if (getChildItem(i)->checkState(0) == Qt::Checked)
				{
					checked++;
				}
				else if (getChildItem(i)->checkState(0) == Qt::PartiallyChecked)
				{
					partiallyChecked = true;
					break;
				}
			}
			if (partiallyChecked) {
				setCheckState(Qt::PartiallyChecked);
			}
			else if (checked == 0) {
				setCheckState(Qt::Unchecked);
			}
			else if (checked == childCount()) {
				setCheckState(Qt::Checked);
			}
			else {
				setCheckState(Qt::PartiallyChecked);
			}
		}
	}

	void scsTreeItem::setCheckState(Qt::CheckState state)
	{
		QTreeWidgetItem::setCheckState(0, state);
	}

	void scsTreeItem::setCheckState(int col, Qt::CheckState state)
	{
		QTreeWidgetItem::setCheckState(col, state);
	}

	void scsTreeItem::outputErrorMessage(QString text)
	{
		QMessageBox box;
		box.setText(text);
		box.exec();
	}



	void scsTreeItem::retranslate()
	{

	}
}