#pragma once
#include <para/visualPara.h>
#include <Container/EFieldContainer.h>
#include <QString>
#include "observer/SiteCellSubject.h"
/*
���ӻ�����
1.ɢ��ͼչʾ
2.������棨����ӣ�
3.������չʾ������ӣ�
*/

class VisualManager
{
public:
	VisualManager();
	~VisualManager();


	/**
	  * @Method:    setContainerData
	  * @Note: 	���������洢��������������ǲ�������������У��Ѿ�����ģ�ʵ�����ݡ�
	  * @Author:    Li Gen
	  * @ Date:     2017/12/10
	  * @Returns:   void
	  * @Parameter: 
	*/
	void setContainerData();

	visPara* getVisualPara();

	/**
	  * @Method:    sendSiteCellName
	  * @Note: 	�������� �۲���ģʽ
	  * @Author:    Li Gen
	  * @ Date:     2017/12/12
	  * @Returns:   void
	*/
	void sendSiteCellName();

	/**
	  * @Method:    saveCellFile
	  * @Note: 	cell���ݵ���
	  * @Author:    Li Gen
	  * @ Date:     2017/12/12
	  * @Returns:   void
	*/
	void saveCellFile(QString dir, int pci,int siteID);

	void loadMeasuredFile(QString file);

	void correct(Vector3d &before, Vector3d &after);

	/**
	  * @Method:    leastSquare
	  * @Note: 	��ȡУ����ʼλ��
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

