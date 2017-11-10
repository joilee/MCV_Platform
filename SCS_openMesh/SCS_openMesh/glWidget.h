//QT中编写opengl，需用到“QGLWidget”类，继承QGLWidget类，在protected成员里加上三个成员函数：initializeGL()、paintGL()、resizeGL()

#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QtOpenGL/QGLWidget>
#include <vector>
#include "util/vector.h"
#include "util/emxCamera.h"
#include "geometry/emxModel.h"
#include "geometry/dataStruct.h"
#include "Context/context.h"
#include "util/EField.h"
#include <QPoint>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_3_Core>
struct material;
struct building;
class emxModel;
class QMouseEvent;
class QWheelEvent;

class GLWidget: public QGLWidget, protected QOpenGLFunctions_4_3_Core {
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

public:
    vector<GLfloat> vertices;
    vector<GLfloat>colorVector;
    vector<GLint> indices;
    GLuint  vertexBufferID, colorBufferID,indexBufferID;
    //
    int m_iWidth;	/**< the width of the canvas */
    int m_iHeight;	/**< the height of the canvas */

    Color m_backGroundColor;/**< scene background color(OpenGL clear color) */

    // Ground
    Vector3d m_groundCenter;/**< the center position of the ground */
    double m_dGroundWidth;

    Color m_groundColor;


    //Light
    Vector3d m_lightPos;	/**< light position */
    Color m_lightColor;		/**< light color */

    emxCamera m_camera;

    //材料信息
    vector<material> materials;
    int defaultMaterial;
    //展示整体的几何场景
    emxModel *TriangleModel;  //场景对象，绘制局部场景必须要使用
    Vector3d minPos,maxPos;
    QPoint lastPos,currentPos;

    vector<building> m_Buildings;


    bool drawVectorScene;  //vector文件场景绘制
    double vis_factor_scence;//场景的透明度

    bool drawSimplaneFlag;
    double vis_factor_face;
    vector<int> horizonNum;
    vector<int> veticalNum;
    double Tmax;
    double Tmin;
    vector<vector<EField>> AP_EPoints;  //记录接收点处相关信息，以便仿真面的绘制
    vector<bool> sceneIsDislpay;   //是否显示场景仿真面结果


    //
    bool drawLocalPoint;
    bool drawLocalLine;
    bool drawLocalFace;

public:
    void updateMesh();

    void drawAllScene();//全局，建筑物展示，无地面，建筑物本身有海拔

    void drawLocalScene();//局部信息，只有三角面片

    void drawCoordinates();

    void setMaterial(vector<material> &materialdatabase) {
        materials = materialdatabase;
    }

    void resetRenderColor();

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void setTriangleModel(emxModel* TriangleData);

    void drawPlane();

    void drawColorbar();

    void LoadUniformColor(double currentVaule,Color &result);

    void removeTriangleModel() {
        //drawTriangleScene=false;
        minPos=Vector3d(0,0,0);
    }
    void setBuilding(vector<building> &Buildings, Vector3d MaxPoint, Vector3d MinPoint) {
        m_Buildings.clear();
        m_Buildings=Buildings;
        drawVectorScene = true;
        minPos = MinPoint;
        maxPos = MaxPoint;
        updateMesh();
        m_camera.zoomOut(2.6);
    }
    void setGrid(vector<int> level, vector<int> vertical) {
        horizonNum = level;
        veticalNum = vertical;
        for (int i=0; i<horizonNum.size(); i++) {
            horizonNum[i]++;
        }
        for (int i=0; i<veticalNum.size(); i++) {
            veticalNum[i]++;
        }
    }
    void setSimPlane(vector<vector<EField>> &PlacePoint,vector<int> h,vector<int> v) {
        AP_EPoints=PlacePoint;
        setGrid(h,v);
        sceneIsDislpay.push_back(true);
        drawSimplaneFlag=true;
    }

    //
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
};


#endif