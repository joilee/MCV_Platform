#pragma once
#include "abstractLocalModel.h"
#include <string>
using namespace  std;

class abstractLocalModelFactory
{
public:
	abstractLocalModelFactory();
	virtual ~abstractLocalModelFactory();
	virtual abstractLocalModel * loadModel(std::string path) = 0;
private:

};

