#include "cityLocalModel.h"
#include <canny/cannyImage.h>
#include <mesh/Mesh.h>
#include <util/emxUtilityInc.h>
#include "../../Context/context.h"
#include <gl/freeglut.h>
#include <util/Color.h>
#include <iostream>
#include <fstream>

double cityLocalModel::disP2P(Pot p, Pot q) {
    double res = (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y) + (p.z - q.z)*(p.z - q.z);
    return sqrt(res);
}
//steps
int countFeaturePoint(int steps, int localRow, int localCol, vector<vector<int> >& cannyPoint) {
    int totalVer = 0;//统计该区域的特征点数量
    if (localRow < 10 || localCol < 10) {
        steps = 2;
    }
    int stepRow = localRow / steps;
    int stepCol = localCol / steps;

    for (int n = 0; n < localCol; n++) {
        for (int m = localRow - 1; m >= 0; m--) {
            if (cannyPoint[m][n] != 0) {
                totalVer++;
            }
        }
    }

    //再计算边上的点 且不能与特征点重合
    for (int i = 0; i <= steps; i++) {
        for (int j = 0; j <= steps; j++) {
            int m = i*stepRow;
            int n = j*stepCol;
            if (m >= localRow)m = localRow - 1;
            if (n >= localCol)n = localCol - 1;
            if (cannyPoint[m][n] == 0) {
                totalVer++;
            }
        }
    }
    return totalVer;
}

Pot cityLocalModel::get_Normal(Pot p1, Pot p2, Pot p3) {
    double x1 = p2.x - p1.x;
    double y1 = p2.y - p1.y;
    double z1 = p2.z - p1.z;

    double x2 = p3.x - p2.x;
    double y2 = p3.y - p2.y;
    double z2 = p3.z - p2.z;

    double a = y1*z2 - y2*z1;
    double b = x2*z1 - x1*z2;
    double c = x1*y2 - x2*y1;

    double radix = sqrt(a*a + b*b + c*c);

    return Pot(a / radix, b / radix, c / radix);

}

void cityLocalModel::clearVandF() {
    V.clear();
    F.clear();
    f_materialId.clear();
    NF.clear();
}


void cityLocalModel::generateBuildingMesh() {
    vector<Building>  local_Buildings = scene->getTotal_Building();
    int concave_polygonNum = 0;
    for (int buildings_id = 0; buildings_id < local_Buildings.size(); buildings_id++) {
        int count = local_Buildings[buildings_id].upper_facePoint.size() - 1; //记录building顶面点坐标时，首末点重合，记录两次，所以   .size（）-1
        double building_height = local_Buildings[buildings_id].height;
        int V_size = V.size();
        vector<int> upper_PointIndex; //存储上顶面点的索引值，以备上顶面剖分生成面片时所用

        //点的存储操作
        //上顶面的点
        for (int id = 0; id < count; id++) {
            Vector3d point = local_Buildings[buildings_id].upper_facePoint[id];
            V.push_back(point);
            upper_PointIndex.push_back(V.size() - 1);
        }
        //下底面的点
        for (int id = 0; id < count; id++) {
            Vector3d point = local_Buildings[buildings_id].upper_facePoint[id];
            double under_height = point.z - building_height;
            V.push_back(Vector3d(point.x, point.y, under_height));
        }

        Vector3d E1, E2, N;
        //面片的操作
        //建筑物侧面剖分生成面片,分成两个三角形
        for (int id = 0; id < count; id++) {
            int i1, i2, i3, i4;

            i1 = id + V_size;
            i2 = (id + 1) % count + V_size;
            i3 = (id + 1) % count + count + V_size;
            i4 = id + count + V_size;

            E1 = V[i2] - V[i1];
            E2 = V[i3] - V[i2];
            N = VectorCross(E1, E2);

            if (N.norm() > DOUBLE_EPSILON) {
                F.push_back(Vector3i(i1, i2, i3));
                NF.push_back(N.normalize());
            }

            E1 = V[i3] - V[i1];
            E2 = V[i4] - V[i3];
            N = VectorCross(E1, E2);

            if (N.norm() > DOUBLE_EPSILON) {
                F.push_back(Vector3i(i1, i3, i4));
                NF.push_back(N.normalize());
            }
        }

        //建筑物上顶面剖分生成面片，面片法向量均为正z轴方向即Vector3d(0,0,1),凹多边形三角化特殊处理见网址：http://blog.sina.com.cn/s/blog_5a6f39cf0101374h.html
        bool convex = true; //为true时是凸多边形，false时是凹多边形
        vector<Vector3d> upper_face = local_Buildings[buildings_id].upper_facePoint;
        upper_face.pop_back(); //首末点重复，所以要删掉末尾的重复点
        int count1 = upper_face.size();

        for (int id = 0; id < count1; id++) {
            Vector3d v1, v2, v3;
            v1 = upper_face[id];
            v2 = upper_face[(id + 1) % count1];
            v3 = upper_face[(id + 2) % count1];
            //看v2是不是凹点
            if (Dot(VectorCross(v2 - v1, v3 - v2), Vector3d(0, 0, -1)) < DOUBLE_EPSILON) { //判断是否为凹
                //判断若为凹点，直接剔除掉v2
                std::vector<Vector3d>::iterator it1 = upper_face.begin() + (id + 1) % count1;
                upper_face.erase(it1);
                std::vector<int>::iterator it2 = upper_PointIndex.begin() + (id + 1) % count1;
                upper_PointIndex.erase(it2);

                count1--;
                id--;

                convex = false;
            }
        }
        if (!convex) {
            concave_polygonNum++;
        }

        //凸多边形剖分，选择一个顶点，然后依次寻找下两个顶点组成一个三角形
        int count2 = upper_face.size();
        for (int id = 0; id <= count2 - 3; id++) {
            E1 = V[upper_PointIndex[id + 2]] - V[upper_PointIndex[0]];
            E2 = V[upper_PointIndex[id + 1]] - V[upper_PointIndex[id + 2]];
            N = VectorCross(E1, E2);
            if (N.norm() > DOUBLE_EPSILON) {
                F.push_back(Vector3i(upper_PointIndex[0], upper_PointIndex[id + 2], upper_PointIndex[id + 1]));
                NF.push_back(N.normalize());
            }
        }
    }//一栋建筑物处理完毕
    cout << "info: 局部建筑物数量是" << local_Buildings.size() <<endl;
    cout << "info: 局部建筑物面片数量是" << F.size() << " 凹建筑物数量是" << concave_polygonNum << endl;

    int V_num = V.size();
    int F_startNum = F.size();
    int NF_startNum = NF.size();
    VERTEX2D_PTR pVtx = ground_pMesh->pVerArr;
    int numV = ground_pMesh->vertex_num;
    for (int i = 3; i<numV + 3; i++) { //前三个点是bounding triangle
        double x, y, z;
        x = ((VERTEX2D_PTR)(ground_pMesh->pVerArr + i))->x;
        y = ((VERTEX2D_PTR)(ground_pMesh->pVerArr + i))->y;
        z = ((VERTEX2D_PTR)(ground_pMesh->pVerArr + i))->z;
        V.push_back(Vector3d(x, y, z));
    }

    TRIANGLE_PTR pTri = ground_pMesh->pTriArr;
    int tri_index = 0;
    int* pi;
    while (pTri != NULL) {
        tri_index++;
        int id[3];
        pi = &(pTri->i1);
        for (int j = 0; j<3; j++) {
            id[j] = *pi++;
            id[j] = id[j] - 3 + V_num;
        }
        Vector3d E1, E2, N;
        E1 = V[id[1]] - V[id[0]];
        E2 = V[id[2]] - V[id[1]];
        N = VectorCross(E1, E2);
        if (N.norm() > DOUBLE_EPSILON) {
            //对于地面的点,法向量的z必须大于0
            if (N.z > 0) {
                F.push_back(Vector3i(id[0], id[1], id[2]));
                NF.push_back(N.normalize());
            } else { //换一个点的排列顺序 0 2 1
                F.push_back(Vector3i(id[0], id[2], id[1]));
                NF.push_back(-N.normalize());
            }
        }
        pTri = pTri->pNext;
    }
    int F_endNum = F.size();
    int NF_endNum = NF.size();
    cout << "info: 地面建模点的数量 " << V.size() - V_num << "  三角形数量 " << F_endNum - F_startNum << " 法向量数量 " << NF_endNum - NF_startNum << endl;
    // calculate the bounding box
    std::vector<Vector3d>::const_iterator v = V.begin();
    for (++v; v != V.end(); ++v) {
        MinPos = Min(MinPos, (*v));
        MaxPos = Max(MaxPos, (*v));
    }
    cout << "info: 结点生成完毕！" << endl;
}


/************************************************************************/
/* 寻找规定范围内的地面数据
1.存储二维栅格数据
2.提取特征点
3.三角剖分
*/
/************************************************************************/
void cityLocalModel::loadLocalGround(Vector3d center, double range, cityScene* scene) {

    //step1 生成局部的cityGround模型数据
    vector<cityGround*> tmpVector = scene->getGround()->getGroundVector();
    if (tmpVector.empty()) {
        cout << "error: 城市地面没有数据，请检查" << endl;
    }
    cityGround* local_Ground = tmpVector[0];

    //step2 进行局部的文件提取 canny算法 剖分

    //int nv = 0;//记录canny算法提取的特征点
    int localRow = local_Ground->getRow();
    int localCol = local_Ground->getCol();

    //初始化特征点矩阵
    //vector<vector<int> > cannyPoint;//从局部点开始的点 不是全局的
    cannyPoint.resize(localRow, vector<int>(localCol));

    //初始化向量矩阵
    normalMatrix.resize(localRow, vector<double>(localCol));
    localGetNormalMatrix();

    //以下是canny算法部分
    //1.
    cannyImage * tmpTest = new cannyImage(localRow, localCol);
    tmpTest->canny(normalMatrix, cannyPoint, localRow, localCol);

    //2.获取特征点数量，以便于初始化网格
    int  totalVer = countFeaturePoint(meshLength, localRow, localCol, cannyPoint);
    ground_pMesh = &meshCTX;
    InitMesh(ground_pMesh, totalVer);

    //3,根据已经获得的特征点，构建三角网格
    int amount = inputMeshPtr();
    cout << "Info: 局部场景地图共有" << amount - 3 << "个点" << endl;

    double last_time, this_time;
    last_time = GetTickCount();
    IncrementalDelaunay(ground_pMesh);
    this_time = GetTickCount();
    cout << "Info: 局部Delaunay剖分，共有" <<ground_pMesh->vertex_num << "个点" << endl;
    cout << "Info: 剖分耗时" << (this_time - last_time) << "ms" << endl;
}
/************************************************************************/
/*将点保存在MESH_PTR 结构中  方案3 采用内部插点                        */
/************************************************************************/
int cityLocalModel::inputMeshPtr() {

    cityGround* local_Ground = (scene->getGround()->getGroundVector())[0];
    int amount = 3;
    int localRow = local_Ground->getRow();
    int localCol = local_Ground->getCol();
    int stdLen = local_Ground->getPrecesion();
    int steps = meshLength;
    double xmin = local_Ground->getXmin();
    double ymax = local_Ground->getYmax();
    if (localRow < 10 || localCol < 10) {
        steps = 2;
    }
    int stepRow = localRow / steps;
    int stepCol = localCol / steps;
    //选择特征点
    //从左到右 从下到上
    for (int n = 0; n < localCol; n++) {
        for (int m = localRow - 1; m >= 0; m--) {
            if (cannyPoint[m][n] != 0) {
                double x = xmin + n*stdLen;
                double y = ymax - m *stdLen;
                double z = local_Ground->getMatrix(m, n);
                ((VERTEX2D_PTR)(ground_pMesh->pVerArr + amount))->x = x;
                ((VERTEX2D_PTR)(ground_pMesh->pVerArr + amount))->y = y;
                ((VERTEX2D_PTR)(ground_pMesh->pVerArr + amount))->z = z;
                amount++;
            }
        }
    }

    //再计算边上的点 且不能与特征点重合
    for (int i = 0; i <= steps; i++) {
        for (int j = 0; j <= steps; j++) {
            //
            int rowIndex = i*stepRow;
            int colIndex = j*stepCol;

            if (rowIndex >= localRow)rowIndex = localRow - 1;
            if (colIndex >= localCol)colIndex = localCol - 1;
            int m = rowIndex;
            int n = colIndex;


            if (cannyPoint[rowIndex][colIndex] == 0) {
                double x = xmin + n*stdLen;
                double y = ymax - m*stdLen;
                double z = local_Ground->getMatrix(m, n);
                ((VERTEX2D_PTR)(ground_pMesh->pVerArr + amount))->x = x;
                ((VERTEX2D_PTR)(ground_pMesh->pVerArr + amount))->y = y;
                ((VERTEX2D_PTR)(ground_pMesh->pVerArr + amount))->z = z;
                amount++;
            }
        }
    }
    return amount;
}

/************************************************************************/
/* 得到一个点的法向量
入参 原始点  周围点的vector
返回参数 Pot形式的法向量
*/
/************************************************************************/
Pot cityLocalModel::GetNormalPoint(Pot src, vector<Pot> adjPoint) {

    int adjSize = adjPoint.size();
    Pot tmpNormal = Pot(0, 0, 0);
    for (int k = 0; k<adjSize - 1; k++) {
        double per = disP2P(src, adjPoint[k + 1]) / (disP2P(src, adjPoint[k]) + disP2P(src, adjPoint[k + 1]) + disP2P(adjPoint[k], adjPoint[k + 1]));
        Pot Nk = get_Normal(adjPoint[k], src, adjPoint[k + 1]);
        tmpNormal.x += Nk.x*per;
        tmpNormal.y += Nk.y*per;
        tmpNormal.z += Nk.z*per;
    }

    if (adjSize == 8) {
        double per = disP2P(src, adjPoint[0]) / (disP2P(src, adjPoint[7]) + disP2P(src, adjPoint[0]) + disP2P(adjPoint[7], adjPoint[0]));
        Pot Nk = get_Normal(adjPoint[7], src, adjPoint[0]);
        tmpNormal.x += Nk.x*per;
        tmpNormal.y += Nk.y*per;
        tmpNormal.z += Nk.z*per;
    }

    return tmpNormal;


}

/************************************************************************/
/* 获取栅格中每个点的法向量的水平分量 ,求出九宫格构成8个三角面平均的法向量的θ角
入参:
出参: normals(θ角的形式)

备注：地图一定是要大于3*3的，不然会出问题。
added by lg
*/
/************************************************************************/
void cityLocalModel::localGetNormalMatrix() {
    cityGround* local_Ground = (scene->getGround()->getGroundVector())[0];
    double xminLocal = local_Ground->getXmin();
    double ymaxLocal = local_Ground->getYmax();
    double stdlen = local_Ground->getPrecesion();
    for (size_t i = 0; i < local_Ground->getRow(); i++) {
        for (size_t j = 0; j <local_Ground->getCol(); j++) {
            double tmpX = xminLocal+ j*stdlen;
            double tmpY = ymaxLocal - i*stdlen;
            double tmpZ = local_Ground->getMatrix(i, j);
            Pot source = Pot(tmpX, tmpY, tmpZ);//该点的三维坐标
            vector<Pot> adjPoints;
            getAdjPoint(adjPoints,i,j);
            Pot normalPoint = GetNormalPoint(source, adjPoints);//求得该点的法向量
            double theta = acos(normalPoint.z / normalPoint.getLength());//获得theta值
            normalMatrix[i][j] = theta;
        }
    }
}
/************************************************************************/
/*
根据点分布的九种情况进行处理

入参：i,j
出参：vector<Pot> adjPoints
*/
/************************************************************************/
void cityLocalModel::getAdjPoint(vector<Pot> &adjPoints,int i,int j) {
    cityGround* local_Ground = (scene->getGround()->getGroundVector())[0];
    double xmin = local_Ground->getXmin();
    double ymax = local_Ground->getYmax();
    double stdlen = local_Ground->getPrecesion();
    double tmpX = xmin + j*stdlen;
    double tmpY = ymax - i*stdlen;
    int col = local_Ground->getCol();
    int row = local_Ground->getRow();
    if (i == 0 && j == 0) { //case1: 如果左上角点不存在
        Pot P1 = Pot(tmpX + stdlen, tmpY, local_Ground->getMatrix(i,j+1));
        Pot P2 = Pot(tmpX + stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j + 1));
        Pot P3 = Pot(tmpX, tmpY - stdlen, local_Ground->getMatrix(i+1, j));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
    } else if (i == 0 && j<col - 1 && j>0) { //case2:
        Pot P1 = Pot(tmpX + stdlen, tmpY, local_Ground->getMatrix(i,j+1));
        Pot P2 = Pot(tmpX + stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j + 1));
        Pot P3 = Pot(tmpX, tmpY - stdlen, local_Ground->getMatrix(i+1, j ));
        Pot P4 = Pot(tmpX - stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j - 1));
        Pot P5 = Pot(tmpX - stdlen, tmpY, local_Ground->getMatrix(i, j - 1));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
        adjPoints.push_back(P4);
        adjPoints.push_back(P5);

    } else if (i == 0 && j == col - 1) { //case3
        Pot P1 = Pot(tmpX, tmpY - stdlen, local_Ground->getMatrix(i+1, j ));
        Pot P2 = Pot(tmpX - stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j-1));
        Pot P3 = Pot(tmpX - stdlen, tmpY, local_Ground->getMatrix(i, j - 1));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
    } else if (0 < i&&i < row - 1 && j == 0) { //case4
        Pot P1 = Pot(tmpX, tmpY + stdlen, local_Ground->getMatrix(i-1, j ));
        Pot P2 = Pot(tmpX + stdlen, tmpY + stdlen, local_Ground->getMatrix(i-1, j + 1));
        Pot P3 = Pot(tmpX + stdlen, tmpY, local_Ground->getMatrix(i, j + 1));
        Pot P4 = Pot(tmpX + stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j +1));
        Pot P5 = Pot(tmpX, tmpY - stdlen, local_Ground->getMatrix(i+1, j ));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
        adjPoints.push_back(P4);
        adjPoints.push_back(P5);
    } else if (0 < i&&i < row - 1 && 0 < j&&j < col - 1) { //case5
        //从该点的上面一个点计算
        Pot P1 = Pot(tmpX, tmpY + stdlen, local_Ground->getMatrix(i-1, j ));
        Pot P2 = Pot(tmpX + stdlen, tmpY + stdlen, local_Ground->getMatrix(i-1, j + 1));
        Pot P3 = Pot(tmpX + stdlen, tmpY, local_Ground->getMatrix(i, j + 1));
        Pot P4 = Pot(tmpX + stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j + 1));
        Pot P5 = Pot(tmpX, tmpY - stdlen, local_Ground->getMatrix(i+1, j));
        Pot P6 = Pot(tmpX - stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j - 1));
        Pot P7 = Pot(tmpX - stdlen, tmpY, local_Ground->getMatrix(i, j - 1));
        Pot P8 = Pot(tmpX - stdlen, tmpY + stdlen,local_Ground->getMatrix(i-1, j - 1));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
        adjPoints.push_back(P4);
        adjPoints.push_back(P5);
        adjPoints.push_back(P6);
        adjPoints.push_back(P7);
        adjPoints.push_back(P8);
    } else if (0 < i&&i < row - 1 && j == col - 1) { //case6
        Pot P1 = Pot(tmpX, tmpY - stdlen, local_Ground->getMatrix(i+1, j));
        Pot P2 = Pot(tmpX - stdlen, tmpY - stdlen, local_Ground->getMatrix(i+1, j - 1));
        Pot P3 = Pot(tmpX - stdlen, tmpY, local_Ground->getMatrix(i, j - 1));
        Pot P4 = Pot(tmpX - stdlen, tmpY + stdlen, local_Ground->getMatrix(i-1, j - 1));
        Pot P5 = Pot(tmpX, tmpY + stdlen, local_Ground->getMatrix(i-1, j));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
        adjPoints.push_back(P4);
        adjPoints.push_back(P5);
    } else if (j == 0 && i == row - 1) { //case7
        Pot P1 = Pot(tmpX, tmpY + stdlen, local_Ground->getMatrix(i-1, j));
        Pot P2 = Pot(tmpX + stdlen, tmpY + stdlen, local_Ground->getMatrix(i-1, j+1));
        Pot P3 = Pot(tmpX + stdlen, tmpY, local_Ground->getMatrix(i, j + 1));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
    } else if (i == row - 1 && 0 < j&&j < col - 1) { //case8
        Pot P1 = Pot(tmpX - stdlen, tmpY, local_Ground->getMatrix(i, j - 1));
        Pot P2 = Pot(tmpX - stdlen, tmpY + stdlen, local_Ground->getMatrix(i-1, j - 1));
        Pot P3 = Pot(tmpX, tmpY + stdlen, local_Ground->getMatrix(i-1, j));
        Pot P4 = Pot(tmpX + stdlen, tmpY + stdlen, local_Ground->getMatrix(i-1, j+1));
        Pot P5 = Pot(tmpX + stdlen, tmpY, local_Ground->getMatrix(i, j + 1));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
        adjPoints.push_back(P4);
        adjPoints.push_back(P5);
    } else if (i == row - 1 && j == col - 1) { //case9
        Pot P1 = Pot(tmpX - stdlen, tmpY, local_Ground->getMatrix(i, j - 1));
        Pot P2 = Pot(tmpX - stdlen, tmpY + stdlen, local_Ground->getMatrix(i-1, j - 1));
        Pot P3 = Pot(tmpX, tmpY + stdlen, local_Ground->getMatrix(i-1, j));
        adjPoints.push_back(P1);
        adjPoints.push_back(P2);
        adjPoints.push_back(P3);
    }
}

/************************************************************************/
/* 从一个整个城市模型场景中，构建一个局部场景

*/
/************************************************************************/
cityLocalModel::cityLocalModel(Vector3d  AP_position, double  LocalRange, cityScene* cityAll,string _name) {
    range = LocalRange;
    MaxPos = Vector3d(DBL_MIN, DBL_MIN, DBL_MIN);
    MinPos = Vector3d(DBL_MAX, DBL_MAX, DBL_MAX);
    name = _name + "_Local_Model";
    scene = new cityScene(AP_position,LocalRange,cityAll);
    loadLocalGround(AP_position, LocalRange, scene);
    cout << "Info: 地面场景构建完成" << endl;

    clearVandF();
    generateBuildingMesh();

    //添加材料信息
    globalContext *globalCtx = globalContext::GetInstance();
    int defaultID = globalCtx->modelManager->matManager->getDefaultMaterial();
    f_materialId = vector<int>(F.size(),defaultID);

    uniformColor = Color(0.0f, 0.0f, 0.0f);
    initDraw();
}
cityLocalModel::~cityLocalModel() {
}

void cityLocalModel::initDraw() {
    //faceColor.resize(F.size(), vector<double>(4));
    globalContext *globalCtx = globalContext::GetInstance();
    double last_time, material_time,this_time;

    //统一透明度和统一的颜色
    uniform_alpha = globalCtx->modelManager->getAlpha();
    int defaultID = globalCtx->modelManager->matManager->getDefaultMaterial();
    int index = globalCtx->modelManager->matManager->getVectorIndexFromID(defaultID);
    uniformColor = globalCtx->modelManager->matManager->getColor(index);



    //获得每一个面的颜色
    for (int i = 0; i < F.size(); i++) {
        int index = globalCtx->modelManager->matManager->getVectorIndexFromID(f_materialId[i]);
        Color tmp = globalCtx->modelManager->matManager->getColor(index);

        vector<float> colorVector;
        colorVector.push_back((double)tmp.r/256.0);
        colorVector.push_back((double)tmp.g/256.0);
        colorVector.push_back((double)tmp.b/256.0);
        colorVector.push_back(uniform_alpha);
        faceColor.push_back(colorVector);
    }

    //计时开始
    last_time = GetTickCount();

    //以下所有都是根据面来设置，比如第一个面编号是1，2，3，第二个面编号是3，4，5

    //构造opengl坐标数组、索引数组、向量数组
    for (int i = 0; i < F.size(); i++) {
        Vector3i vIndex = F[i];//3个点
        for (int j = 0; j < 3; j++) {
            //索引坐标
            indices.push_back(i*3+j);
            //点的坐标
            vertices.push_back(V[vIndex[j]].x);
            vertices.push_back(V[vIndex[j]].y);
            vertices.push_back(V[vIndex[j]].z);
            //向量坐标
            normals.push_back(NF[i].x);
            normals.push_back(NF[i].y);
            normals.push_back(NF[i].z);
        }
    }
    this_time = GetTickCount();

    showWireList = glGenLists(1);
    // SHOW WIRE
    glNewList(showWireList, GL_COMPILE);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glColor4f(0.0f, 0.0f, 0.0f,1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < F.size(); i++) {
        for (int j = 0; j < 3; j++) {
            glVertex3d((GLdouble)V[F[i][j]].x, (GLdouble)V[F[i][j]].y, (GLdouble)V[F[i][j]].z);
            glVertex3d((GLdouble)V[F[i][(j + 1) % 3]].x, (GLdouble)V[F[i][(j + 1) % 3]].y, (GLdouble)V[F[i][(j + 1) % 3]].z);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEndList();
    cout << "info:局部场景openGL数据初始化完成，共耗时"<<(this_time-last_time)/1000<<"s" << endl;
}

void cityLocalModel:: writeToObj() {
    ofstream fout("D:\\test.obj");
    for (int i = 0; i < F.size(); i++) {
        Vector3i vIndex = F[i];//3个点
        for (int j = 0; j < 3; j++) {
            //点的坐标
            fout << "v " << V[vIndex[j]].x << " " << V[vIndex[j]].y << " " << V[vIndex[j]].z << endl;
        }
    }
    for (int i = 0; i < F.size(); i++) {
        for (int j = 0; j < 3; j++) {
            fout << "vn " << NF[i].x << " " << NF[i].y << " " << NF[i].z << endl;
        }
    }

    for (int i = 0; i < F.size(); i++) {
        fout << "f ";
        for (int j = 0; j < 3; j++) {
            fout << i * 3 + j+1 << "/" << i * 3 + j+1<<" ";
        }
        fout << endl;
    }
    fout.close();
}

void cityLocalModel::draw(vector<bool> mode) {
    if (mode[0]) { //draw vertice

    }
    if (mode[1]) { //draw line
        //glCallList(showWireList);
        glDisable(GL_LIGHTING);
        glLineWidth(2.0f);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < F.size(); i++) {
            for (int j = 0; j < 3; j++) {
                glVertex3d((GLdouble)V[F[i][j]].x, (GLdouble)V[F[i][j]].y, (GLdouble)V[F[i][j]].z);
                glVertex3d((GLdouble)V[F[i][(j + 1) % 3]].x, (GLdouble)V[F[i][(j + 1) % 3]].y, (GLdouble)V[F[i][(j + 1) % 3]].z);
            }
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
    if(mode[2]) { //draw face
        // enable and specify pointers to vertex arrays
        glEnableClientState(GL_NORMAL_ARRAY);;
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor4f(uniformColor.r/256.0,uniformColor.g/256.0, uniformColor.b/256.0, uniform_alpha);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

double cityLocalModel:: getAltitude(double x, double y) {
    //step1 生成局部的cityGround模型数据
    cityGround* local_Ground = (scene->getGround()->getGroundVector())[0];
    if (local_Ground == NULL) {
        cout << "error : 不存在局部模型" << endl;
        return -1000;
    }
    return local_Ground->getPointAltitude(x, y);
}
