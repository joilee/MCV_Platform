#pragma  once
#include <map>
#include "../../Model/abstractModel.h"
#include "../../Model/abstractModelFactory.h"
#include  "../../Model/cityModel/cityModelFactory.h"
#include <para/modelPara.h>
#define NO_LOCAL_MODEL_ID -1111

//负责模型的增删查
class ModelContainer
{
public:
	ModelContainer();
	~ModelContainer();

public:
	void  insertModel(abstractModel* model);
	void  deleteModel(abstractModel* model);
	void  deleteModel(string name);
	void  deleteModel(int id);
	bool  isLocalExist();
	int getModelSize(){ return modelMap.size(); }

	cityLocalModel * getFirstLocal();
	cityLocalModel * getLocalModelByID(int id);
	bool isCityExist();
	bool isLocalIDExist(int id);
	cityModel* getFirstCity();

	//返回所有id
	vector<int> getIDs();
	vector<int> getLocalCityModelID();
	//返回所有名称
	vector<string> getNames();

	vector<Vector3d> getCityBoundingBox();
	int getCityBuildingNum();
	int getCityConcaveNum();
	int getLocalFaceNum(int id=NO_LOCAL_MODEL_ID);
	vector<Vector3d> getLocalBountdingBox(int id = NO_LOCAL_MODEL_ID);
private:
	map<int, abstractModel*> modelMap;
};


