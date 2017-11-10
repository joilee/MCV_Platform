#pragma once
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <iostream>
#include <iomanip>
#include "scsTreeItem.h"
#include "scsTree.h"
#include "../../Model/abstractModel.h"

namespace scs_GUI{
	class scsModelTree :public scsTreeItem
	{
		Q_OBJECT
	public:
		scsModelTree();
		scsModelTree(scsTree *parent);
		scsModelTree(scsTreeItem *parent);
		~scsModelTree();

		//////////////////////////////////////////////////////////////////////////
		// scsTreeItem Interface
		void showContextMenu(const QPoint & position, scsTreeItem *pItem);
		void init();
		void changeSelectionTo(scsTreeItem *pItem);
		void itemChanged(scsTreeItem * item, int column);
		virtual void retranslate();
		virtual void itemToMove(scsTreeItem* from, scsTreeItem* to);

		/**
		* @brief Handle event when multi items are selected
		*
		* Because selected items are not filtered by tree, items belong to other
		* trees may be included in the list. This should be processed here.
		*
		* @param [in] QList<scsTreeItem * > pItems Selected items
		* @since 5/8/2017
		*/
		void itemSelectionChanged(QList<scsTreeItem *> pItems);

		//////////////////////////////////////////////////////////////////////////
		// scsModelListener Class Interface
		/**
		* @brief invoked when 'component' has been inserted into 'componentPath'
		* @param [in] abstractMode * component
		* @param [in] const vector<string> & componentPath
		* @since 5/8/2017
		*/
		void ComponentInserted(abstractModel* component, const vector<string> &componentPath);

		public slots:
		void slotActionDelete();
		void slotActionCreate();
		void slotActionExport();

	private:
		QString getFilterString(const vector<pair<string, string>>& formatInfo);
	};

}