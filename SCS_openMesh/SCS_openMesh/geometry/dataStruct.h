//对提供的场景几何信息进行转换，得到便于使用的OBJ格式信息，以及绕射时所需的棱边信息

#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include "util/vector.h"
#include "util/Color.h"
#include <vector>
#include <iostream>
using namespace std;

class Color;

struct  material
{
	int Id; //材质编号
	string name; //材质名称
	double thickness; //材质厚度
	Color color; //材质颜色
	double frequency;  //材质属性对应的频率
	double dielectric; //材质相对介电常数
	double permeability; //材质磁导率，一般情况下为1
	double conductivity; //材质电导率
	double Transmission_Loss; //材质透射损耗，单位dB
	double Reflection_Loss; //材质反射损耗，单位dB
};



#endif