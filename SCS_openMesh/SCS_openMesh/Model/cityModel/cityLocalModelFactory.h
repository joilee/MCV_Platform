#pragma once
#include "Model/abstractLocalModel/abstractLocalModel.h"
#include "Model/abstractLocalModel/abstractLocalModelFactory.h"
#include "cityLocalModel.h"
#include "cityModel.h"
using namespace std;

class cityLocalModelFactory
{
public:
	cityLocalModelFactory();
	~cityLocalModelFactory();

	/************************************************************************/
	/*
	@brief 调用局部场景的构造函数
	*/
	/************************************************************************/
	abstractLocalModel *loadModel(Vector3d center, int siteID, double range, cityModel * cModel);
protected:
private:
};