#include "EField.h"

Site_Data::Site_Data(int id)
{
	siteID = id;
	init();
}

Site_Data::~Site_Data()
{
	auto it = cellsMap.begin();
	for (; it != cellsMap.end();it++)
	{
		delete it->second;
	}
	init();
}

void Site_Data::init()
{
	map<int, Cell_Data*> emptyMap;
	cellsMap.swap(emptyMap);
}

Cell_Data::Cell_Data(int PCI)
{
	pci = PCI;
	cell_name = "";
	efildVec.clear();
}

Cell_Data::~Cell_Data()
{
	// Õ∑≈vector
	auto it = efildVec.begin();
	for (; it != efildVec.end();it++)
	{
		delete *it;
	}
	// Õ∑≈ƒ⁄¥Ê
	vector<EField*>().swap(efildVec);
}
