#pragma  once
#include "Model/abstractLocalModel/abstractLocalModel.h"
#include <cityModule/cityScene.h>
#include <mesh/meshStruct.h>
#include <util/vector.h>
#include <mesh/point.h>
#include <util/Color.h>
#include <gl/freeglut.h>
#include <memory>
#define meshLength 10

using namespace  std;

/*
	@brief �ֲ�������
	@note �����ֲ�������;ֲ����档��ͬ�ڴ�������Ԫ�����㷨��������Ҫ���������ߣ�����Ҫָ�������
	*/
class cityLocalModel :public  abstractLocalModel {
public:
    cityLocalModel(Vector3d center, double range, cityScene* cityAll, string _name);
    ~cityLocalModel();
 
    /*
    @brief ���obj�ļ�
    */
    void writeToObj(string path);

private:
    std::vector<vector<GLfloat> > faceColor;//ÿ�������ɫ


    Color uniformColor;//ͳһ��ɫ
    double uniform_alpha;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLuint> indices;

    /*
    @brief �ӽ�����͵���ģ�͵õ�������
    */
    void generateBuildingMesh();


public:
    /*
    @brief  �ֲ�������Ա���� �������λ�õĺ���
    */
    double getAltitude(double x, double y);
    cityScene *getScene() {
        return scene;
    }
	MESH_PTR getMesh() {
        return ground_pMesh;
    }


private:

    //************************************
    // Method:    loadLocalGround
    // Note: �Ӿֲ�������������
    // FullName:  cityLocalModel::loadLocalGround
    // Access:    private 
    // Returns:   void
    // Qualifier:
    // Parameter: Vector3d center
    // Parameter: double range
    // Parameter: cityScene * cityLocal
    //************************************
    void loadLocalGround(Vector3d center, double range, cityScene* cityLocal);
    void localGetNormalMatrix();
    void getAdjPoint(vector<Pot> &adjPoints, int i, int j);
    Pot GetNormalPoint(Pot src, vector<Pot> adjPoint);
    double disP2P(Pot p, Pot q);
    Pot get_Normal(Pot p1, Pot p2, Pot p3);
    int inputMeshPtr();

    //����ģ������
    //vector<Building> local_Buildings;
    cityScene *scene;
    vector<vector<double> >normalMatrix;
    vector<vector<int> > cannyPoint;

    MESH_PTR ground_pMesh;
    
    GLuint showWireList;
public:
    /*
    @brief �̳и���ĺ���
    */
    void  draw(vector<bool> mode,double alpha);
    void initDraw();
};

