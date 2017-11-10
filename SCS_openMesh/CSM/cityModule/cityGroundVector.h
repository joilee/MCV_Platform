#pragma once

#include "cityGround.h"
#include <vector>
using namespace std;


class cityGroundVector
{
public:
	cityGroundVector(string path);
	cityGroundVector();
	~cityGroundVector();
	vector<cityGround*> & getGroundVector(){ return groundVector; }
    bool existed();
	double getAltitudeFrom0(double x, double y);
    double getBaseByIndex(int i);
	vector<double> getBoundingBoxByIndex(int i);
	void addItem(cityGround* a);
private:
	vector<cityGround*> groundVector;
	vector<double> bases;
	vector<double> xmins;
	vector<double> xmaxs;
	vector<double> ymins;
	vector<double> ymaxs;
	vector<int>rowNumber;
	vector<int>colNumber;

};

