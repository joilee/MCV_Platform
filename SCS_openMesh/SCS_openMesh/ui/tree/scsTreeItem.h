#pragma once

#include <vector>
#include <QWidget>
#include <QTreeWidgetItem>
using namespace std;

namespace scs_GUI{
	class scsTree;
	class scsTreeItem :public QObject, public QTreeWidgetItem
	{
		Q_OBJECT

	public:
		scsTreeItem();
		scsTreeItem(scsTree *parent);
		scsTreeItem(scsTreeItem *parent);
		~scsTreeItem();
		void setParentTree(scsTree *parent);
		scsTree* getParentTree();

		/**
		* @brief Return the pointer to parent item
		* @return scsTreeItem* Parent item.NULL if no parent.
		* @since 5/3/2017
		*/
		scsTreeItem* getParentItem();

		/**
		* @brief 返回指定index的子节点
		* @param [in] idx int 子节点的索引
		* @return scsTreeItem* 子节点的指针
		* @since 5/3/2017
		*/
		scsTreeItem* getChildItem(int idx);

		/**
		* @brief 获取给定名称的子节点
		* @param [in] name const QString & 节点的名称
		* @return scsTreeItem* 子节点指针，如果找不到给定名称的节点则返回NULL
		* @since 5/3/2017
		*/
		scsTreeItem* getChildItem(const QString& name);

		/**
		* @brief 获取选中的子节点，如果有多个子节点被选中则返回第一个
		* @return scsTreeItem * 子节点指针，如果当前节点没有子节点被选中则返回NULL
		* @since 5/3/2017
		*/
		scsTreeItem *getSelectedItem() const;

		/**
		* @brief 获取所有选中的子节点
		* @return QList<scsTreeItem*> 子节点列表，如果当前节点没有子节点被选中则返回一个空的列表
		* @since 5/3/2017
		*/
		QList<scsTreeItem*> getSelectedItems() const;

		int addSubItem(scsTreeItem *pItem);

		int addSubItem(scsTreeItem *pItem, scsTreeItem *pParentItem);

		int addSubItem(scsTreeItem *pItem, scsTreeItem *pParentItem, scsTreeItem *pPrecedingItem);

		/**
		* @brief Expand the tree to given item
		* @param [in] pItem scsTreeItem * 要展开至的节点
		* @return void
		* @since 5/3/2017
		*/
		void expandToItem(scsTreeItem* pItem);

		/**
		* @brief 判断当前树中是否有子节点处于Checked状态
		* @return bool
		* @since 5/3/2017
		*/
		bool hasChildrenChecked();

		/**
		* @brief Set whether current item is checkable
		* @param [in] able bool true if Checkable, false for not
		* @return void
		* @since 5/3/2017
		*/
		void setCheckable(bool able);

		/**
		* @brief 设置当前节点的Check状态
		* QTreeItem是可以有多列的，此函数默认设置第一列的Check状态。如果要设置多列节点的Check状态，
		* 可以使用@see setCheckState(int col, Qt::CheckState state)
		* @param [in] state Qt::CheckState 节点的Check状态
		* @return void
		* @since 5/3/2017
		*/
		inline void setCheckState(Qt::CheckState state);

		/**
		* @brief 设置当前节点给定列的Check状态
		* @param [in] col int 要设置的列序号，从0开始
		* @param [in] state Qt::CheckState 节点的Check状态
		* @return void
		* @since 5/3/2017
		*/
		void setCheckState(int col, Qt::CheckState state);

		/**
		* @brief 改变一个节点的Check状态，同时改变所有子节点的状态与其一致，并检查并改变父节点的状态
		* @param [in] state Qt::CheckState 节点的Check状态
		* @return void
		* @since 5/3/2017
		*/
		virtual void setCheckStateRecursive(Qt::CheckState state);

		/**
		* @brief 递归地更改所有子节点的Check状态为给定状态
		* @param [in] state Qt::CheckState 节点的Check状态
		* @return void
		* @since 5/3/2017
		*/
		void setChildCheckStateRecursive(Qt::CheckState state);

		/**
		* @brief 更新当前节点的Check状态
		*		  根据所有子节点的Check状态将当前节点的Check状态设置为
		*		  Qt::Checked,Qt::Unchecked,Qt::PartiallyChecked三种状态之一。
		* @return void
		* @since 5/3/2017
		*/
		void updateCheckState();

		/**
		* @brief 递归地更新父节点的Check状态
		* @return void
		* @since 5/3/2017
		*/
		void updateParentCheckState();

		void removeItem(scsTreeItem *pItem);

		/**
		* @brief 删除所有的子节点
		* @return void
		* @since 5/3/2017
		*/
		void removeAllChildren();

		/**
		* @brief 判断树中是否有名称为itemName的Item
		* @param [in] itemName QString
		* @return bool 如果有则返回true，否则返回false
		* @since 5/3/2017
		* @deprecated
		*/
		bool contains(QString itemName);

		int getID();
		void setID(int ID);

		/**
		* @brief 重载此函数，设置右键菜单及其响应。
		*/
		virtual void showContextMenu(const QPoint & position, scsTreeItem * pItem){};

		/**
		* @brief 重载此函数，设置对选择Item改变时的响应
		*/
		virtual void changeSelectionTo(scsTreeItem *pItem){};

		/**
		* 重载此函数，设置对选择多个Item时的响应
		*/
		virtual void itemSelectionChanged(QList<scsTreeItem *> pItems){};

		/**
		* 重载此函数，设置对Item改变时的响应
		*/
		virtual void itemChanged(scsTreeItem * item, int column){};

		virtual void itemDoubleClicked(scsTreeItem * item, int column){};

		virtual void keyPressEvent(QKeyEvent* event){};

		/**
		* @brief 重载此函数，设置程序语言改变时的响应
		* @return void
		* @since 5/3/2017
		*/
		virtual void retranslate();

		virtual void itemToMove(scsTreeItem* from, scsTreeItem* to){};

	signals:
		void updateDescription();

		public:
				scsTreeItem* getItemFromPath(const vector<string> &componentPath);
				vector<string> getPathToRoot(scsTreeItem* pItem = NULL);

		protected:
				void outputErrorMessage(QString text);		// 输出参数输入错误的信息

		protected:
				int m_iID;
				bool m_bMultiSelection;
				scsTree* m_pParentTree;
	};
};