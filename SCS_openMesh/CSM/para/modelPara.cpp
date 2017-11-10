#include "modelPara.h"
ModelPara::ModelPara()
{
	LocalScene_range = 0;
}

ModelPara::~ModelPara()
{

}

/************************************************************************/
/* �õ�һ�����Ӧ�ĸ߶�                                                                     */
/************************************************************************/
double  ModelPara::getPointAltitude(double x, double y)
{
	if (localScene==NULL)
	{
		return DBL_MIN;
	}
	return localScene->getAltitude(x, y);
}
