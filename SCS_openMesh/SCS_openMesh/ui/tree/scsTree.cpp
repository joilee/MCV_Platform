#include "scsTree.h"
#include  "scsTreeItem.h"
#include <QEvent>
#include <QMimeData>
#include <QDragMoveEvent>
#include <QDropEvent>

namespace scs_GUI{
	scsTree::scsTree(QWidget *parent) : QTreeWidget(parent)
	{
		// 树中没有节点时，当前ID最大值为-1
		m_iIDMax = -1;

		m_SizeHint = QSize(-1, -1);

		// Drag and Drop
		setDragEnabled(true);
		setDragDropMode(QAbstractItemView::InternalMove);
		invisibleRootItem()->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		
		setHeaderHidden(true);
		setContextMenuPolicy(Qt::CustomContextMenu);

		//右键菜单
		QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotContextMenuRequested(const QPoint &)));
		QObject::connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotItemChanged(QTreeWidgetItem *, int)));
		QObject::connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotItemDoubleClicked(QTreeWidgetItem *, int)));
	}
	scsTree::~scsTree()
	{
		scsTreeItem *pItem;
		//删除所有的节点
		for (int i = this->topLevelItemCount() - 1; i >= 0; i--)
		{
			pItem = static_cast<scsTreeItem *>(this->topLevelItem(i));
			removeTopItem(pItem->getID());
		}
		this->clear();
	}

	int scsTree::getID(scsTreeItem *pItem)
	{
		m_iIDMax++;
		m_ItemMap[m_iIDMax] = pItem;
		pItem->setID(m_iIDMax);
		return m_iIDMax;
	}

	scsTreeItem * scsTree::getTopItem(scsTreeItem *pItem)
	{
		scsTreeItem* pRoot = pItem;
		if (pRoot != NULL) {
			while (dynamic_cast<QTreeWidgetItem *>(pRoot)->parent() != NULL)
			{
				pRoot = static_cast<scsTreeItem *>(dynamic_cast<QTreeWidgetItem *>(pRoot)->parent());
			}
			return pRoot;
		}
		else {
			return NULL;
		}
	}

	int scsTree::addTopItem(scsTreeItem *pItem)
	{
		this->addTopLevelItem(pItem);
		m_iIDMax++;
		m_ItemMap[m_iIDMax] = pItem;
		pItem->setID(m_iIDMax);
		pItem->setParentTree(this);

		QObject::connect(pItem, SIGNAL(updateDescription()), this, SLOT(slotUpdateDescription()));
		

		return m_iIDMax;
	}

	int scsTree::addItem(scsTreeItem *pItem, int iParentID)
	{
		fflush(stdout);
		scsTreeItem* pParentItem = m_ItemMap[iParentID];
		//		printf("[[ %s ]]\n",pParentItem->text(0).toLatin1().data());
		pParentItem->addChild(pItem);
		m_iIDMax++;
		m_ItemMap[m_iIDMax] = pItem;
		pItem->setID(m_iIDMax);
		pItem->setParentTree(this);
		// 链接信号和槽
		QObject::connect(pItem, SIGNAL(updateDescription()), this, SLOT(slotUpdateDescription()));
		return m_iIDMax;
	}

	int scsTree::addItem(scsTreeItem *pItem, int iParentID, int iPrecedingID)
	{
		scsTreeItem* pParentItem = m_ItemMap[iParentID];
		scsTreeItem* pPrecedingItem = m_ItemMap[iPrecedingID];

		pParentItem->insertChild(pParentItem->indexOfChild(pPrecedingItem) + 1, pItem);
		m_iIDMax++;
		m_ItemMap[m_iIDMax] = pItem;
		pItem->setID(m_iIDMax);
		pItem->setParentTree(this);
		return m_iIDMax;
	}

	int scsTree::addSubItemToSelectedItem(scsTreeItem *pItem)
	{
		//	QList<QTreeWidgetItem *> selectedItemList = m_pTree->selectedItems();
		QList<QTreeWidgetItem *> selectedItemList = this->selectedItems();
		if (selectedItemList.size() > 0)
		{
			selectedItemList[0]->addChild((QTreeWidgetItem *)pItem);
			m_iIDMax++;
			m_ItemMap[m_iIDMax] = pItem;
			pItem->setID(m_iIDMax);
			pItem->setParentTree(this);
			return m_iIDMax;
		}
		return -1;
	}

	const scsTreeItem* scsTree::getItemByID(int ID)
	{
		// 判断给定的ID是否存在与其对应的树节点
		std::map<int, scsTreeItem *>::const_iterator it = m_ItemMap.find(ID);
		//不存在，返回NULL
		if (it == m_ItemMap.end()) {
			return NULL;
		}
		else {
			//存在，返回树节点的指针
			return it->second;
		}
	}

	scsTreeItem * scsTree::getSelectedItem() const
	{
		QList<QTreeWidgetItem *> selectedItemList = this->selectedItems();
		scsTreeItem *pCurrentItem;
		if (selectedItemList.size() > 0) {
			pCurrentItem = (scsTreeItem *)selectedItemList[0];
		}
		else {
			pCurrentItem = NULL;
		}
		return pCurrentItem;
	}

	QList<scsTreeItem *> scsTree::getSelectedItems() const
	{
		QList<QTreeWidgetItem *> selectedItemList = this->selectedItems();
		QList<scsTreeItem *> result;
		while (!selectedItemList.isEmpty())
		{
			result.push_back((scsTreeItem *)(selectedItemList.takeFirst()));
		}
		return result;
	}

	void scsTree::removeTopItem(int iItemID)
	{
		// 判断给定的ID是否存在与其对应的树节点
		std::map<int, scsTreeItem *>::const_iterator it = m_ItemMap.find(iItemID);
		//存在，删除相应节点及map中的信息
		if (it != m_ItemMap.end()) {
			scsTreeItem* pItem = it->second;
			// 删除子节点
			for (int i = pItem->childCount() - 1; i >= 0; i--)
			{
				for (std::map<int, scsTreeItem *>::iterator it_tmp = m_ItemMap.begin(); it_tmp != m_ItemMap.end(); it_tmp++)
				{
					if (it_tmp->second == static_cast<scsTreeItem *>(pItem->child(i)))
					{
						delete it_tmp->second;
						m_ItemMap.erase(it_tmp);
						break;
					}
				}
			}
			deleteItem(pItem);
			m_ItemMap.erase(m_ItemMap.find(iItemID));
		}
	}

	/**
	* 貌似跟removeTopItem的实现没怎么有区别
	*/
	void scsTree::removeItem(int iItemID)
	{
		// 判断给定的ID是否存在与其对应的树节点
		std::map<int, scsTreeItem *>::const_iterator it = m_ItemMap.find(iItemID);
		//存在，删除相应节点及map中的信息
		if (it != m_ItemMap.end()) {
			scsTreeItem* pItem = it->second;
			deleteItem(pItem);
			m_ItemMap.erase(it);
		}
	}

	/**
	* 未测试
	*/
	void scsTree::deleteItem(scsTreeItem* pItem)
	{
		if (!pItem) return;
		for (int i = pItem->childCount() - 1; i >= 0; i--) {
			deleteItem((scsTreeItem *)pItem->child(i));
		}
		delete pItem;
	}

	void scsTree::setSelectedItem(scsTreeItem *pItem)
	{
		setCurrentItem(pItem);
	}

	void scsTree::slotContextMenuRequested(const QPoint & position)
	{
		scsTreeItem * pItem = (scsTreeItem *)this->itemAt(position);
		if (pItem == NULL) return;

		//Change current selection
		setSelectedItem(pItem);
		getTopItem(pItem)->changeSelectionTo(pItem);

		// 找到点选的节点的顶层节点，调用顶层节点中的showContextMenu函数
		scsTreeItem *pParent = pItem;
		while (dynamic_cast<QTreeWidgetItem*>(pParent)->parent() != NULL)
		{
			pParent = static_cast<scs_GUI::scsTreeItem *>(dynamic_cast<QTreeWidgetItem*>(pParent)->parent());
		}
		if (pParent != NULL) {
			pParent->showContextMenu(position, pItem);
		}
	}


	void scsTree::slotSelectionChanged()
	{
		scsTreeItem *pItem = getSelectedItem();
		if (pItem != NULL)
		{
			printf("scsTree::slotSelectionChanged! >> %d | name[%s]\n", pItem->getID(), pItem->text(0).toLatin1().data());
			// 取pItem的根节点pRoot(其所在子树的TopLevelItem),调用其changeSelectionTo(scsTreeItem *)函数
			scsTreeItem * pRoot = getTopItem(pItem);
			pRoot->changeSelectionTo(pItem);
		}
	}

	void scsTree::slotItemChanged(QTreeWidgetItem *pItem, int colume)
	{
		if (pItem != NULL)
		{
			scsTreeItem *pRoot = getTopItem(static_cast<scsTreeItem *>(pItem));
			pRoot->itemChanged(static_cast<scsTreeItem *>(pItem), colume);
		}
	}

	void scsTree::slotUpdateDescription()
	{
		emit updateDescription();
	}

	void scsTree::slotItemDoubleClicked(QTreeWidgetItem *pItem, int colume)
	{
		if (pItem != NULL)
		{
			scsTreeItem *pRoot = getTopItem(static_cast<scsTreeItem *>(pItem));
			pRoot->itemDoubleClicked(static_cast<scsTreeItem *>(pItem), colume);
		}
	}

	void scsTree::keyPressEvent(QKeyEvent* event)
	{
		scsTreeItem *pItem = getSelectedItem();
		if (pItem != NULL)
		{
			scsTreeItem *pTopItem = getTopItem(pItem);
			pTopItem->keyPressEvent(event);
		}
	}

	QSize scsTree::sizeHint() const
	{
		return m_SizeHint;
	}

	void scsTree::setCustomSizeHint(const QSize &size)
	{
		m_SizeHint = size;
		updateGeometry();
	}

	void scsTree::changeEvent(QEvent * event)
	{
		if (event->type() == QEvent::LanguageChange)
		{
			std::map<int, scsTreeItem *>::iterator it = m_ItemMap.begin();
			for (; it != m_ItemMap.end(); ++it)
			{
				if (it->second != NULL)
					it->second->retranslate();
			}
		}
		QTreeWidget::changeEvent(event);
	}

	void scsTree::dropEvent(QDropEvent* event)
	{
		QList<QModelIndex> idxs = selectedIndexes();
		scsTreeItem* pItemDrag = dynamic_cast<scsTreeItem*>(itemFromIndex(idxs.at(0)));
		
		scsTreeItem* pItemDrop = dynamic_cast<scsTreeItem*>(itemAt(event->pos()));
		if (pItemDrop != NULL)
		{
			//printf(">> Drop     >> [%s]\n", pItem->text(0).toLocal8Bit().data());
			scsTreeItem* pTop = getTopItem(pItemDrag);
			if ((pTop == getTopItem(pItemDrop)) // Items of different sub-trees cannot be dragged
				&& (pItemDrag->getParentItem() != pItemDrop) // Items cannot be dragged to its parent node(without node change).
				&& (pItemDrop->flags().testFlag(Qt::ItemIsDropEnabled)))
			{
				pTop->itemToMove(pItemDrag, pItemDrop);
			}
			else
			{
			}
		}
		//在这里不做Item的移动，而只是发出itemToMove的信号，具体的操作交由子树去处理
		event->setDropAction(Qt::IgnoreAction);
		QTreeWidget::dropEvent(event);
	}
}
