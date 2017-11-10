#pragma once
#include "../visualData/visualModelItem.h"
#include <string>
#include <set>
class modelObserver;

class modelSubject
{
public:
	explicit modelSubject();
	~modelSubject();
	set<modelObserver*> observers;
	void attach(modelObserver*);
	void detach(modelObserver*);
	void notify();
	//模型信息展示项
	visualModelItem *visItem;
private:

};

