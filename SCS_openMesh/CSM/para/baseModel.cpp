#include "baseModel.h"

BaseModel::BaseModel()
{
	LocalScene_range = 0;
	m_Type = ModelType::UNDEFINED;
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
	if (m_Type==ModelType::CITY_LOCAL)
	{
		return localScene->getAltitude(x, y);
	}
	else 
	{
		
	}

}
