#ifndef ANTENNA_H
#define  ANTENNA_H
#include <string>
#include "../util/vector.h"
#include <vector>
class TransAntenna
{
public:
	//	int index;
	//int Site_Name;
	string Cell_Name;
	int PCI;
	double initial_Gain;
	std::vector<std::vector<double>> direction_Gain; //方向增益
	Vector3d position; //坐标位置
	Vector3d polor_direction; //极化方向，即电场方向
	double frequency; //频率
	double trans_power; //发射源功率
	double wire_loss; //线缆损耗
	double enlarge_power; //扩放增益
	double phi,theta; //天线朝向俯仰角、方位角，phi是水平角，theta是垂直角
};

class Site
{
public:
	int Site_Name;
	std::vector<TransAntenna>Site_Antennas;
};


#endif // !ANTENNA_H

