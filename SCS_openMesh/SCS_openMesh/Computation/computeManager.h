#pragma  once
#include <para/computePara.h>
#include "../observer/antennaSubject.h"
#include <QString>

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
	vector<Site> &getSite();
	
	/*
	@brief ����������Ƿ��Ѿ�����
	@note ���桢С������
	*/
	bool checkPara();

	antennaSubject * getSubject(){ return subject; }
private:
	ComputePara* cptPara;
	antennaSubject *subject;
	bool siteFlag, antennaFlag, noSimFlag;
};

