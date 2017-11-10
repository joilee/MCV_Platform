#pragma once
#include "../util/vector.h"
#include <vector>
using namespace  std;

struct Vedge
{
	Vector3d start;  //起始点
	Vector3d end;  //终止点
	//夹棱边两面的法向量
	Vector3d normal_front;
	Vector3d normal_back;
	int materialId;  //棱边所在object的材料属性
};


class Building
{
public:
	Building();
	~Building();
	vector<Vector3d> upper_facePoint;  //建筑物上顶面点坐标
	double height;  //建筑物高度，用上顶面点z坐标减去高度即可获得下底面对应点的高度z坐标
};