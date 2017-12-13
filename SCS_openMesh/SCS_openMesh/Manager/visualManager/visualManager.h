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

	void saveAllCellsFile(QString baseFilePath);

	EFieldContainer *getContainer();
private:
	EFieldContainer * visContainer;
	visPara * para;
	SiteCellSubject *m_SiteCellSubject;
};

