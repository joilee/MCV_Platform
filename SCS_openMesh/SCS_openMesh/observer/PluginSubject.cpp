#include "PluginSubject.h"
#include <QDebug>
#include "PluginObserver.h"
#include <iostream>
using namespace std;

PluginSubject::PluginSubject()
{
	item = new VisualPluginItem;
}

PluginSubject::~PluginSubject()
{
	delete item;
	observers.clear();
}

void PluginSubject::attach(ComputePluginObserver* a)
{
	observers.insert(a);
	cout<< "info: 添加 插件 观察者成功 "<<a->getName().toStdString() << endl;
}

void PluginSubject::detach(ComputePluginObserver*a)
{
	set<ComputePluginObserver*>::iterator it = observers.begin();
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

void PluginSubject::notify()
{
	cout<< "info: PluginSubject开始发送消息"<< endl;
	auto it = observers.begin();
	while (it != observers.end())
	{
		(*it)->updatePluginInfo(item);
		it++;
	}
}

void PluginSubject::clearItem()
{
	if (item!=nullptr)
	{
		delete item;
	}
}
