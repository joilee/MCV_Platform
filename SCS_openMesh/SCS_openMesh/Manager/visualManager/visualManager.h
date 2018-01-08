#pragma once
#include <para/visualPara.h>
#include <Container/EFieldContainer.h>
#include <QString>
#include "observer/SiteCellSubject.h"
/*
可视化管理
1.散点图展示
2.结果保存（待添加）
3.仿真面展示（待添加）
*/

class VisualManager
{
public:
	VisualManager();
	~VisualManager();


	/**
	  * @Method:    setContainerData
	  * @Note: 	将仿真结果存储到结果容器，但是并不会更改容器中（已经导入的）实测数据。
	  * @Author:    Li Gen
	  * @ Date:     2017/12/10
	  * @Returns:   void
	  * @Parameter: 
	*/
	void setContainerData();

	visPara* getVisualPara();

	/**
	  * @Method:    sendSiteCellName
	  * @Note: 	发送数据 观察者模式
	  * @Author:    Li Gen
	  * @ Date:     2017/12/12
	  * @Returns:   void
	*/
	void sendSiteCellName();

	/**
	  * @Method:    saveCellFile
	  * @Note: 	cell数据导出
	  * @Author:    Li Gen
	  * @ Date:     2017/12/12
	  * @Returns:   void
	*/
	void saveCellFile(QString dir, int pci,int siteID);

	void loadMeasuredFile(QString file);

	void correct(Vector3d &before, Vector3d &after);

	/**
	  * @Method:    leastSquare
	  * @Note: 	获取校正起始位置
	  * @Author:    Li Gen
	  * @ Date:     2017/12/19
	  * @Returns:   void
	  * @Parameter: double & a
	  * @Parameter: double & b
	  * @Parameter: double & weight
	  * @Parameter: int & site_id
	  * @Parameter: int & PCI_id
	  * @Parameter: int & Field_id
	*/
	void leastSquare(double &a, double &b, double &weight, int &site_id, int &PCI_id, int &Field_id);

	EFieldContainer *getContainer();

	vector<double> getBeforeVec(){ return beforeCorrection_calculationfield; }
	vector<double> getAfterVec(){ return afterCorrection_calculationfield; }
	vector<double> getMeasured(){ return Measuredfield; }

	SiteCellSubject *getSiteSubject(){ return m_SiteCellSubject; }

private:
	EFieldContainer * visContainer;
	visPara * para;
	SiteCellSubject *m_SiteCellSubject;

	vector<double>beforeCorrection_calculationfield;
	vector<double>afterCorrection_calculationfield;
	vector<double>Measuredfield;
};

