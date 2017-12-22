#pragma once
#include <util/vector.h>
#include <vector>
#include <map>
class Measured_Field
{
public:
	Vector3d position;
	double RSRP;
};

class Pci_FIeld
{
public:
	~Pci_FIeld();
	int PCI;
	vector<Measured_Field*> M_Field;
};

class Site_Field
{
public:
	~Site_Field();
	Vector3d AP_position;
	map<int, Pci_FIeld*> PCI_Fields;
	void addPCIData(int PCI,Pci_FIeld* a);
	Pci_FIeld* getPCI(int pci);
	bool isPciExist(int pci);
};

class Measured_Data
{
public:
	map<int, Site_Field*> Site_Fields;
	bool isSiteExist(int siteid);
	void addSiteData(int siteID);

};


