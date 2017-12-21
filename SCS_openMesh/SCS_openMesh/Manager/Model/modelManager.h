#pragma  once


#include  "../../observer/modelSubject.h"
#include "observer/LocalModelSubject.h"
#include "Container/ModelContainer.h"
#include "../Material/materialManager.h"
#include <map>
#include "Model/OBJModel/objModelFactory.h"
#include "Model/OBJModel/objLocalModelFactory.h"


class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	/*
	@brief 在map中插入模型
	*/
	void  insertGlobalModel(abstractGlobalModel* model);

	/*
	@brief 在map中删除模型，根据模型指针
	*/
	void  deleteGlobalModel(abstractGlobalModel* model);

	/*
	@brief 在map中删除模型，根据模型独一无二的名字
	*/
	void  deleteGLobalModel(string name);

	/*
	@brief 在map中删除模型，根据模型id
	*/
	void deleteGlobalModel(int id);


	/*
	@brief 在map中插入模型
	*/
	void  insertLocalModel(abstractLocalModel* model);

	/*
	@brief 在map中删除模型，根据模型指针
	*/
	void  deleteLocalModel(abstractLocalModel* model);

	/*
	@brief 在map中删除模型，根据模型独一无二的名字
	*/
	void  deleteLocalModel(string name);

	/*
	@brief 在map中删除模型，根据模型id
	*/
	void deleteLocalModel(int id);


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
	bool generateLocalModel(vector< Vector3d> center, vector<int> siteName, double range);

	/*
	@brief 返回私有变量
	*/
	modelSubject *getModelSubject(){ return m_subject; }

	LocalModelSubject * getLocalSubject(){ return m_local_subject; }
	/* 
	@brief 将城市模型名称和城市场景数据发送到相应的观察者
	@return void
	@note 推送建筑物数据
	*/
	void getGlobalNewState();

	void getLocalNewState();

	void sendLocalStateByID(int id);

	void sendAllModelName();
	/*
	@brief 检测局部模型是否存在，即是否有可以存在的计算的模型
	@return bool
	@note 
	*/
	bool checkLocalExist();

	bool checkLocalExistByID(int id);

	/*
	@brief  返回局部模型
	@return cityLocalModel
	@note  根据map遍历返回第一个局部模型，如果没有返回NULL
	*/
	abstractLocalModel * getFirstLocal();
	abstractLocalModel * getLocalModelByID(int id);
	/*
	@brief 检测全局模型是否存在，即是否有可以剖分的模型
	*/
	bool checkCityExist();

	/*
	@brief 返回第一个城市模型
	*/
	abstractGlobalModel * getFirstCity();

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

	/*
	@brief 是否要绘制城市场景
	*/
	bool getWholeModelFlag();

	void setWholeModelFlag(bool a);

	void setLocalShowID(int id);

	int getLocalShowID();

	void loadGlobalObj(string objPath);

	void loadLocalObj(vector<Vector3d> center, double localRange, vector<int> siteName);
private:

	cityLocalModelFactory *localFac;
	cityModelFactory *cityFac;
	objModelFactory *objFac;
	objLocalModelFactory *objLocalFac;
	modelSubject * m_subject;
	LocalModelSubject * m_local_subject;

	//将要展示的局部模型
	int LocalModelID_SHOW;



	//
	ModelPara * modelPara;
	ModelContainer *modelContainer;
public:
	scsMaterialManager * matManager;
};