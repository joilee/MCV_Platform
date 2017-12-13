#include "SitesContainer.h"
#include <QMessageBox>
SitesContainer::SitesContainer()
{
	Sites.clear();
}

SitesContainer::~SitesContainer()
{
	Sites.clear();
}

void SitesContainer::addSite(Site *s)
{
	int id = s->Site_Name;
	if (isSiteExist(id))
	{
		QMessageBox::warning(NULL, "Error", QStringLiteral("站点id重复，请检查站点文件"));
	}
	Sites.insert(make_pair(id, s));
}

void SitesContainer::addAntenna(TransAntenna t,int siteName)
{
	if (! isSiteExist(siteName))
	{
		Site *new_site=new Site;
		new_site->Site_Name = siteName;
		addSite(new_site);
	}
	Sites[siteName]->Site_Antennas.push_back(t);
}

bool SitesContainer::isSiteExist(int id)
{
	map<int, Site*>::iterator it = Sites.find(id);
	if (it==Sites.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool SitesContainer::isSiteAndAntennaExist(int SiteID, int AntennaID)
{
	if (!isSiteExist(SiteID))
	{
		return false;
	}
	map<int, Site*>::iterator it = Sites.find(SiteID);
	int len=it->second->Site_Antennas.size();
	if (AntennaID>=0&&AntennaID<len)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SitesContainer::clear()
{
	Sites.erase(Sites.begin(), Sites.end());
}

/**
  * @Method:    setInitialGain
  * @Note: 	设置初始增益
  * @Author:    Li Gen
  * @ Date:     2017/11/27
  * @Returns:   void
  * @Parameter: int SiteID
  * @Parameter: int AntennasID
*/
void SitesContainer::setInitialGain(int SiteID, int AntennasID, double value)
{
	if (!isSiteAndAntennaExist(SiteID,AntennasID))
	{
		//如果不存在
		QMessageBox::warning(NULL, "Error", QStringLiteral("站点id或者天线id错误，请检查文件"));
		return;
	}
	map<int, Site*>::iterator it = Sites.find(SiteID);
	it->second->Site_Antennas[AntennasID].initial_Gain = value;

}

void SitesContainer::addDirectionGain(int SiteID, int AntennasID, vector<double>value)
{
	if (!isSiteAndAntennaExist(SiteID, AntennasID))
	{
		//如果不存在
		QMessageBox::warning(NULL, "Error", QStringLiteral("站点id或者天线id错误，请检查文件"));
		return;
	}
	map<int, Site*>::iterator it = Sites.find(SiteID);
	it->second->Site_Antennas[AntennasID].direction_Gain.push_back(value);
}

int SitesContainer::getSitesSize()
{
	return Sites.size();
}

int SitesContainer::getAntennasSize(int id)
{
	if (isSiteExist(id))
	{
		return Sites[id]->Site_Antennas.size();
	}
	return -1;
}

int SitesContainer::getSiteIDByOrder(int num)
{
	if (num>=getSitesSize())
	{
		return -1;
	}
	map<int, Site*>::iterator it = Sites.begin();
	int i = 0;
	for (; i < num&&it!=Sites.end();i++)
	{
		it++;
	}
	return it->first;
}

std::vector<int> SitesContainer::getIDs()
{
	vector<int> res;
	map<int, Site*>::iterator it = Sites.begin();
	for (; it != Sites.end();it++)
	{
		res.push_back(it->first);
	}
	return res;
}

Site* SitesContainer::getSiteByID(int id)
{
	if (isSiteExist(id))
	{
		return Sites[id];
	}
	return nullptr;
}

Vector3d SitesContainer::getSitePositionById(int id)
{
	map<int, Site*>::iterator it = Sites.find(id);
	if (it==Sites.end())
	{
		QMessageBox::critical(NULL, QStringLiteral("错误"), QStringLiteral("获取Site位置出错，数组下标超出范围！"));
		return Vector3d(0, 0, 0);
	}
	Vector3d res = it->second->Site_Antennas[0].position;
	return res;
}

std::vector<Site*> SitesContainer::getAllSites()
{
	vector<Site*>res;
	map<int, Site*>::iterator it = Sites.begin();
	for (; it != Sites.end();it++)
	{
		res.push_back(it->second);
	}
	return res;
}

std::vector<Vector3d> SitesContainer::getSitesPosition()
{
	vector<Vector3d> res;
	map<int, Site*>::iterator it = Sites.begin();
	for (; it != Sites.end(); it++)
	{
		res.push_back(it->second->Site_Antennas[0].position);
	}
	return res;
}

std::map<int, Site*> SitesContainer::getSitesMap()
{
	return Sites;
}

