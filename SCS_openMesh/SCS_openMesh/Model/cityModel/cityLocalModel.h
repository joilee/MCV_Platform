#pragma  once
#include "../abstractModel.h"
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
    @brief ���obj�ļ�
    */
    void writeToObj();

private:
    std::vector<Vector3d> V;					// vertices
    std::vector<Vector3i> F;					// faces
    std::vector<Vector3d> NF;					 // normals of faces
    /*std::vector<Vector3d> Vn;                 // normals of vertices*/
    std::vector<int>f_materialId;                //the material of face,��Ƭ��Ӧ�Ĳ��ϱ��
    std::vector<vector<GLfloat> > faceColor;//ÿ�������ɫ
    void clearVandF();

    Color uniformColor;//ͳһ��ɫ
    double uniform_alpha;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLuint> indices;

    /*
    @brief �ӽ�����͵���ģ�͵õ�������
    */
    void generateBuildingMesh();

    int range;

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
    Vector3d MaxPos, MinPos;
    cityScene *scene;
    vector<vector<double> >normalMatrix;
    vector<vector<int> > cannyPoint;

    MESH_PTR ground_pMesh;
    
    GLuint showWireList;
public:
    /*
    @brief �̳и���ĺ���
    */
    void  draw(vector<bool> mode);
    void initDraw();
};

