#include "antennaSubject.h"
#include "antennaObserver.h"
#include <iostream>
antennaSubject::antennaSubject()
{
	AntennaItem = new visualAntennaItem;
}

antennaSubject::~antennaSubject()
{
	delete AntennaItem;
	observers.clear();
}

void antennaSubject::attach(antennaObserver*a)
{
	observers.insert(a);
	cout << "info: 添加天线观察者成功 " << a->getName()<< endl;
}

void antennaSubject::detach(antennaObserver*a)
{
	set<antennaObserver*>::iterator it = observers.begin();
	while (it != observers.end())
	{
		if ((*it) == a)
		{
			observers.erase(it);
			break;
		}
		++it;
	}
}

void antennaSubject::notify()
{
	cout << "info: antennaSubject开始发送消息" << endl;
	set<antennaObserver*>::iterator it = observers.begin();
	while (it != observers.end())
	{
		(*it)->update(this->AntennaItem);
		it++;
	}
}