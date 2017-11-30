#pragma once
#include <string>
#include "../visualData/LocalModelItem.h"
using namespace std;

/*
@brief 观察者模式——为了模型准备的观察者模式
*/

//抽象观察者
class LocalModelObserver
{
public:
	LocalModelObserver(string name)
	{
		this->name = name;
	}
	string getName(){ return name; }
	string name;
	virtual void updateLocal(LocalModelItem*a) = 0;
};