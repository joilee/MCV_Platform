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
	@brief 局部场景类
	@note 包含局部建筑物和局部地面。不同于纯三角面元，在算法计算中需要建筑物的棱边，必须要指明建筑物。
	*/
class cityLocalModel :public  abstractLocalModel {
public:
    cityLocalModel(Vector3d center, double range, cityScene* cityAll, string _name);
    ~cityLocalModel();
 
    /*
    @brief 输出obj文件
    */
    void writeToObj(string path);

private:
    std::vector<vector<GLfloat> > faceColor;//每个面的颜色


    Color uniformColor;//统一颜色
    double uniform_alpha;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLuint> indices;

    /*
    @brief 从建筑物和地面模型得到三角形
    */
    void generateBuildingMesh();


public:
    /*
    @brief  局部场景成员函数 获得所在位置的海拔
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
    // Note: 从局部场景产生地面
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

    //地面模型数组
    //vector<Building> local_Buildings;
    cityScene *scene;
    vector<vector<double> >normalMatrix;
    vector<vector<int> > cannyPoint;

    MESH_PTR ground_pMesh;
    
    GLuint showWireList;
public:
    /*
    @brief 继承父类的函数
    */
    void  draw(vector<bool> mode,double alpha);
    void initDraw();
};

