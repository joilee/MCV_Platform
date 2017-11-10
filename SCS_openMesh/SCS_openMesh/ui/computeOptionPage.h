#ifndef PAGES_H
#define PAGES_H
#include<QtGui>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QListWidget>
#include <string>
#include <QComboBox>
#include "../../observer/antennaObserver.h"
using namespace std;
/************************************************************************/
/* 
   发射源参数窗口     
	发射源包含天线、基站、雷达。
    参数通过文本的形式导入
	参数列表支持动态添加和删除，每个参数列表支持编辑。
*/
/************************************************************************/
class emitSource:public QWidget,public antennaObserver
{
	Q_OBJECT
	public:
		emitSource(QWidget* parent );		
		void update(visualAntennaItem*a);
		double getAngle();
		QPushButton *loadSitesButton;
		QPushButton *loadTransAntennaButton;
		QTreeWidget *sitesTreewidget;
		QListWidget * AntennaList;

	protected:

	private:
		double angleOfNorth;//正北方向在场景坐标系中的phi角度
		QGroupBox *firstGroup;
		QLineEdit * angleEdit;
		bool findSite(QString a);
		bool findAnte(string a);
		QTreeWidgetItem * getItem(QString a);
public:
		bool siteflag, gainFlag;
	private slots:
		void openTransAntennas_DirGain();
		void openTransAntenna_ParamFile();
};

class fieldpoint:public QWidget
{
	Q_OBJECT
public:
	fieldpoint(QWidget* parent=0);
	QPushButton * loadReceieverPointFile;
private:
	//数据
	//场景的范围，单位为m
//	double fieldXmin,fieldXmax,fieldYmin,fieldYmax,fieldZmin,fieldZmax;



	string RecePoint;

	//左上角x，左上角y
	double leftupX,leftupY;
	//右上角x,右上角y
	double rightbottomX,rightbottomY;
	//预测精度,预测高度。
	double predictPrecision,predictAltitude;

	//布局
	QLineEdit *filePath;
	QLineEdit* leftupXinput,*leftupYinput;
	QLineEdit* rightbottomXinput,*rightbottomYinput;
	QLineEdit* Precisioninput,*Altitudeinput;
public:
	//获得参数
	void getFieldPoint(double &lx,double &ly,double &rx,double &ry,double &pre,double &alti);
};

class simuArgument:public QWidget
{
	Q_OBJECT
public:
	simuArgument(QWidget* parent=0);



private:

	double  reflectNum;
	double  refractNum;
	double  diffractionNum;
	bool isDiffraction;

	double RT_sample;//采样球密度
	double RT_radius;//采样球半径
	double RT_BeamNum;//beam数量

public:
	QLineEdit* lineEdit_RT_ReflectCount;
	QLineEdit* lineEdit_RT_RefractCount;
	QComboBox * comboBox_diffraction;
	QComboBox* comboBox_transmission;
	QLineEdit* lineEdit_RT_Diffraction;
	QLineEdit * lineEdit_RT_sample;
	QLineEdit*lineEdit_RT_sample_radius;
	QLineEdit*lineEdit_RT_sampleBeamNum;
	void getSimuArgu(double &reflect,double &refract,double &diffract,bool &isDiff,int transIndex,double &sample,double &radius,double &beamNum);
};

#endif