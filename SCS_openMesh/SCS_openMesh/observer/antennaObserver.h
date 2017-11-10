#pragma once
#include <string>
#include "../visualData/visualAntennaItem.h"
using namespace std;
class antennaObserver
{
public:
	antennaObserver(string name=NULL)
	{
		this->name = name;
	}
	string getName(){ return name; }
	virtual void update(visualAntennaItem*a) = 0;

private:	
	string name;
};