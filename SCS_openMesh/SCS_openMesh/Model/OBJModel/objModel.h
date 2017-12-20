#pragma once
#include "Model/abstractGlobalModel/abstractGlobalModel.h"
#include <cityModule/cityScene.h>
#include <vector>
#include <gl/freeglut.h>
#include <mesh/point.h>

class objModel :public abstractGlobalModel
{
public:
	objModel(string objPath);
	~objModel(){}
	void draw(vector<bool> mode, double alpha);
	void initDraw();
	double getAltitude(double x, double y);
	Vector3d getMinPoint();
	Vector3d getMaxPoint();
	int getBuildingSize(){ return 0; }
	int getConcaveSize(){ return 0; }
	void getNormals();

private:

	//城市模型场景 包括建筑物 地面模型  以及两者合成的三角面片模型

	vector<GLfloat> vertices; 
	vector<Pot> points;
	vector<int> faces;
	vector<Vector3d> normals;

	double uniform_alpha;
	Color uniformColor;

	Vector3d pMax;
	Vector3d pMin;

};

