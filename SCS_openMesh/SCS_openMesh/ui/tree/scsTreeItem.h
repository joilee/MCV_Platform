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
		* @brief ����ָ��index���ӽڵ�
		* @param [in] idx int �ӽڵ������
		* @return scsTreeItem* �ӽڵ��ָ��
		* @since 5/3/2017
		*/
		scsTreeItem* getChildItem(int idx);

		/**
		* @brief ��ȡ�������Ƶ��ӽڵ�
		* @param [in] name const QString & �ڵ������
		* @return scsTreeItem* �ӽڵ�ָ�룬����Ҳ����������ƵĽڵ��򷵻�NULL
		* @since 5/3/2017
		*/
		scsTreeItem* getChildItem(const QString& name);

		/**
		* @brief ��ȡѡ�е��ӽڵ㣬����ж���ӽڵ㱻ѡ���򷵻ص�һ��
		* @return scsTreeItem * �ӽڵ�ָ�룬�����ǰ�ڵ�û���ӽڵ㱻ѡ���򷵻�NULL
		* @since 5/3/2017
		*/
		scsTreeItem *getSelectedItem() const;

		/**
		* @brief ��ȡ����ѡ�е��ӽڵ�
		* @return QList<scsTreeItem*> �ӽڵ��б������ǰ�ڵ�û���ӽڵ㱻ѡ���򷵻�һ���յ��б�
		* @since 5/3/2017
		*/
		QList<scsTreeItem*> getSelectedItems() const;

		int addSubItem(scsTreeItem *pItem);

		int addSubItem(scsTreeItem *pItem, scsTreeItem *pParentItem);

		int addSubItem(scsTreeItem *pItem, scsTreeItem *pParentItem, scsTreeItem *pPrecedingItem);

		/**
		* @brief Expand the tree to given item
		* @param [in] pItem scsTreeItem * Ҫչ�����Ľڵ�
		* @return void
		* @since 5/3/2017
		*/
		void expandToItem(scsTreeItem* pItem);

		/**
		* @brief �жϵ�ǰ�����Ƿ����ӽڵ㴦��Checked״̬
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
		* @brief ���õ�ǰ�ڵ��Check״̬
		* QTreeItem�ǿ����ж��еģ��˺���Ĭ�����õ�һ�е�Check״̬�����Ҫ���ö��нڵ��Check״̬��
		* ����ʹ��@see setCheckState(int col, Qt::CheckState state)
		* @param [in] state Qt::CheckState �ڵ��Check״̬
		* @return void
		* @since 5/3/2017
		*/
		inline void setCheckState(Qt::CheckState state);

		/**
		* @brief ���õ�ǰ�ڵ�����е�Check״̬
		* @param [in] col int Ҫ���õ�����ţ���0��ʼ
		* @param [in] state Qt::CheckState �ڵ��Check״̬
		* @return void
		* @since 5/3/2017
		*/
		void setCheckState(int col, Qt::CheckState state);

		/**
		* @brief �ı�һ���ڵ��Check״̬��ͬʱ�ı������ӽڵ��״̬����һ�£�����鲢�ı丸�ڵ��״̬
		* @param [in] state Qt::CheckState �ڵ��Check״̬
		* @return void
		* @since 5/3/2017
		*/
		virtual void setCheckStateRecursive(Qt::CheckState state);

		/**
		* @brief �ݹ�ظ��������ӽڵ��Check״̬Ϊ����״̬
		* @param [in] state Qt::CheckState �ڵ��Check״̬
		* @return void
		* @since 5/3/2017
		*/
		void setChildCheckStateRecursive(Qt::CheckState state);

		/**
		* @brief ���µ�ǰ�ڵ��Check״̬
		*		  ���������ӽڵ��Check״̬����ǰ�ڵ��Check״̬����Ϊ
		*		  Qt::Checked,Qt::Unchecked,Qt::PartiallyChecked����״̬֮һ��
		* @return void
		* @since 5/3/2017
		*/
		void updateCheckState();

		/**
		* @brief �ݹ�ظ��¸��ڵ��Check״̬
		* @return void
		* @since 5/3/2017
		*/
		void updateParentCheckState();

		void removeItem(scsTreeItem *pItem);

		/**
		* @brief ɾ�����е��ӽڵ�
		* @return void
		* @since 5/3/2017
		*/
		void removeAllChildren();

		/**
		* @brief �ж������Ƿ�������ΪitemName��Item
		* @param [in] itemName QString
		* @return bool ������򷵻�true�����򷵻�false
		* @since 5/3/2017
		* @deprecated
		*/
		bool contains(QString itemName);

		int getID();
		void setID(int ID);

		/**
		* @brief ���ش˺����������Ҽ��˵�������Ӧ��
		*/
		virtual void showContextMenu(const QPoint & position, scsTreeItem * pItem){};

		/**
		* @brief ���ش˺��������ö�ѡ��Item�ı�ʱ����Ӧ
		*/
		virtual void changeSelectionTo(scsTreeItem *pItem){};

		/**
		* ���ش˺��������ö�ѡ����Itemʱ����Ӧ
		*/
		virtual void itemSelectionChanged(QList<scsTreeItem *> pItems){};

		/**
		* ���ش˺��������ö�Item�ı�ʱ����Ӧ
		*/
		virtual void itemChanged(scsTreeItem * item, int column){};

		virtual void itemDoubleClicked(scsTreeItem * item, int column){};

		virtual void keyPressEvent(QKeyEvent* event){};

		/**
		* @brief ���ش˺��������ó������Ըı�ʱ����Ӧ
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
				void outputErrorMessage(QString text);		// �����������������Ϣ

		protected:
				int m_iID;
				bool m_bMultiSelection;
				scsTree* m_pParentTree;
	};
};