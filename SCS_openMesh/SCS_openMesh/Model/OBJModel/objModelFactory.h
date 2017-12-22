#pragma once
#include "Model/abstractGlobalModel/abstractGlobalModelFactory.h"
#include "objModel.h"
using namespace std;

class objModelFactory
{
public:
	objModelFactory();
	~objModelFactory();

	/************************************************************************/
	/*
	@brief 调用城市场景的构造函数
	*/
	/************************************************************************/
	abstractGlobalModel *loadModel(string path);
protected:
private:
};