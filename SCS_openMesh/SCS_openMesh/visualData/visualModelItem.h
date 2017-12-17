#pragma once
#include <util/vector.h>
#include <vector>

/*
观察者模式
推送的参数被封装成了visualModelItem
包括
1.生成模型的列表（城市+局部），列表就是名字+id
2.城市场景的数据
*/

class visualModelItem
{
public:
	visualModelItem();
	~visualModelItem();

	bool cityNeedUpdate(){ return updateCityData; }
	bool localNeedUpdate(){ return updateLocalData; }
	void setGlobalFlag(bool a){ updateCityData = a; }
	void setLocalFlag(bool a){ updateLocalData = a; }

	inline int getBuildingNum(){ return buildingNum; }
	inline int getConcaveNum(){ return concaveBuilding; }
	inline Vector3d getcityMin(){ return cityMin; }
	inline Vector3d getcityMax(){ return cityMax; }

	inline vector<string> getGlobalName(){ return globalModelName; }
	inline vector<int> getGlobalID(){ return globalModelID; }

	inline vector<string> getLocalName(){ return localModelName; }
	inline vector<int> getLocalModelID(){ return localModelID; }

	void setBuildingNum(int a){ buildingNum=a; }
	void setConcaveNum(int a){  concaveBuilding=a; }
	 void setcityMin(Vector3d a){  cityMin=a; }
	 void setcityMax(Vector3d a){ cityMax=a; }

	 void setGlobalModelID(vector<int> a){ globalModelID = a; }
	 void setGlobalName(vector<string> a){ globalModelName = a; }
	
	 void setLocalModelID(vector<int> a){ localModelID = a; }
	 void setLocalModelName(vector<string>a){ localModelName = a; }


	 //增加城市数据
	 void addCity(int BuildNum,int ConcaveNum,Vector3d bboxMin,Vector3d bboxMax);

	 void clearCity();

private:
	int buildingNum;
	int concaveBuilding;
	Vector3d cityMin;
	Vector3d cityMax;
	vector<int> globalModelID;
	vector<string> globalModelName;
	vector<int> localModelID;
	vector<string> localModelName;
	bool updateCityData;
	bool updateLocalData;


};

