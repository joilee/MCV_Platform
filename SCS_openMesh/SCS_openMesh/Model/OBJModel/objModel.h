#pragma once
#include "Model/abstractGlobalModel/abstractGlobalModel.h"
#include <cityModule/cityScene.h>
#include <util/Color.h>
#include <vector>
#include <gl/freeglut.h>

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
	void readObj(string objPath);

private:

	//城市模型场景 包括建筑物 地面模型  以及两者合成的三角面片模型

	vector<GLfloat> vertices; 
	vector<Vector3d> points;
	vector<Vector3i> faces;
	vector<Vector3d> normals;
	vector<int>f_materialId;
	vector<vector<GLfloat>> faceColor;
	vector<GLuint> indices;

	double uniform_alpha;
	Color uniformColor;
	GLuint showWireList;

	Vector3d pMax;
	Vector3d pMin;

};

