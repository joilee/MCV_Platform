#ifndef PREDICTIONINDEX_H
#define PREDICTIONINDEX_H
#include "util/Vector.h"
#include <iostream>
using namespace std;

struct field_BestServer
{
	Vector3d position;
	double Max_Strength;
	int BestServerIndex; //最佳服务小区的索引
};

struct field_SINR
{
	Vector3d position;
	double SINR;
};


struct field_Throughput
{
	Vector3d position;
	double throughput;
};

struct field_strength
{
	Vector3d position;
	double Strength;
};


struct MCSEfficiency
{
	double SINR;
	double MCS;
};

#endif