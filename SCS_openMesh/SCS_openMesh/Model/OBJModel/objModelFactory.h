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
	@brief ���ó��г����Ĺ��캯��
	*/
	/************************************************************************/
	abstractGlobalModel *loadModel(string path);
protected:
private:
};