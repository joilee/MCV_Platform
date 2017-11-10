#include "visualAntennaItem.h"

visualAntennaItem::visualAntennaItem(){
	SitesID.clear();

}

visualAntennaItem::~visualAntennaItem()
{
	mp.clear();
	SitesID.clear();
}

void visualAntennaItem:: addSite(int a,string b)
{
	SitesID.insert(a); 
	map<int, SiteAntenna*>::iterator it = mp.find(a);
	if (it==mp.end())
	{
		SiteAntenna * tmp = new SiteAntenna(a);
		tmp->addAnte(b);
		mp.insert(make_pair(a, tmp));
	}
	else
	{
		it->second->addAnte(b);
	}
}


void SiteAntenna::addAnte(string a)
{
	ante.insert(a);
}
SiteAntenna::SiteAntenna(int a)
{
	id = a;
	ante.clear();
}