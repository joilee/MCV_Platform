#include "visualResultItem.h"

void Site_Item::clearData()
{
	auto it = siteCell.begin();
	while (it != siteCell.end())
	{
		delete it->second;
		it++;
	}
	siteCell.clear();
}

void Site_Item::addCell(int siteID, int cell_PCI, string cell_Name)
{
	Cell_Item * tmp = new Cell_Item(cell_PCI, cell_Name);

	if (siteCell.find(siteID)==siteCell.end())
	{
		siteCell.insert(make_pair(siteID, new Cell_Item_Vector()));
	}

	auto it = siteCell.find(siteID);
	it->second->vec.push_back(tmp);
	return;
}

bool Site_Item::needUpdate()
{
	return updateFlag;
}

Cell_Item_Vector::Cell_Item_Vector()
{
	vec.clear();
}

Cell_Item_Vector::~Cell_Item_Vector()
{
	auto it = vec.begin();
	while (it!=vec.end())
	{
		delete *it;
		it++;
	}
	vec.clear();
}
