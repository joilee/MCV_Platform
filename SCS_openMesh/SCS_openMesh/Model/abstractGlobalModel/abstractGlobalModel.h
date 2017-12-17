#pragma once

#include "Model/abstractModel/abstractModel.h"
#include <util/vector.h>
class abstractGlobalModel:public abstractModel
{
public:
	abstractGlobalModel();
	virtual ~abstractGlobalModel();

	virtual double getAltitude(double x, double y)=0;
	virtual Vector3d getMinPoint()=0;
	virtual Vector3d getMaxPoint()=0;
	virtual int getBuildingSize()=0;
	virtual int getConcaveSize()=0;
private:

};
