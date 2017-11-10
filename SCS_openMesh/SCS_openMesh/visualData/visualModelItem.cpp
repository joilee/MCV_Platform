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
	modelList[0] = "";
	modelList[1] = "";
}

visualModelItem::~visualModelItem()
{

}

void visualModelItem::clearCity()
{
	modelList[0] = "";
	buildingNum = 0;
	concaveBuilding = 0;
	cityMin = Vector3d(0, 0, 0);
	cityMax = Vector3d(0, 0, 0);
}

void visualModelItem::clearLocal()
{
	modelList[1] = "";
	LocalFaceNum = 0;
	localMin = Vector3d(0, 0, 0);
	localMax = Vector3d(0, 0, 0);
}