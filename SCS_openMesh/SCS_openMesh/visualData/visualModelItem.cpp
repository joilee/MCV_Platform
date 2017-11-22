#include "visualModelItem.h"

visualModelItem::visualModelItem()
{
	buildingNum = 0;
	concaveBuilding = 0;
	cityMin = Vector3d(0, 0, 0);
	cityMax = Vector3d(0, 0, 0);
	LocalFaceNum = 0;
	localMin = Vector3d(0, 0, 0);
	localMax = Vector3d(0, 0, 0);
	modelList.resize(2);
	modelList[0] = "";//城市场景为空
	modelList[1] = "";//
}

visualModelItem::~visualModelItem()
{

}

bool visualModelItem::isCityEmpty()
{
	return modelFlag[0];
}

bool visualModelItem::isLocalSceneEmpty()
{
	return modelFlag[1];
}

void visualModelItem::addCity(int BuildNum, int ConcaveNum, Vector3d bboxMin, Vector3d bboxMax,string name)
{
	setBuildingNum(BuildNum);
	setConcaveNum(ConcaveNum);
	setcityMin(bboxMin);
	setcityMax(bboxMax);
	modelFlag[0] = true;
	modelList[0] = name;
}

void visualModelItem::addLocalScene(int faceNum, Vector3d minBox, Vector3d maxBox,string name)
{
	setLocalFaceNum(faceNum);
	setlocalMin(minBox);
	setlocalMax(maxBox);
	modelFlag[1] = true;
	modelList[1] = name;
}

void visualModelItem::clearCity()
{
	modelList[0] = "";
	modelFlag[1] = false;
	buildingNum = 0;
	concaveBuilding = 0;
	cityMin = Vector3d(0, 0, 0);
	cityMax = Vector3d(0, 0, 0);
}

void visualModelItem::clearLocal()
{
	modelList[1] = "";
	modelFlag[1] = false;
	LocalFaceNum = 0;
	localMin = Vector3d(0, 0, 0);
	localMax = Vector3d(0, 0, 0);
}