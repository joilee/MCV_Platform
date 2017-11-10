#pragma once
#include <QOpenGLFunctions_4_3_Core>
#include <QtOpenGL/QGLWidget>
#include "util/vector.h"
#include "util/emxCamera.h"
#include <QPoint>
#include <QMouseEvent>

class QMouseEvent;
class QWheelEvent;

//openGL基类
class scsGLWidget:public QGLWidget, protected QOpenGLFunctions_4_3_Core 
{
	Q_OBJECT
public:
	//绘制基本数据
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
	Vector3d minPos, maxPos;
	QPoint lastPos, currentPos;

	scsGLWidget(QWidget* parent = 0);

	~scsGLWidget();

	void initialScene()
	{
		m_lightPos = Vector3d(0, 0.55, 200);
		m_groundCenter = Vector3d(0, 0, 0);
		m_dGroundWidth = 5;
		minPos = Vector3d(DBL_MAX, DBL_MAX, DBL_MAX);
		maxPos = Vector3d(DBL_MIN, DBL_MIN, DBL_MIN);
		setMouseTracking(true);
	}

	/**
	  * @Method:    updateMesh
	  * @Note: 	
	  * @Author:    Li Gen
	  * @ Date:     2017/10/25
	  * @Access:    public 
	  * @Returns:   void
	  * @Qualifier:
	*/
	void updateMesh();

	void drawCoordinates();

	void resetRenderColor();

	void mousePressEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

	void wheelEvent(QWheelEvent *event);

	void LoadUniformColor(double currentVaule, Color &result);

	void initializeGL();

	void paintGL();

	void resizeGL(int width, int height);
};