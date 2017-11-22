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
	@brief ����������Ԫ��
	@since 5/11/2017
	@note  ��ǰֻ֧��һ��ȫ�ֺ�һ���ֲ������ȫ�ֺͶ���ֲ�����ȴ���һ���Ľ�
	*/
	/************************************************************************/
	vector<string> modelList;
	vector<bool> modelFlag;
	inline int getBuildingNum(){ return buildingNum; }
	inline int getConcaveNum(){ return concaveBuilding; }
	inline Vector3d getcityMin(){ return cityMin; }
	inline Vector3d getcityMax(){ return cityMax; }
	inline int getLocalFaceNum(){ return LocalFaceNum; }
	inline Vector3d getlocalMin(){ return localMin; }
	inline Vector3d getlocalMax(){ return localMax; }
	inline vector<string> getName(){ return modelList; }
	inline vector<bool> getFlag(){ return modelFlag; }
	void setBuildingNum(int a){ buildingNum=a; }
	void setConcaveNum(int a){  concaveBuilding=a; }
	 void setcityMin(Vector3d a){  cityMin=a; }
	 void setcityMax(Vector3d a){ cityMax=a; }
	 void setLocalFaceNum(int a){  LocalFaceNum=a; }
	 void setlocalMin(Vector3d a){  localMin=a; }
	 void setlocalMax(Vector3d a){ localMax=a; }

	 //�ж��Ƿ�Ϊ��
	 bool isCityEmpty();
	 bool isLocalSceneEmpty();

	 //���ӳ�������
	 void addCity(int BuildNum,int ConcaveNum,Vector3d bboxMin,Vector3d bboxMax,string name);
	 //���Ӿֲ���������
	 void addLocalScene(int faceNum,Vector3d minBox,Vector3d maxBox,string name);

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

