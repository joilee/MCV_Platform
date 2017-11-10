#pragma  once
#include <para/computePara.h>
#include "../observer/antennaSubject.h"
#include <QString>

/*
	算法管理
	1.参数的导入，更改
	2.算法模块的参数设计
	3.算法dll的导入
*/

class computeManager
{
public:
	computeManager();
	~computeManager();

	/*
	@brief 打开天线参数文件
	*/
	void openTransAntenna_ParamFile(QString path);

	/*
	@brief 打开非仿真面接受点,放入cptPara
	*/
	void openNo_simplaneReceiver(string path);

	/*
	@brief 打开增益文件
	*/
	void openTransAntennas_DirGain(QStringList path);

	/*
	@brief 返回计算参数
	*/
	inline ComputePara *getComputationPara(){ return cptPara; }

	/*
	@brief 返回小区的引用
	*/
	vector<Site> &getSite();
	
	/*
	@brief 检查计算参数是否都已经存在
	@note 增益、小区天线
	*/
	bool checkPara();

	antennaSubject * getSubject(){ return subject; }
private:
	ComputePara* cptPara;
	antennaSubject *subject;
	bool siteFlag, antennaFlag, noSimFlag;
};

