#pragma once
#include "Model/abstractLocalModel/abstractLocalModelFactory.h"
#include "objLocalModel.h"
using namespace std;

class objLocalModelFactory
{
public:
	objLocalModelFactory();
	~objLocalModelFactory();

	/************************************************************************/
	/*
	@brief 调用城市场景的构造函数
	*/
	/************************************************************************/
	abstractLocalModel *loadModel(Vector3d center, double localRange, string _name, objModel* cModel);
protected:
private:
};