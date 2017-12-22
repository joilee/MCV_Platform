#include "EFieldContainer.h"

EFieldContainer::EFieldContainer()
{
	eFieldData.clear();
	measuredData.clear();
}

EFieldContainer::~EFieldContainer()
{
	deleteAllSimuData();
	clearMeasuredData();
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
	return res;
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
			return NO_SITE_PCI;
		}
	}
	else
	{
		return NO_SITE_PCI;
	}
}

void EFieldContainer::addPCI_Mearsured(Cell_Data *a)
{
	int siteID = getSiteIDByPCI(a->pci);
	if (siteID==NO_SITE_PCI)
	{

	}
	else
	{
		auto it = measuredData.find(siteID);
		if (it == measuredData.end())
		{
			Site_Data *tmp = new Site_Data(siteID);
			measuredData.insert(make_pair(siteID, tmp));
		}
		it = measuredData.find(siteID);
		it->second->cellsMap.insert(make_pair(a->pci, a));
	}
	return;
}

void EFieldContainer::clearMeasuredData()
{
	auto it = measuredData.begin();
	for (; it != measuredData.end(); it++)
	{
		delete it->second;
	}
	map<int, Site_Data*> tmp;
	measuredData.swap(tmp);
}

void EFieldContainer::addEfield_Measured(EField*a, int pci)
{
	if (!isPCIExist(pci))
	{
		return;
	}
	int siteID = getSiteIDByPCI(pci);
	if (siteID == NO_SITE_PCI)
	{

	}
	else
	{
		auto it = measuredData.find(siteID);
		if (it == measuredData.end())
		{
			//新建site
			Site_Data *tmp = new Site_Data(siteID);
			measuredData.insert(make_pair(siteID, tmp));
		}
		it = measuredData.find(siteID);
		auto it2 = it->second->cellsMap.find(pci);
		if (it2==it->second->cellsMap.end())
		{
			//新建cell
			Cell_Data * tmpCell = new Cell_Data(pci);
			it->second->cellsMap.insert(make_pair(pci, tmpCell));
		}
		auto it3 = it->second->cellsMap.find(pci);
		it3->second->efildVec.push_back(a);
	}
	return;
}

