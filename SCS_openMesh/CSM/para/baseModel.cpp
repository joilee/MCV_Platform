#include "baseModel.h"
#include <fstream>
using namespace std;

BaseModel::BaseModel()
{
	LocalScene_range = 0;
	m_Type = ModelType::UNDEFINED;
}

BaseModel::~BaseModel()
{
	V.clear();
	F.clear();
	NF.clear();
	f_materialId.clear();
	AP_Edge_list.clear();
}

double BaseModel::getPointAltitude(double x, double y)
{
	if (m_Type==ModelType::CITY_LOCAL)
	{
		return localScene->getAltitude(x, y);
	}
	else 
	{
		
	}

}

void BaseModel::outputOBJ()
{
	ofstream outfile;
	outfile.open("mesh.obj"); 
	outfile.setf(ios::fixed,ios::floatfield);
	outfile.precision(8);
	for (int i = 0; i < V.size(); i++)
	{
		
		outfile << 'v' << " "  << V[i].x << " " << V[i].y << " " << V[i].z << endl;
	}
	for (int i = 0; i < F.size();i++)
	{
		outfile << 'f' << " " << F[i].x+1 << " " << F[i].y+1 << " " << F[i].z+1 << endl;
	}
	outfile.close();
}
