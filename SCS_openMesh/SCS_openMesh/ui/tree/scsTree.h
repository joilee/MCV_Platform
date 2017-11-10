#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QList>

using namespace std;
namespace scs_GUI
{
	class scsTreeItem;
	class scsTree :public QTreeWidget
	{
		Q_OBJECT
	public:
		scsTree(QWidget *parent = 0);
		~scsTree();

		int getID(scsTreeItem *pItem);

		/************************************************************************/
		/*@brief 获得给定节点在树中所属的TopLevel节点                                                                      */
		/************************************************************************/
		scsTreeItem *getTopItem(scsTreeItem* pItem);

		/**
		* @brief 添加一个顶层节点
		* @param [in] scsTreeItem * pItem 要添加的节点对象指针
		* @return int 新加入的节点在树中的ID。如果插入错误则返回-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addTopItem(scsTreeItem *pItem);

		/**
		* @brief 将某一节点插入到指定父节点中的指定兄弟节点后面
		* @param [in] scsTreeItem * pItem 要添加的节点对象指针
		* @param [in] int iParentID 父节点的ID
		* @param [in] int iPrecedingID 前一兄弟节点的ID
		* @return int 新加入的节点在树中的ID。如果插入错误则返回-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addItem(scsTreeItem *pItem, int iParentID, int iPrecedingID);

		/**
		* @brief 向某一节点添加新的子节点
		* @param [in] scsTreeItem * pItem 要添加的节点对象指针
		* @param [in] int iParentID 父节点的ID
		* @return int 新加入的节点在树中的ID。如果插入错误则返回-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addItem(scsTreeItem *pItem, int iParentID);

		/**
		* @brief 向选中节点添加新的子节点
		* @param [in] scsTreeItem * pItem 要添加的节点对象指针
		* @return int 新加入的节点在树中的ID。如果插入错误则返回-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addSubItemToSelectedItem(scsTreeItem *pItem);

		/**
		* @brief 通过Item的ID获取Item的指针
		* @param [in] int ID
		* @return scsTreeItem*
		* @since 5/2/2017
		* @deprecated
		*/
		const scsTreeItem* getItemByID(int ID);

		/**
		* @brief 获取当前选中的节点的指针
		* @return const scsTreeItem *
		* @since 5/2/2017
		* @deprecated
		*/
		scsTreeItem *getSelectedItem() const;

		QList<scsTreeItem *> getSelectedItems() const;

		/**
		* @brief 删除一个顶层节点，其所有子节点也会一并被删除
		* @param [in] int iItemID 要删除的节点在树中的ID
		* @return void
		* @since 5/2/2017
		* @deprecated
		*/
		void removeTopItem(int iItemID);

		/**
		* @brief 删除一个节点，其所有子节点也会一并被删除
		* @param [in] int iItemID 要删除的节点在树中的ID
		* @return void
		* @since 5/2/2017
		* @deprecated
		*/
		void removeItem(int iItemID);

		/**
		* @brief 删除给定的Item，其子节点也会被递归的删除
		* @param [in] scsTreeItem * pItem
		* @return void
		* @since 12/10/2009
		* @deprecated
		*/
		void deleteItem(scsTreeItem* pItem);

		virtual QSize sizeHint() const;
		void setCustomSizeHint(const QSize &size);

		public slots:
		void setSelectedItem(scsTreeItem *pItem);
		void slotSelectionChanged();

		private slots:
		void slotContextMenuRequested(const QPoint & position);

		void slotItemChanged(QTreeWidgetItem *pItem, int colume);
		void slotItemDoubleClicked(QTreeWidgetItem *pItem, int colume);

		/**
		* @brief 截获并转发树中节点Item的updateDescription信号
		* @return void
		* @since 5/2/2017
		* @deprecated
		*/
		void slotUpdateDescription();

	signals:
		void updateDescription();

	protected:
		void keyPressEvent(QKeyEvent* event);
		/**
		* @brief 当程序语言改变时的响应，会相应地调用所有Item的retranslate()函数
		* @since 5/2/2017
		*/
		void changeEvent(QEvent * event);
		void dropEvent(QDropEvent* event);

		std::map<int, scsTreeItem *> m_ItemMap;
		int m_iIDMax; /**< 当前所有已经使用ID的最大值,为-1则树中没有Item*/
		QSize m_SizeHint;


	};
};