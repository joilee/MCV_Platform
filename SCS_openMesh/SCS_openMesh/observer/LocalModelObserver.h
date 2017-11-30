#pragma once
#include <string>
#include "../visualData/LocalModelItem.h"
using namespace std;

/*
@brief �۲���ģʽ����Ϊ��ģ��׼���Ĺ۲���ģʽ
*/

//����۲���
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