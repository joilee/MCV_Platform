#pragma once


#include <QWidget>
#include <QVBoxLayout>
#include "scsModelTree.h"
#include "scsTree.h"


using namespace scs_GUI;


namespace scs_GUI{
	class scsTreeWidget:public QWidget
	{
		Q_OBJECT
	public:
		scsTreeWidget(QWidget * parent = 0, Qt::WindowFlags flags = 0);
		~scsTreeWidget();
		void initTree();
		void setCustomSizeHint(const QSize &size);
		QSize sizeHint() const;
	public slots:
		void slotSelectionChanged();
	private:
		QVBoxLayout *m_pVLayout;
		QSize m_SizeHint;
		scsTree *m_pTree;
		scsModelTree *m_pModelTree;

	};
};