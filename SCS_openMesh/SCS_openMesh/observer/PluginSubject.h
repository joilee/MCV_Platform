#pragma  once
#include "visualData/visualPluginItem.h"
#include <set>

class ComputePluginObserver;
class PluginSubject
{
public:
	PluginSubject();
	~PluginSubject();
	set<ComputePluginObserver*> observers;
	void attach(ComputePluginObserver* a);
	void detach(ComputePluginObserver*a);
	void notify();

	void clearItem();
	VisualPluginItem *item;
private:

};

