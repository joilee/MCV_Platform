/************************************************************************/
/*  计算参数的对话框 一个对话框拥有三个页面                                            */
/************************************************************************/

#ifndef COMPUTEOPTION
#define COMPUTEOPTION
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include "Context/context.h"
#include "computeOptionPage.h"
#include <QtWidgets/QFileDialog>
using namespace std;
class computerOptionDialog : public QDialog
{
	Q_OBJECT
public:
	computerOptionDialog(QWidget *parent = 0);

	emitSource* es;
	fieldpoint *fp;
	simuArgument *sa;
	
	public slots:
			void changePage(QListWidgetItem *current, QListWidgetItem *previous);
			void getPara();
			bool checkSiteAndAnte();
private:
	QListWidget *contentsWidget;
	QStackedWidget *pagesWidget;
	QPushButton *okButton;
	QPushButton *check;
};

#endif

