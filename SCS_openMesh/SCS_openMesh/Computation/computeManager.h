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
	  * @Note: 	根据数组中的下标id获取小区的位置。通常是返回第一个cell的位置,id从0开始计数
	  * @Author:    Li Gen
	  * @ Date:     2017/11/22
	  * @Access:    public 
	  * @Returns:   Vector3d
	  * @Qualifier:
	  * @Parameter: int id
	*/
	Vector3d getSitePosition(int id);

	antennaSubject * getSubject(){ return subject; }
private:
	ComputePara* cptPara;
	antennaSubject *subject;
	bool siteFlag, antennaFlag, noSimFlag;
};

