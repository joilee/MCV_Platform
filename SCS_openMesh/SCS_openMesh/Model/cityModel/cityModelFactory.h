#pragma  once

#include "../abstractModelFactory.h"
#include "cityModel.h"
#include "cityLocalModel.h"
#include <string>

using namespace std;

class cityModelFactory :public abstractModelFactory
{
public:
	cityModelFactory();
	~cityModelFactory();

	/*
	@brief 调用城市场景的构造函数 
	*/
	abstractModel *loadModel(string path);

	/************************************************************************/
	/* 
	@brief 调用局部场景的构造函数
	*/
	/************************************************************************/
	abstractModel *loadModel(Vector3d center, double range, cityModel * cModel);
private:
};

