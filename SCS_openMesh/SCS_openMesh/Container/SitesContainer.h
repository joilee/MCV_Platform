#pragma  once
#include <para/computePara.h>

/************************************************************************/
/*
�����ļ�������                                                                  


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
	  * @Note: 	��õ�num��site������,num��0��ʼ
	  * @Author:    Li Gen
	  * @ Date:     2017/11/27
	  * @Returns:   int
	  * @Parameter: int num
	*/
	int getSiteIDByOrder(int num);

	/**
	  * @Method:    getIDs
	  * @Note: 	��ȡ����վ���id
	  * @Author:    Li Gen
	  * @ Date:     2017/12/13
	  * @Returns:   std::vector<int>
	*/
	vector<int> getIDs();

	Site* getSiteByID(int id);

	vector<Site*> getAllSites();

	Vector3d getSitePositionById(int id);

	/**
	  * @Method:    getSitesPosition
	  * @Note: 	��ȡ����վ���λ��
	  * @Author:    Li Gen
	  * @ Date:     2017/12/13
	  * @Returns:   std::vector<Vector3d>
	*/
	vector<Vector3d> getSitesPosition();

	map<int, Site*> getSitesMap();
private:
	std::map<int, Site*> Sites;  //�����վ
};


