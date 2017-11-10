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
	//ģ����Ϣչʾ��
	visualModelItem *visItem;
private:

};

