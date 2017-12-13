#pragma  once
#include <para/visualPara.h>

/*
����������
���� site -> cell(pci)->cell���ݣ���3��ά��

ʵ��һ�¹���
��1�� ����id��ȡsite������
��2�� ����pciֵ��ȡ��Ӧcell����ֵ������ֵ��
��3�� ����idֵɾ��site������
��4������pciֵɾ����Ӧ��cellֵ��
*/

class EFieldContainer
{
public:
	EFieldContainer();
	~EFieldContainer();

	//��������
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

	//ʵ������
	void setMeasuredData(Cell_Data *a);

private:
	map<int, Site_Data*> eFieldData;//��������
	
	Cell_Data *  mesuredData;
};

