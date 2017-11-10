#pragma once
#include <string>
#include "../visualData/visualModelItem.h"
using namespace std;

/*
@brief �۲���ģʽ����Ϊ��ģ��׼���Ĺ۲���ģʽ
*/
class Subject;
//����۲���
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