#pragma once

#include "Model/abstractModel/abstractModel.h"
#include <util/vector.h>
#include <mesh/point.h>
#include <util/Color.h>
#include <memory>
using namespace  std;

class abstractLocalModel:public abstractModel
{
public:
	abstractLocalModel();
	virtual ~abstractLocalModel();

	int getTriangleNum() {
		return F.size();
	}
	inline Vector3d getMin() {
		return MinPos;
	}
	inline Vector3d getMax() {
		return MaxPos;
	}
	inline vector<Vector3d>& getVertices() {
		return V;
	}
	inline std::vector<Vector3i>& getFaces() {
		return F;
	}
	inline std::vector<Vector3d>&getNF() {
		return NF;
	}
	inline vector<int> &getF_material() {
		return f_materialId;
	}
	int getRange() {
		return range;
	}

	void clearVandF();

	std::vector<Vector3d> V;					// vertices
	std::vector<Vector3i> F;					// faces
	std::vector<Vector3d> NF;					 // normals of faces
	/*std::vector<Vector3d> Vn;                 // normals of vertices*/
	std::vector<int>f_materialId;                //the material of face,面片对应的材料编号
	Vector3d MaxPos, MinPos;

	int range;

	/*
	@brief  局部场景成员函数 获得所在位置的海拔
	*/
	virtual double getAltitude(double x, double y)=0;
public:


};

