#pragma  once
#include "Model/abstractLocalModel/abstractLocalModel.h"
#include <util/vector.h>
#include <mesh/point.h>
#include <util/Color.h>
#include <gl/freeglut.h>
#include <memory>
#include "objModel.h"
#define meshLength 10

using namespace  std;

/*
@brief �ֲ�������
@note obj
*/
class objLocalModel :public  abstractLocalModel {
public:
	objLocalModel(Vector3d center, double localRange, string _name,objModel* cModel);
	~objLocalModel(){}

	/*
	@brief  �ֲ�������Ա���� �������λ�õĺ���
	*/
	double getAltitude(double x, double y);
	/*
	@brief �̳и���ĺ���
	*/
	void draw(vector<bool> mode, double alpha);
	void intDraw();
	void generateLocalScence(Vector3d center);

	vector<double> calculateCoffs(Vector3d a, Vector3d b, Vector3d c);
	bool isInside(Vector3d center,Vector3i face);
	bool isInMap(int id);

private:
	Color uniformColor;//ͳһ��ɫ
	double uniform_alpha;
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLuint> indices;
	vector<vector<GLfloat>> faceColor;
	GLuint showWireList;

	//����ģ��ȫ����Ϣ
	vector<Vector3d> tempVertices;
	vector<Vector3d> tempNormals;
	vector<Vector3i> tempFaces;

	map<int, int> pointID;
};

