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
	vector<double> calculateCoffs(Vector3d a, Vector3d b,Vector3d c);

	vector<Vector3d> getPoints(){ return points; }
	vector<Vector3i> getFaces(){ return faces; }
	vector<Vector3d> getFaceNormals(){ return normals; }

private:

	//����ģ�ͳ��� ���������� ����ģ��  �Լ����ߺϳɵ�������Ƭģ��

	vector<Vector3d> points;
	vector<Vector3i> faces;
	vector<Vector3d> normals;

	vector<int>f_materialId;
	vector<vector<GLfloat>> faceColor;
	vector<GLuint> indices;
	vector<GLfloat> vertices;
	vector<GLfloat> m_normals;
	double uniform_alpha;
	Color uniformColor;
	GLuint showWireList;

	Vector3d pMax;
	Vector3d pMin;

};

