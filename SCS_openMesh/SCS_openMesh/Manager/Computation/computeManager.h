#pragma  once
#include "Container/SitesContainer.h"
#include "../../observer/antennaSubject.h"
#include "observer/PluginSubject.h"
#include <QString>
#include <QtPlugin>
/*
	�㷨����
	1.�����ĵ��룬����
	2.�㷨ģ��Ĳ������
	3.�㷨dll�ĵ���
*/

class computeManager
{
public:
	computeManager();
	~computeManager();

	/*
	@brief �����߲����ļ�
	*/
	void openTransAntenna_ParamFile(QString path);

	/*
	@brief �򿪷Ƿ�������ܵ�,����cptPara
	*/
	void openNo_simplaneReceiver(string path);

	/*
	@brief �������ļ�
	*/
	void openTransAntennas_DirGain(QStringList path);

	/*
	@brief ���ؼ������
	*/
	inline ComputePara *getComputationPara(){ return cptPara; }

	/*
	@brief ����С��������
	*/
	vector<Site*> getSite();
	
	/*
	@brief ����������Ƿ��Ѿ�����
	@note ���桢С������
	*/
	bool checkPara();

	/**
	  * @Method:    setCptCatagory
	  * @Note: 	
	  * @Author:    Li Gen
	  * @ Date:     2017/11/17
	  * @Access:    public 
	  * @Returns:   void
	  * @Qualifier:
	  * @Parameter: ComputationEnum ce
	*/
	void setCptCatagory(ComputationEnum ce);

	void setCptCatagory(int mode);

	/**
	  * @Method:    getSitePosition
	  * @Note: 	���������е��±�id��ȡС����λ�á�ͨ���Ƿ��ص�һ��cell��λ��,id��0��ʼ����
	  * @Author:    Li Gen
	  * @ Date:     2017/11/22
	  * @Access:    public 
	  * @Returns:   Vector3d
	  * @Qualifier:
	  * @Parameter: int id
	*/
	Vector3d getSitePosition(int id);

	/**
	  * @Method:    transferContainerToPara
	  * @Note: 	��վ�����ݴ�������У�׼����������
	  * @Author:    Li Gen
	  * @ Date:     2017/12/12
	  * @Returns:   void
	*/
	void transferContainerToPara();

	SitesContainer* getContainer();

	antennaSubject * getSubject(){ return subject; }

	void setPluginPath(QString path);

	QString getPluginPath();

	PluginSubject * getPluginSubject();

	void sendPluginNewState();
private:
	SitesContainer* sitesContainer;
	ComputePara* cptPara;
	antennaSubject *subject;
	PluginSubject *m_pluginSubject;
	bool siteFlag;//�Ƿ��ȡ����
	bool antennaFlag;//�Ƿ��ȡ����
	bool noSimFlag;

	QString pluginPath;
};

