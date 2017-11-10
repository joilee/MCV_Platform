#include "modelSubject.h"
#include "../observer/modelObserver.h"

void modelSubject::attach(modelObserver*a)
{
	observers.insert(a);
	cout << "info: 添加模型观察者成功 "<<a->name << endl;
}
void modelSubject::detach(modelObserver*a)
{
	set<modelObserver*>::iterator it=observers.begin();
	while (it!=observers.end())
	{
		if ((*it)==a)
		{
			observers.erase(it);
			break;
		}
		++it;
	}
}

void modelSubject::notify()
{
	cout << "info: modelSubject开始发送消息" << endl;
	set<modelObserver*>::iterator it = observers.begin();
	while (it != observers.end())
	{
		(*it)->update(this->visItem);
		it++;
	}
}

modelSubject::modelSubject()
{
	visItem = new visualModelItem;
}

modelSubject::~modelSubject()
{

}