#ifndef  MESHOPTION_H
#define MESHOPTION_H
#include "util/vector.h"
#include <QDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
class meshOptionDialog:public QDialog
{
	Q_OBJECT
public:
	meshOptionDialog(QWidget *parent=0);
	~meshOptionDialog();
public:
	vector<Vector3d> center;//sites可能有多个
	vector<int> siteName;
	double range;
	bool inputFlag;

private slots:
	void onOkbutton();
	void onRefreshButton();
	void dynamicLoadSite(int);
	void meshModeButtonToggled(int, bool);
public:
	void createActions();
	void setTheLayout();
	void getValue(vector<Vector3d> &_center,double &_range);
	void enableSingleMode(bool flag);
	void enableMultiMode(bool flag);
	QButtonGroup * meshModeGroup;
	//单个模型模式
	QRadioButton *singleModelMode;
	//多个模式模式（多个模型不用显示仿真面和散点图）
	QRadioButton *MultiModelMode;
	QComboBox * cbo_XYZ;
	QGroupBox * firstGroup;
	QLineEdit *centerXLE;
	QLineEdit *centerYLE;
	QLineEdit *centerZLE;
	QLineEdit *rangeLE;
	QPushButton *okbutton;
	QPushButton* exitbutton;
	QPushButton* refreshButton;
	enum ModelNum
	{
		SINGLE_MODEL=1,
		MULTI_MODEL=2
	} modelNum;

private:

};



#endif