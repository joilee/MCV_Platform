#ifndef INFOPAGE_H
#define  INFOPAGE_H
#include<QtGui>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include "util/vector.h"
class BuildingInfoPage:public QWidget
{
	Q_OBJECT
public:
	BuildingInfoPage(QWidget* parent =0);
	void init();
	void setLineEdit();
	void setView();//…Ë÷√≤ºæ÷
	void setValue(int c,int b,Vector3d bmin ,Vector3d bmax);
	QGroupBox *firstGroup;
	QGroupBox *secondGroup;

	QLineEdit *buildingLE;
	QLineEdit*concaveLE;
	QLineEdit*XminLE,*YminLE,*ZminLE;
	QLineEdit*XmaxLE,*YmaxLE,*ZmaxLE;
	int concaveBuilding;
	int buildingCount;
	Vector3d boundingMin,boundingMax;
};


class localPage:public QWidget
{
	Q_OBJECT
public:
	localPage(QWidget *parent=0);

	QGroupBox *firstGroup;
	QLineEdit * triLE;
	QLineEdit*XminLE,*YminLE,*ZminLE;
	QLineEdit*XmaxLE,*YmaxLE,*ZmaxLE;
	int triangleNum;
	Vector3d boundingMin,boundingMax;
	void init();
	void setLineEditLocal();
	void setView();
	void setParametre(int num,Vector3d minNum,Vector3d maxNum);

};
#endif
