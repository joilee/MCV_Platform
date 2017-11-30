#include "visualModelItem.h"

visualModelItem::visualModelItem()
{
	buildingNum = 0;
	concaveBuilding = 0;
	cityMin = Vector3d(0, 0, 0);
	cityMax = Vector3d(0, 0, 0);

	updateData= false;

}

visualModelItem::~visualModelItem()
{

}

void visualModelItem::addCity(int BuildNum, int ConcaveNum, Vector3d bboxMin, Vector3d bboxMax)
{
	setBuildingNum(BuildNum);
	setConcaveNum(ConcaveNum);
	setcityMin(bboxMin);
	setcityMax(bboxMax);
}



void visualModelItem::clearCity()
{
	buildingNum = 0;
	concaveBuilding = 0;
	cityMin = Vector3d(0, 0, 0);
	cityMax = Vector3d(0, 0, 0);
}

