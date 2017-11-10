#include "scsTreeWidget.h"

namespace scs_GUI{
	scsTreeWidget::scsTreeWidget(QWidget * parent, Qt::WindowFlags flags)
		: QWidget(parent, flags)
		, m_SizeHint(200, 200)
	{
		m_pTree = new scsTree;

		m_pVLayout = new QVBoxLayout(this);
		m_pVLayout->addWidget(m_pTree);
		m_pVLayout->setMargin(0);
		initTree();

		QObject::connect(m_pTree, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectionChanged()));
	}

	scsTreeWidget::~scsTreeWidget()
	{
		delete m_pTree;
		delete m_pVLayout;
	}
	void scsTreeWidget::initTree()
	{
		m_pModelTree = new scsModelTree(m_pTree);
	}
	void scsTreeWidget::slotSelectionChanged()
	{
		QList<scsTreeItem *> listItem = m_pTree->getSelectedItems();
		scsTreeItem *pTopItem = NULL;
		if (listItem.size() == 1)
		{
			pTopItem = m_pTree->getTopItem(listItem.at(0));
			pTopItem->changeSelectionTo(listItem.at(0));
		}
		else if (listItem.size() > 1)
		{
			pTopItem = m_pTree->getTopItem(listItem.at(0));
			pTopItem->itemSelectionChanged(listItem);
		}
		//emit signal to tell that ParameterListeners has changed.
	}
	QSize scsTreeWidget::sizeHint() const
	{
		return m_SizeHint;
	}

	void scsTreeWidget::setCustomSizeHint(const QSize &size)
	{
		m_SizeHint = size;
	}
}