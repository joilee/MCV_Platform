#pragma once
#include <util/vector.h>
#include <vector>

/*
�۲���ģʽ
���͵Ĳ�������װ����visualModelItem
����
1.����ģ�͵��б�����+�ֲ������б��������+id
2.���г���������
*/

class visualModelItem
{
public:
	visualModelItem();
	~visualModelItem();

	bool needUpdate(){ return updateData; }
	void setFlag(bool a){ updateData = a; }

	inline int getBuildingNum(){ return buildingNum; }
	inline int getConcaveNum(){ return concaveBuilding; }
	inline Vector3d getcityMin(){ return cityMin; }
	inline Vector3d getcityMax(){ return cityMax; }

	inline vector<string> getName(){ return modelName; }
	inline vector<int> getID(){ return modelID; }

	void setBuildingNum(int a){ buildingNum=a; }
	void setConcaveNum(int a){  concaveBuilding=a; }
	 void setcityMin(Vector3d a){  cityMin=a; }
	 void setcityMax(Vector3d a){ cityMax=a; }

	 void setModelID(vector<int> a){ modelID = a; }
	 void setName(vector<string> a){ modelName = a; }
	


	 //���ӳ�������
	 void addCity(int BuildNum,int ConcaveNum,Vector3d bboxMin,Vector3d bboxMax);

	 void clearCity();

private:
	int buildingNum;
	int concaveBuilding;
	Vector3d cityMin;
	Vector3d cityMax;
	vector<int> modelID;
	vector<string> modelName;
	bool updateData;



};

