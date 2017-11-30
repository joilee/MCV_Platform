#pragma  once
#include <para/computePara.h>

/************************************************************************/
/*
天线文件容器类                                                                  


*/
/************************************************************************/
class SitesContainer
{
public:
	SitesContainer();
	~SitesContainer();

	void addSite(Site *s);

	void addAntenna(TransAntenna t, int siteName);

	bool isSiteExist(int id);

	bool isSiteAndAntennaExist(int SiteID, int AntennaID);

	void clear();

	void setInitialGain(int SiteID,int AntennasID, double value);

	void addDirectionGain(int SiteID, int AntennasID, vector<double>value);

	int getSitesSize();

	int getAntennasSize(int id);


	/**
	  * @Method:    getSiteIDByOrder
	  * @Note: 	获得第num个site的名字,num从0开始
	  * @Author:    Li Gen
	  * @ Date:     2017/11/27
	  * @Returns:   int
	  * @Parameter: int num
	*/
	int getSiteIDByOrder(int num);

	vector<int> getIDs();

	Site* getSiteByID(int id);

	vector<Site*> getAllSites();

	Vector3d getSitePositionById(int id);

	vector<Vector3d> getSitesPosition();


private:
	std::map<int, Site*> Sites;  //多个基站
};


