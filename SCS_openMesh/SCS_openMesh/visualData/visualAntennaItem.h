#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

//һ��С�������кܶ������
class SiteAntenna
{
public:
	SiteAntenna(int a);
	int id;
	void addAnte(string);
	inline void clearAnte(){ ante.clear(); }
	set<string> ante;
};

class visualAntennaItem
{
public:
	visualAntennaItem();
	~visualAntennaItem();

	/*
	@brief ���С�� �� ���ߵı��
	*/
	void addSite(int a,string b);

	inline bool findSite(int a){ 
		set<int>::iterator it;
		it = SitesID.find(a);
		return it == SitesID.end() ? false : true;
	}

	inline void deleteSite(int a)
	{
		SitesID.erase(a);
	}

	inline int  getSitesSize()
	{
		return SitesID.size();
	}

	inline int getSite(int a)
	{
		set<int>::iterator it = SitesID.begin();
		int count = 0;
		for (;it!=SitesID.end();it++)
		{
			if (count == a)return *it;
			count++;
		}
	}

	inline set<int> & getSites(){ return SitesID; }

	inline map<int, SiteAntenna*>& getMap(){ return mp; }
private :
	set<int> SitesID;
	map<int, SiteAntenna*> mp;
};