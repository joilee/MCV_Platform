#include "modelPara.h"
ModelPara::ModelPara()
{
	LocalScene_range = 0;
}

ModelPara::~ModelPara()
{

}

/************************************************************************/
/* 得到一个点对应的高度                                                                     */
/************************************************************************/
double  ModelPara::getPointAltitude(double x, double y)
{
	if (localScene==NULL)
	{
		return DBL_MIN;
	}
	return localScene->getAltitude(x, y);
}
