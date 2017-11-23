#include "baseModel.h"

BaseModel::BaseModel()
{
	LocalScene_range = 0;
}

BaseModel::~BaseModel()
{
	V.clear();
	F.clear();
	NF.clear();
	f_materialId.clear();
	AP_Edge_list.clear();
}

double BaseModel::getPointAltitude(double x, double y)
{
	if (localScene == NULL)
	{
		return DBL_MIN;
	}
	return localScene->getAltitude(x, y);
}
