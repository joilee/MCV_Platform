#pragma once
#include "visualData/LocalModelItem.h"
#include <string>
#include <set>
class LocalModelObserver;
class LocalModelSubject
{
public:
	LocalModelSubject();
	~LocalModelSubject();
	set<LocalModelObserver*> observers;
	void attach(LocalModelObserver*);
	void detach(LocalModelObserver*);
	void notify();
	LocalModelItem *visItem;
};

