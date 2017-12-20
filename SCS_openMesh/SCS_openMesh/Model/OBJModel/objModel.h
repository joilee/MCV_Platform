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

	//����ģ�ͳ��� ���������� ����ģ��  �Լ����ߺϳɵ�������Ƭģ��

	vector<GLfloat> vertices; 
	vector<Pot> points;
	vector<int> faces;
	vector<Vector3d> normals;

	double uniform_alpha;
	Color uniformColor;

	Vector3d pMax;
	Vector3d pMin;

};

