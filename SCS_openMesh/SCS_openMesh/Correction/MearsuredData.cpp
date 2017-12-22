#include "MearsuredData.h"

Site_Field::~Site_Field()
{

}

void Site_Field::addPCIData(int PCI, Pci_FIeld* a)
{
	if (isPciExist(PCI))
	{
		PCI_Fields[PCI] = a;
		return;
	}
	PCI_Fields.insert(make_pair(PCI, a));
}

Pci_FIeld* Site_Field::getPCI(int pci)
{
	if (isPciExist(pci))
	{
		auto it = PCI_Fields.find(pci);
		return it->second;
	}
	return nullptr;
}

bool Site_Field::isPciExist(int pci)
{
	auto it = PCI_Fields.find(pci);
	if (it==PCI_Fields.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Measured_Data::isSiteExist(int siteid)
{
	auto it = Site_Fields.find(siteid);
	if (it==Site_Fields.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

Pci_FIeld::~Pci_FIeld()
{
	auto it = M_Field.begin();
	while (it!=M_Field.end())
	{
		delete *it;
		it++;
	}
	M_Field.clear();
}
