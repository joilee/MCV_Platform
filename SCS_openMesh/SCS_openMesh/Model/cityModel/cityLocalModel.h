#pragma  once
#include "../abstractModel.h"
#include <cityModule/cityScene.h>
#include <mesh/meshStruct.h>
#include <util/vector.h>
#include <mesh/point.h>
#include <util/Color.h>
#include <gl/freeglut.h>

#define meshLength 10

using namespace  std;

/*
	@brief 局部场景类
	@note 包含局部建筑物和局部地面。不同于纯三角面元，在算法计算中需要建筑物的棱边，必须要指明建筑物。
	*/
class cityLocalModel :public abstractModel {
public:
    cityLocalModel(Vector3d center, double range, cityScene* cityAll, string _name);
    ~cityLocalModel();
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

    /*
    @brief 输出obj文件
    */
    void writeToObj();

private:
    std::vector<Vector3d> V;					// vertices
    std::vector<Vector3i> F;					// faces
    std::vector<Vector3d> NF;					 // normals of faces
    /*std::vector<Vector3d> Vn;                 // normals of vertices*/
    std::vector<int>f_materialId;                //the material of face,面片对应的材料编号
    std::vector<vector<GLfloat> > faceColor;//每个面的颜色
    void clearVandF();

    Color uniformColor;//统一颜色
    double uniform_alpha;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLuint> indices;

    /*
    @brief 从建筑物和地面模型得到三角形
    */
    void generateBuildingMesh();

    int range;

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

    //************************************
    // Method:    getRange
    // FullName:  cityLocalModel::getRange
    // Access:    public 
    // Returns:   int
    // Qualifier:
    //************************************
    int getRange() {
        return range;
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
    Vector3d MaxPos, MinPos;
    cityScene *scene;
    vector<vector<double> >normalMatrix;
    vector<vector<int> > cannyPoint;
    MESH meshCTX;
    MESH_PTR ground_pMesh;
    //
    GLuint showWireList;
public:
    /*
    @brief 继承父类的函数
    */
    string name;
    inline string  getName() {
        return name;
    }
    void  draw(vector<bool> mode);
    void initDraw();
};

