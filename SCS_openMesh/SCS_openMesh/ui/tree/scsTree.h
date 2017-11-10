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
		/*@brief ��ø����ڵ�������������TopLevel�ڵ�                                                                      */
		/************************************************************************/
		scsTreeItem *getTopItem(scsTreeItem* pItem);

		/**
		* @brief ���һ������ڵ�
		* @param [in] scsTreeItem * pItem Ҫ��ӵĽڵ����ָ��
		* @return int �¼���Ľڵ������е�ID�������������򷵻�-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addTopItem(scsTreeItem *pItem);

		/**
		* @brief ��ĳһ�ڵ���뵽ָ�����ڵ��е�ָ���ֵܽڵ����
		* @param [in] scsTreeItem * pItem Ҫ��ӵĽڵ����ָ��
		* @param [in] int iParentID ���ڵ��ID
		* @param [in] int iPrecedingID ǰһ�ֵܽڵ��ID
		* @return int �¼���Ľڵ������е�ID�������������򷵻�-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addItem(scsTreeItem *pItem, int iParentID, int iPrecedingID);

		/**
		* @brief ��ĳһ�ڵ�����µ��ӽڵ�
		* @param [in] scsTreeItem * pItem Ҫ��ӵĽڵ����ָ��
		* @param [in] int iParentID ���ڵ��ID
		* @return int �¼���Ľڵ������е�ID�������������򷵻�-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addItem(scsTreeItem *pItem, int iParentID);

		/**
		* @brief ��ѡ�нڵ�����µ��ӽڵ�
		* @param [in] scsTreeItem * pItem Ҫ��ӵĽڵ����ָ��
		* @return int �¼���Ľڵ������е�ID�������������򷵻�-1
		* @since 5/2/2017
		* @deprecated
		*/
		int addSubItemToSelectedItem(scsTreeItem *pItem);

		/**
		* @brief ͨ��Item��ID��ȡItem��ָ��
		* @param [in] int ID
		* @return scsTreeItem*
		* @since 5/2/2017
		* @deprecated
		*/
		const scsTreeItem* getItemByID(int ID);

		/**
		* @brief ��ȡ��ǰѡ�еĽڵ��ָ��
		* @return const scsTreeItem *
		* @since 5/2/2017
		* @deprecated
		*/
		scsTreeItem *getSelectedItem() const;

		QList<scsTreeItem *> getSelectedItems() const;

		/**
		* @brief ɾ��һ������ڵ㣬�������ӽڵ�Ҳ��һ����ɾ��
		* @param [in] int iItemID Ҫɾ���Ľڵ������е�ID
		* @return void
		* @since 5/2/2017
		* @deprecated
		*/
		void removeTopItem(int iItemID);

		/**
		* @brief ɾ��һ���ڵ㣬�������ӽڵ�Ҳ��һ����ɾ��
		* @param [in] int iItemID Ҫɾ���Ľڵ������е�ID
		* @return void
		* @since 5/2/2017
		* @deprecated
		*/
		void removeItem(int iItemID);

		/**
		* @brief ɾ��������Item�����ӽڵ�Ҳ�ᱻ�ݹ��ɾ��
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
		* @brief �ػ�ת�����нڵ�Item��updateDescription�ź�
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
		* @brief ���������Ըı�ʱ����Ӧ������Ӧ�ص�������Item��retranslate()����
		* @since 5/2/2017
		*/
		void changeEvent(QEvent * event);
		void dropEvent(QDropEvent* event);

		std::map<int, scsTreeItem *> m_ItemMap;
		int m_iIDMax; /**< ��ǰ�����Ѿ�ʹ��ID�����ֵ,Ϊ-1������û��Item*/
		QSize m_SizeHint;


	};
};