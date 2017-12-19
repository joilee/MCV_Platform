#pragma  once
#include <para/visualPara.h>
#define NO_SITE_PCI -1
/*
仿真结果容器
按照 site -> cell(pci)->cell数据，共3个维度

实现一下功能
（1） 按照id获取site的数据
（2） 按照pci值获取对应cell测量值（仿真值）
（3） 按照id值删除site的数据
（4）按照pci值删除对应的cell值。
*/

class EFieldContainer
{
public:
	EFieldContainer();
	~EFieldContainer();

	//仿真数据
	bool isDataExist();
	bool isSiteExist(int id);
	bool isPCIExist(int pciValue);
	Site_Data* getSiteDataByID(int id);
	Cell_Data* getCellDataByPCI(int pciValue);
	void deleteSiteDataByID(int id);
	void deleteCellDataByID(int pciValue);
	void deleteAllSimuData();
	int getSiteSize();
	int getCellSizeBySiteID(int id);
	void addSiteData(Site_Data* a);
	void addNewSite(int siteID);
	void addCellData(Cell_Data* a, int siteID);

	vector<int> getAllSiteID();
	
	vector<int> getPCIsBySiteID(int siteID);

	int getSiteIDByPCI(int pci);
	map<int, Site_Data*> getSimData(){ return eFieldData; }

	//实测数据
	void addPCI_Mearsured(Cell_Data *a);
	void clearMeasuredData();
	void addEfield_Measured(EField*a,int pci);
	map<int, Site_Data*> getMeasuredData(){ return measuredData; }
private:
	map<int, Site_Data*> eFieldData;//仿真数据
	
	map<int, Site_Data*> measuredData;
};

