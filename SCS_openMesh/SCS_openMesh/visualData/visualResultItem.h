#pragma once
#include<map>
#include <string>
#include <vector>
using namespace std;

//单个cell
class Cell_Item
{
public:
	int id;
	string name;
	Cell_Item(int a, string b)
	{
		id = a;
		name = b;
	}
};

//一个site下面的多个cell
class Cell_Item_Vector
{
public:
	Cell_Item_Vector();
	~Cell_Item_Vector();
	vector<Cell_Item*> vec;
};

//所有站点
class Site_Item
{
public:
	Site_Item()
	{
		updateFlag = false;
		siteCell.clear();
	}
	~Site_Item()
	{
		clearData();
	}

	void clearData();

	map<int, Cell_Item_Vector*> &getData()
	{
		return siteCell;
	}

	void setSiteCell(map<int, Cell_Item_Vector*>& a)
	{
		siteCell = a;
	}

	void setFlag(bool a)
	{
		updateFlag = a;
	}

	void addCell(int siteID, int cell_PCI, string cell_Name);

private:
	map<int, Cell_Item_Vector*> siteCell;
	bool updateFlag;

};