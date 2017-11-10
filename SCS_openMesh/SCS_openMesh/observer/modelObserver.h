#pragma once
#include <string>
#include "../visualData/visualModelItem.h"
using namespace std;

/*
@brief 观察者模式——为了模型准备的观察者模式
*/
class Subject;
//抽象观察者
class modelObserver
{
public:
	modelObserver(string name)
	{
		this->name = name;
	}
	string getName(){ return name; }
	string name;
	virtual void update(visualModelItem*a) = 0;
};