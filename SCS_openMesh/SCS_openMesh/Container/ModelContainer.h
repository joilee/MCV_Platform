#pragma  once
#include <map>
#include "Model/abstractModel/abstractModel.h"
#include "Model/cityModel/cityLocalModel.h"
#include "Model/cityModel/cityModel.h"
#include "Model/cityModel/cityLocalModelFactory.h"
#include "Model/cityModel/cityModelFactory.h"

#include <para/modelPara.h>
#define NO_LOCAL_MODEL_ID -1111

//负责模型的增删查
class ModelContainer
{
public:
	ModelContainer();
	~ModelContainer();

public:
	void  insertGlobalModel(abstractGlobalModel* model);
	void  insertLocalModel(abstractLocalModel* model);

	void  deleteGlobalModel(abstractGlobalModel* model);
	void  deleteGlobalModel(string name);
	void  deleteGlobalModel(int id);

	void  deletelocalModel(abstractLocalModel* model);
	void  deletelocalModel(string name);
	void  deletelocalModel(int id);

	bool  isLocalExist();
	bool  isLocalType(int id);
	bool  isWholeType(int id);
	int getGlobalModelSize(){ return globalMap.size(); }
	int  getLocalModelSize(){ return localMap.size(); }

	abstractLocalModel * getFirstLocal();
	abstractLocalModel  * getLocalModelByID(int id);
	bool isCityExist();
	bool isLocalIDExist(int id);
	abstractGlobalModel* getFirstCity();

	//返回所有id
	vector<int> getGlobalIDs();
	vector<int> getLocalModelID();
	//返回所有名称
	vector<string> getGlobalNames();
	vector<string> getlocalNames();

	vector<Vector3d> getCityBoundingBox();
	int getCityBuildingNum();
	int getCityConcaveNum();
	int getLocalFaceNum(int id=NO_LOCAL_MODEL_ID);
	vector<Vector3d> getLocalBountdingBox(int id = NO_LOCAL_MODEL_ID);
private:
	map<int, abstractLocalModel*> localMap;
	map<int, abstractGlobalModel*>globalMap;
};


