#pragma  once

#include "Model/abstractGlobalModel/abstractGlobalModelFactory.h"
#include "cityModel.h"

#include <string>

using namespace std;

class cityModelFactory :public abstractGlobalModelFactory
{
public:
	cityModelFactory();
	~cityModelFactory();

	/*
	@brief 调用城市场景的构造函数 
	*/
	abstractGlobalModel *loadModel(string path);
	abstractGlobalModel *loadModel(string name, vector<string> bPath, vector<string> hPath, string aPath);

private:
};

