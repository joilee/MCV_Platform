#include "scsGLWidget.h"
#include <gl/freeglut.h>
#include "util/emxUtilityInc.h"

/**
  * @Method:    vertexCallback
  * @Note:      回调函数
  * @Author:    Li Gen
  * @ Date:     2017/10/25
  * @Access:    public 
  * @Returns:   void CALLBACK
  * @Qualifier: 
  * @Parameter: GLvoid * vertex
*/
void  CALLBACK vertexCallback(GLvoid *vertex)  //点坐标信息
{
	const GLdouble *pointer = (GLdouble *)vertex;
	//glColor3dv(pointer + 3);//在此设置颜色  
	glVertex3dv(pointer);
}
void CALLBACK beginCallback(GLenum which)
{
	glBegin(which);
}
void CALLBACK endCallback()
{
	glEnd();
}
void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte *estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}
void CALLBACK combineCallback(GLdouble coords[3],
	GLdouble *vertex_data[4],
	GLfloat weight[4], GLdouble **dataOut)
{
	GLdouble *vertex;
	int i;
	vertex = (GLdouble *)malloc(6 * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	for (i = 3; i < 7; i++)
	{
		vertex[i] = weight[0] * vertex_data[0][i]
			+ weight[1] * vertex_data[1][i]
			+ weight[2] * vertex_data[2][i]
			+ weight[3] * vertex_data[3][i];
	}
	*dataOut = vertex;
}


scsGLWidget::scsGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	resetRenderColor();
	initialScene();
}

void scsGLWidget::resetRenderColor()
{
	m_lightColor = Color::WHITE;
	m_backGroundColor = Color::MediumSlateBlue;
	m_groundColor = Color::GRAY;
}

void scsGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	////定义材料属性
	float mat_specular[] = { 0.3f, 0.3f, 0.3f, 0.3f };
	float mat_shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	//创建光源
	float light0_position[] = { m_lightPos.x, m_lightPos.y, m_lightPos.z, 0 };
	float light0_diffuse[] = { m_lightColor.r, m_lightColor.g, m_lightColor.b, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);  //光源位置
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);  //散射光
	// 指定环境光的RGBA强度值
	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//定义光照模型
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(m_backGroundColor.r, m_backGroundColor.g, m_backGroundColor.b, 0.0);   //设置当前清除颜色
	glShadeModel(GL_SMOOTH);  //平滑着色
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// 基于源象素alpha通道值的半透明混合函数

	glEnable(GL_CULL_FACE);//剔除背面
}

void scsGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //用设定的当前清除值清除指定的缓冲区
	m_camera.setupModelMatrix();
	drawCoordinates();
}

void scsGLWidget::resizeGL(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;

	// set OpenGL viewport
	glViewport(0, 0, width, height);
	m_camera.setWindowSize(width, height);
	m_camera.setPerspectiveProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

scsGLWidget:: ~scsGLWidget()
{

}

void scsGLWidget::updateMesh()
{
	m_camera.setSceneBBox(minPos, maxPos);
	// update ground attribute
	Vector3d dist = maxPos - minPos;
	m_groundCenter = (minPos + maxPos) * .5;
	m_groundCenter.z = minPos.z;
	m_dGroundWidth = Max(dist.x, dist.y) * 5;  //使场景和背景的比例显示恰当，背景宽度是Max(dist.x, dist.y) 的5倍
}

//鼠标跟踪事件
void scsGLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	//cout<<lastPos.x()<<"  "<<lastPos.y()<<endl;
	if (event->button() == Qt::LeftButton)
	{
		m_camera.beginDrag(lastPos.x(), lastPos.y());
	}
	else if (event->button() == Qt::RightButton)
	{
	}
	else if (event->button() == Qt::MidButton)
	{

	}
}

void scsGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		m_camera.mouseMove(event->pos().x(), event->pos().y());
		updateGL();
	}else if (event->buttons() & Qt::MidButton)  //支持图形移动功能
	{
		m_camera.setModelTranslate(event->pos().x() - lastPos.x(), event->pos().y() - lastPos.y());
		lastPos = event->pos();
		updateGL();
	}
}

void scsGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if ((event->button() == Qt::LeftButton) && (event->type() == QEvent::MouseButtonRelease))
		m_camera.endDrag(event->pos().x(), event->pos().y());

}

//通过鼠标滚轮实现图形缩放
void scsGLWidget::wheelEvent(QWheelEvent *event)
{
	double zoomValue = 1.3;
	//event->delta()当鼠标滑轮在滚动时用于返回滑动的距离，该值等于鼠标旋转角度的8倍。
	if ((event->delta()) > 0)   //正数值表示滑轮相对于用户在向前滑动
		m_camera.zoomIn(zoomValue);
	else if ((event->delta()) < 0)  //负数值表示滑轮相对于用户是向后滑动的。
		m_camera.zoomOut(zoomValue);
	updateGL();
}

//绘制坐标轴
void scsGLWidget::drawCoordinates()
{
	GLUquadricObj* quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_FALSE);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	int ww = width() / 4;
	if (ww > 128) ww = 128;
	glViewport(0, 0, ww + 20, (GLint)ww + 20);
	glOrtho(-24, 24, -24, 24, -24, 24);

	//画坐标轴
	glMultMatrixd(m_camera.getRotateMatrix());
	glColor3f(1.0, 0.0, 0.0);
	gluCylinder(quadratic, 1.0f, 1.0f, 12.0f, 32, 32);
	glTranslatef(0.0, 0.0, 12.0f);
	gluCylinder(quadratic, 2.0f, 0.0f, 4.0f, 32, 32);
	glRasterPos3f(0.0, 0.0, 8);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'z');
	glTranslatef(0.0, 0.0, -12.0f);
	glRotated(-90, 1.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	gluCylinder(quadratic, 1.0f, 1.0f, 12.0f, 32, 32);
	glTranslatef(0.0, 0.0, 12.0f);
	gluCylinder(quadratic, 2.0f, 0.0f, 4.0f, 32, 32);
	glRasterPos3f(0.0, 0.0, 8);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'y');
	glTranslatef(0.0, 0.0, -12.0f);
	glRotated(90, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	gluCylinder(quadratic, 1.0f, 1.0f, 12.0f, 32, 32);
	glTranslatef(0.0, 0.0, 12.0f);
	gluCylinder(quadratic, 2.0f, 0.0f, 4.0f, 32, 32);
	glRasterPos3f(0.0, 0.0, 8);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'x');

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glViewport(0, 0, (GLint)width(), (GLint)height());  //视口还原为原始的整个屏幕
	glDisable(GL_BLEND);
	gluDeleteQuadric(quadratic);
}