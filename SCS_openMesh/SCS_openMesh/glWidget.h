//QT�б�дopengl�����õ���QGLWidget���࣬�̳�QGLWidget�࣬��protected��Ա�����������Ա������initializeGL()��paintGL()��resizeGL()

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

    //������Ϣ
    vector<material> materials;
    int defaultMaterial;
    //չʾ����ļ��γ���
    emxModel *TriangleModel;  //�������󣬻��ƾֲ���������Ҫʹ��
    Vector3d minPos,maxPos;
    QPoint lastPos,currentPos;

    vector<building> m_Buildings;


    bool drawVectorScene;  //vector�ļ���������
    double vis_factor_scence;//������͸����

    bool drawSimplaneFlag;
    double vis_factor_face;
    vector<int> horizonNum;
    vector<int> veticalNum;
    double Tmax;
    double Tmin;
    vector<vector<EField>> AP_EPoints;  //��¼���յ㴦�����Ϣ���Ա������Ļ���
    vector<bool> sceneIsDislpay;   //�Ƿ���ʾ������������


    //
    bool drawLocalPoint;
    bool drawLocalLine;
    bool drawLocalFace;

public:
    void updateMesh();

    void drawAllScene();//ȫ�֣�������չʾ���޵��棬�����ﱾ���к���

    void drawLocalScene();//�ֲ���Ϣ��ֻ��������Ƭ

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