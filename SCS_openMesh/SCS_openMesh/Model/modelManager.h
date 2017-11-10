#pragma  once

#include "abstractModel.h"
#include "abstractModelFactory.h"
#include  "cityModel/cityModelFactory.h"
#include  "../observer/modelSubject.h"
#include <para/modelPara.h>
#include "../Material/materialManager.h"
#include <map>
class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	/*
	@brief 在map中插入模型
	*/
	void  insertModel(abstractModel* model);

	/*
	@brief 在map中删除模型，根据模型指针
	*/
	void  deleteModel(abstractModel* model);

	/*
	@brief 在map中删除模型，根据模型独一无二的名字
	*/
	void  deleteModel(string name);

	/*
	@brief 载入城市模型
	*/
	void loadCityModel(string path);
	
	/*
	@brief 从城市场景中构造局部场景
	@param [in] Vector3d center
	@param [in]  double range
	@return bool
	@note 当城市场景不存在时候，返回异常
	@since 5/14/2017
	*/
	bool generateLocalModel(Vector3d center,double range);

	/*
	@brief 返回私有变量
	*/
	modelSubject *getModelSubject(){ return m_subject; }

	/* 
	@brief 将最新的状态发送到相应的观察者
	@return void
	@note 现在只支持一个城市场景和一个局部场景存在。即map中有多个场景时，只针对遍历时候第一个遇到的有效
	*/
	void sendNewState();

	/*
	@brief 检测局部模型是否存在，即是否有可以存在的计算的模型
	@return bool
	@note 
	*/
	bool checkLocalExist();

	/*
	@brief  返回局部模型
	@return cityLocalModel
	@note  根据map遍历返回第一个局部模型，如果没有返回NULL
	*/
	cityLocalModel * getFirstLocal();

	/*
	@brief 检测全局模型是否存在，即是否有可以剖分的模型
	*/
	bool ModelManager::checkCityExist();

	/*
	@brief 返回第一个城市模型
	*/
	cityModel * getFirstCity();

	/*
	@brief 返回不透明度
	*/
	inline double getAlpha(){ return transparency; }

	/*
	@brief 是否要绘制局部场景
	*/
	inline bool drawLocalFlag(){ return drawTriangleScene; }

	/*
	@brief 返回计算所需要的模型参数
	*/
	ModelPara* getModelPara(){ return modelPara; }


	//************************************
	// Method:    setModelPara
	// Note: 设置计算参数
	// FullName:  ModelManager::setModelPara
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void setModelPara();
private:

	map<string, abstractModel*> modelMap;
	cityModelFactory *cityFac;
	modelSubject * m_subject;

	//场景透明度
	double transparency;
	bool drawTriangleScene; //局部三角形文件场景绘制flag

	//
	ModelPara * modelPara;

public:
	scsMaterialManager * matManager;
};