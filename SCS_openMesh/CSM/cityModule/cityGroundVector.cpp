#include "cityGroundVector.h"
#include <fstream>
#include <iostream>
#include <cmath>
cityGroundVector::cityGroundVector(string indn)
{
	groundVector.clear();
	ifstream Findex;
	int addrOfIndn = 0;
	for (int i = indn.size() - 1; i >= 0; i--)
	{
		if (indn[i] == '\\' || indn[i] == '/')
		{
			addrOfIndn = i;
			break;
		}
	}
	string headFile = indn.substr(0, addrOfIndn + 1);
	Findex.open(indn);
	vector<string> Hnames;
	while (Findex.good() && !Findex.eof())
	{
		string hn = "";
		double xmin, xmax, ymin, ymax, base;
		Findex >> hn;
		if (hn != "")
		{
			Findex >> xmin >> xmax >> ymin >> ymax >> base;
			//cout<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<base<<endl;
			Hnames.push_back(hn);
			xmins.push_back(xmin);
			xmaxs.push_back(xmax);
			ymins.push_back(ymin);
			ymaxs.push_back(ymax);
			bases.push_back(base);
			int row = int((ymax - ymin) / base);
			int col = int((xmax - xmin) / base);

		}

	}
	Findex.close();
	for (int i = 0; i < Hnames.size();i++)
	{
		string tt = headFile + Hnames[i];
		groundVector.push_back(new cityGround(tt, xmins[i], xmaxs[i], ymins[i], ymaxs[i], bases[i]));
	}
}

bool cityGroundVector::existed()
{
	if (groundVector.empty())
	{
		return false;
	}
	return true;
}

double cityGroundVector:: getAltitudeFrom0(double x, double y)
{
	return groundVector[0]->getPointAltitude(x, y);
}

double cityGroundVector::getBaseByIndex(int i)
{
	if (i<0||i>=bases.size())
	{
		cout << "error: wrong  Base Index!" << endl;
		return -1;
	}
	return bases[i];
}
vector<double> cityGroundVector::getBoundingBoxByIndex(int i)
{
	vector<double> res;
	if (i < 0 || i >= xmins.size())
	{
		cout << "error: wrong  BoundingBox Index!" << endl;
		return res;
	}
	res.push_back(xmins[i]);
	res.push_back(xmaxs[i]);
	res.push_back(ymins[i]);
	res.push_back(ymaxs[i]);
	return res;
}

cityGroundVector::~cityGroundVector()
{
	vector<cityGround*>(groundVector).swap(groundVector);
}
cityGroundVector::cityGroundVector()
{
	bases.clear();
	xmins.clear();
	xmaxs.clear();
	ymaxs.clear();
	ymins.clear();
	rowNumber.clear();
	colNumber.clear();
	groundVector.clear();
}
void cityGroundVector::addItem(cityGround* a){
	//assert(a);
	bases.push_back(a->getPrecesion());
	xmins.push_back(a->getXmin());
	xmaxs.push_back(a->getXmax());
	ymins.push_back(a->getYmin());
	ymaxs.push_back(a->getYmax());
	rowNumber.push_back(a->getRow());
	colNumber.push_back(a->getCol());
	groundVector.push_back(a);
}