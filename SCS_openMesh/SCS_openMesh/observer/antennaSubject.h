#include "../visualData/visualAntennaItem.h"
#include <set>
class antennaObserver;

class antennaSubject
{
public:
	antennaSubject();
	~antennaSubject();

	set<antennaObserver*> observers;
	void attach(antennaObserver*);
	void detach(antennaObserver*);
	void notify();
	visualAntennaItem *AntennaItem;
private:

};

