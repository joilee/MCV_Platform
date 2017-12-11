#pragma once
#include <para/visualPara.h>
#include <Container/EFieldContainer.h>
#include <QString>
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
	  * @Parameter: visPara * a
	*/
	void setContainerData(visPara * a);

private:
	EFieldContainer * visContainer;
};

