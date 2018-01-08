#pragma once
#include <string>
#include "./visualData/visualResultItem.h"

using namespace std;

class SiteCellObserver
{
public:
	SiteCellObserver(string a = NULL)
	{
		name = a;
	}
	string getName(){ return name; }
	virtual void updateResult(Site_Item*a) = 0;
private:
	string name;
};