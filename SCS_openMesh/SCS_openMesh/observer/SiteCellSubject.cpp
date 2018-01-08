#include "SiteCellSubject.h"
#include "SiteCellObserver.h"
#include <iostream>
SiteCellSubject::SiteCellSubject()
{
	siteAndCellData = new Site_Item;
}

SiteCellSubject::~SiteCellSubject()
{
	delete siteAndCellData;
	observers.clear();
}

void SiteCellSubject::attach(SiteCellObserver* a)
{
	observers.insert(a);
	cout << "info: ��� ������ վ�� ���� �۲��߳ɹ� " << a->getName() << endl;
}

void SiteCellSubject::detach(SiteCellObserver*a)
{
	set<SiteCellObserver*>::iterator it = observers.begin();
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

void SiteCellSubject::notify()
{
	cout << "info: SiteCellSubject��ʼ������Ϣ" << endl;
	auto it = observers.begin();
	while (it!=observers.end())
	{
		(*it)->updateResult(siteAndCellData);
		it++;
	}
}

void SiteCellSubject::clearItem()
{
	if (siteAndCellData!=nullptr)
	{
		delete siteAndCellData;
	}
	siteAndCellData = nullptr;
}
