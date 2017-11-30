#include "LocalModelSubject.h"
#include "observer/LocalModelObserver.h"

LocalModelSubject::LocalModelSubject()
{
}


LocalModelSubject::~LocalModelSubject()
{
}

void LocalModelSubject::attach(LocalModelObserver* a)
{
	observers.insert(a);
	cout << "info: ��Ӿֲ�ģ�͹۲��߳ɹ� " << a->getName()<< endl;
}

void LocalModelSubject::detach(LocalModelObserver* a)
{
	set<LocalModelObserver*>::iterator it = observers.find(a);
	if (it!=observers.end())
	{
		observers.erase(it);
	}
	return;
}

void LocalModelSubject::notify()
{
	cout << "LocalModelSubject��ʼ������Ϣ" << endl;
	set<LocalModelObserver*>::iterator it = observers.begin();
	while (it != observers.end())
	{
		(*it)->updateLocal(this->visItem);
		it++;
	}
}
