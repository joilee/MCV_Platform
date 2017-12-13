#pragma  once
#include "../visualData/visualResultItem.h"
#include <set>
class SiteCellObserver;

class SiteCellSubject
{
public:
	SiteCellSubject();
	~SiteCellSubject();
	set<SiteCellObserver*> observers;
	void attach(SiteCellObserver* a);
	void detach(SiteCellObserver*a);
	void notify();

	void clearItem();
	Site_Item * siteAndCellData;
private:

};

