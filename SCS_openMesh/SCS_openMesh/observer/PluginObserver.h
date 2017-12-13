#pragma once
#include <string>
#include <QString>
#include "visualData/visualPluginItem.h"
using namespace std;
class ComputePluginObserver
{
public:
	ComputePluginObserver(QString a = "")
	{
		name = a;
	}
	virtual void updatePluginInfo(VisualPluginItem* a) = 0;
	QString getName(){ return name; }
private:
	QString name;
};

