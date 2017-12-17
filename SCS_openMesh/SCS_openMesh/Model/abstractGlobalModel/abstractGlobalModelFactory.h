#pragma once
#include "abstractGlobalModel.h"

class abstractGlobalModelFactory
{
public:
	abstractGlobalModelFactory(){};
	virtual ~abstractGlobalModelFactory(){};
	virtual abstractGlobalModel * loadModel(std::string path) = 0;
private:

};

