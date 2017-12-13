#include "EFieldContainer.h"

EFieldContainer::EFieldContainer()
{
	eFieldData.clear();
	mesuredData = nullptr;
}

EFieldContainer::~EFieldContainer()
{
	eFieldData.clear();
	if (mesuredData!=nullptr)
	{
		delete mesuredData;
	}
}

bool EFieldContainer::isDataExist()
{
	if (eFieldData.size()==0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool EFieldContainer::isSiteExist(int id)
{
	auto it = eFieldData.find(id);
	if (it!=eFieldData.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EFieldContainer::isPCIExist(int pciValue)
{
	auto it = eFieldData.begin();
	bool flag = false;
	for (; it != eFieldData.end();it++)
	{
		auto it2 = it->second->cellsMap.find(pciValue);
		if (it2 != it->second->cellsMap.end())
		{
			flag = true;
			return flag;
		}
	}
	return flag;
}

Site_Data* EFieldContainer::getSiteDataByID(int id)
{
	if (isSiteExist(id))
	{
		return eFieldData[id];
	}
	else
	{
		return nullptr;
	}
}

Cell_Data* EFieldContainer::getCellDataByPCI(int pciValue)
{
	auto it = eFieldData.begin();
	bool flag = false;
	for (; it != eFieldData.end(); it++)
	{
		auto it2 = it->second->cellsMap.find(pciValue);
		if (it2 != it->second->cellsMap.end())
		{
			flag = true;
			return it2->second;
		}
	}
	if (flag==false)
	{
		return nullptr;
	}
}

void EFieldContainer::deleteSiteDataByID(int id)
{
	if (isSiteExist(id))
	{
		eFieldData.erase(id);
	}
	return;
}

void EFieldContainer::deleteCellDataByID(int pciValue)
{
	auto it = eFieldData.begin();
	for (; it != eFieldData.end(); it++)
	{
		auto it2 = it->second->cellsMap.find(pciValue);
		if (it2 != it->second->cellsMap.end())//find success
		{
			it->second->cellsMap.erase(it2);//delete
		}
	}

}

void EFieldContainer::deleteAllSimuData()
{
	auto it = eFieldData.begin();
	for (; it != eFieldData.end(); it++)
	{
		delete it->second;
	}
	map<int, Site_Data*> tmp;
	eFieldData.swap(tmp);
}

int EFieldContainer::getSiteSize()
{
	return eFieldData.size();
}

int EFieldContainer::getCellSizeBySiteID(int id)
{
	if (isSiteExist(id))
	{
		return eFieldData[id]->cellsMap.size();
	}
	return 0;
}

void EFieldContainer::addSiteData(Site_Data* a)
{
	if (isSiteExist(a->siteID))
	{
		cout << "可视化数据 仿真站点 站点id已经存在" << endl;
		return;
	}
	eFieldData.insert(make_pair(a->siteID, a));
}

void EFieldContainer::addNewSite(int siteID)
{
	if (isSiteExist(siteID))
	{
		cout << "可视化数据 仿真站点 站点id已经存在" << endl;
		return;
	}
	Site_Data *a = new Site_Data(siteID);
	eFieldData.insert(make_pair(siteID, a));
}

void EFieldContainer::addCellData(Cell_Data* a, int siteID)
{
	if (isPCIExist(a->pci))
	{
		cout << "可视化数据 仿真站点 cell  PCI已经存在" << endl;
		return;
	}
	if (!isSiteExist(siteID))
	{
		addNewSite(siteID);
		eFieldData[siteID]->cellsMap.insert(make_pair(a->pci, a));
	}

}

std::vector<int> EFieldContainer::getAllSiteID()
{
	vector<int> res;
	auto it = eFieldData.begin();
	while (it!=eFieldData.end())
	{
		res.push_back(it->first);
	}
}

std::vector<int> EFieldContainer::getPCIsBySiteID(int siteID)
{
	if (isSiteExist(siteID))
	{
		vector<int> res;
		Site_Data * sd = getSiteDataByID(siteID);
		auto it = sd->cellsMap.begin();
		while (it!=sd->cellsMap.end())
		{
			res.push_back(it->first);
			it++;
		}
		return res;
	}
	else
	{
		return vector<int>();
	}
}

int EFieldContainer::getSiteIDByPCI(int pci)
{
	if (isPCIExist(pci))
	{
		auto it = eFieldData.begin();
		bool flag = false;
		for (; it != eFieldData.end(); it++)
		{
			auto it2 = it->second->cellsMap.find(pci);
			if (it2 != it->second->cellsMap.end())
			{
				flag = true;
				return it->first;
			}
		}
		if (flag == false)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

void EFieldContainer::setMeasuredData(Cell_Data *a)
{
	mesuredData = a;
}

