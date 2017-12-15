#pragma once

#include "abstractModel.h"

class abstractGlobalModel
{
public:
	abstractGlobalModel();
	~abstractGlobalModel();
	void virtual draw(vector<bool> mode);
	void virtual initDraw();
	virtual double getAltitude(double x, double y);
private:

};
