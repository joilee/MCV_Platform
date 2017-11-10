#pragma once
#include <util/vector.h>
#include <vector>
class visualModelItem
{
public:
	visualModelItem();
	~visualModelItem();
	/************************************************************************/
	/*
	@brief 数组有两个元素，名字若为“”，则表示不存在。
	@since 5/11/2017
	@note  当前只支持一个全局和一个局部，多个全局和多个局部对象等待下一步改进
	*/
	/************************************************************************/
	vector<string> modelList;

	inline int getBuildingNum(){ return buildingNum; }
	inline int getConcaveNum(){ return concaveBuilding; }
	inline Vector3d getcityMin(){ return cityMin; }
	inline Vector3d getcityMax(){ return cityMax; }
	inline int getLocalFaceNum(){ return LocalFaceNum; }
	inline Vector3d getlocalMin(){ return localMin; }
	inline Vector3d getlocalMax(){ return localMax; }
	inline vector<string> getFlag(){ return modelList; }
	void setBuildingNum(int a){ buildingNum=a; }
	void setConcaveNum(int a){  concaveBuilding=a; }
	 void setcityMin(Vector3d a){  cityMin=a; }
	 void setcityMax(Vector3d a){ cityMax=a; }
	 void setLocalFaceNum(int a){  LocalFaceNum=a; }
	 void setlocalMin(Vector3d a){  localMin=a; }
	 void setlocalMax(Vector3d a){ localMax=a; }

	 void clearCity();
	 void clearLocal();
private:
	int buildingNum;
	int concaveBuilding;
	Vector3d cityMin;
	Vector3d cityMax;
	int LocalFaceNum;
	Vector3d localMin;
	Vector3d localMax;



};

