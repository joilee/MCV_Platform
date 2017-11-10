#include <gl/freeglut.h>
#include "glWidget.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <QtWidgets/QMessageBox>


//构造函数
GLWidget::GLWidget(QWidget *parent): QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	resetRenderColor();
	// Ground
	m_lightPos = Vector3d(0, 0.55,200);

	m_groundCenter = Vector3d(0, 0, 0);
	m_dGroundWidth=5;

	TriangleModel=NULL;
	vis_factor_face=1;
	
	minPos = Vector3d(DBL_MAX, DBL_MAX, DBL_MAX);
	maxPos = Vector3d(DBL_MIN,DBL_MIN,DBL_MIN);

	drawVectorScene=false;
	drawSimplaneFlag=false;
	vis_factor_scence =1;
	materials.clear();
	defaultMaterial=-1;
	drawLocalPoint = false;;
	drawLocalLine=false;
	drawLocalFace=false;
	setMouseTracking(true);
}


//绘制多边形的信息
void  CALLBACK vertexCallback(GLvoid *vertex)  //点坐标信息
{  
	const GLdouble *pointer = (GLdouble *) vertex;  
	//glColor3dv(pointer + 3);//在此设置颜色  
	glVertex3dv(pointer);  
}  
void CALLBACK beginCallback(GLenum which)  
{  
	glBegin(which);  
}  
void CALLBACK endCallback  ()  
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
							  GLfloat weight[4], GLdouble **dataOut )  
{  
	GLdouble *vertex;  
	int i;  
	vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));  
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

void GLWidget::initializeGL()
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
	GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	 //定义光照模型
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(m_backGroundColor.r , m_backGroundColor.g, m_backGroundColor.b, 0.0);   //设置当前清除颜色
	glShadeModel(GL_SMOOTH);  //平滑着色
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);		// 基于源象素alpha通道值的半透明混合函数

	glEnable(GL_CULL_FACE);//剔除背面

}


void GLWidget::paintGL()
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //用设定的当前清除值清除指定的缓冲区

		m_camera.setupModelMatrix();
		globalContext *globalCtx = globalContext::GetInstance();
		
		if (globalCtx->modelManager->drawLocalFlag())
		{
			glEnable(GL_BLEND);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_DEPTH_TEST);
			drawLocalScene();
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glDisable(GL_BLEND);
		}
		if (drawVectorScene)
		{
			drawAllScene();
		}
		if (drawSimplaneFlag)
		{
			drawPlane();
		}

		drawCoordinates();
		if (globalCtx->modelManager->drawLocalFlag())
		{
			//glutSwapBuffers();
			glFlush();
		}
		
}

void GLWidget::resizeGL(int width, int height)
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

GLWidget::~GLWidget()
{

}
void GLWidget::resetRenderColor()
{
	m_lightColor = Color::WHITE;
	m_backGroundColor =  Color::MediumSlateBlue;
	m_groundColor = Color::GRAY;
}

void GLWidget::drawAllScene()
{
	if (drawVectorScene)
	{
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();

		GLUtesselator *tobj = gluNewTess();  
		if (!tobj) 
		{   
			return;   
		}  
		gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);  
		gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);  
		gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);  
		//gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);  
		//gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);  

		glColor4d(1, 1, 1,vis_factor_scence);

		for(int buildings_id = 0; buildings_id < m_Buildings.size(); buildings_id++)
		{
			double building_height = m_Buildings[buildings_id].height; //建筑物的高度
			gluTessBeginPolygon(tobj, NULL); 
			//建筑物上顶面
			GLdouble (*a)[3] = new GLdouble[m_Buildings[buildings_id].upper_facePoint.size() -1][3];
			gluTessBeginContour(tobj); 
			for (int id = 0; id < m_Buildings[buildings_id].upper_facePoint.size()-1; id++) //记录building顶面点坐标时，首末点重合，记录两次，所以   .size（）-1
			{
				a[id][0] = m_Buildings[buildings_id].upper_facePoint[id].x;
				a[id][1] = m_Buildings[buildings_id].upper_facePoint[id].y;
				a[id][2] =	m_Buildings[buildings_id].upper_facePoint[id].z;	
				gluTessVertex(tobj, a[id], a[id]); 
			}
			gluTessEndContour(tobj); 
			gluTessEndPolygon(tobj);
			delete []a;

			gluTessBeginPolygon(tobj, NULL); 
			//建筑物下底面
			GLdouble (*b)[3] = new GLdouble[m_Buildings[buildings_id].upper_facePoint.size() -1][3];
			gluTessBeginContour(tobj); 
			for (int id = 0; id < m_Buildings[buildings_id].upper_facePoint.size()-1; id++)
			{
				b[id][0] = m_Buildings[buildings_id].upper_facePoint[id].x;
				b[id][1] = m_Buildings[buildings_id].upper_facePoint[id].y;
				b[id][2] = m_Buildings[buildings_id].upper_facePoint[id].z - building_height;	
				gluTessVertex(tobj, b[id], b[id]); 
			}
			gluTessEndContour(tobj); 
			gluTessEndPolygon(tobj);
			delete []b;

			//建筑物侧面
			glBegin(GL_QUADS);
			for (int id = 0; id < m_Buildings[buildings_id].upper_facePoint.size()-1; id++)
			{
				glVertex3d(m_Buildings[buildings_id].upper_facePoint[id].x,m_Buildings[buildings_id].upper_facePoint[id].y,m_Buildings[buildings_id].upper_facePoint[id].z);
				glVertex3d(m_Buildings[buildings_id].upper_facePoint[id+1].x,m_Buildings[buildings_id].upper_facePoint[id+1].y,m_Buildings[buildings_id].upper_facePoint[id+1].z);
				glVertex3d(m_Buildings[buildings_id].upper_facePoint[id+1].x,m_Buildings[buildings_id].upper_facePoint[id+1].y,m_Buildings[buildings_id].upper_facePoint[id+1].z - building_height);
				glVertex3d(m_Buildings[buildings_id].upper_facePoint[id].x,m_Buildings[buildings_id].upper_facePoint[id].y,m_Buildings[buildings_id].upper_facePoint[id].z - building_height);
			}
			glEnd();
		}	 
		gluDeleteTess(tobj);   
		glPopMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}

	return;
}


/************************************************************************/
/*  绘制场景
此处场景为局部
*/
/************************************************************************/
void GLWidget::drawLocalScene(){

	globalContext *globalCtx = globalContext::GetInstance();
	minPos = Min(minPos, globalCtx->modelManager->getFirstLocal()->getMin());
	maxPos = Max(maxPos, globalCtx->modelManager->getFirstLocal()->getMax());
	updateMesh();
	vector<bool> mode;
	mode.push_back(drawLocalPoint);
	mode.push_back(drawLocalLine);
	mode.push_back(drawLocalFace);
	globalCtx->modelManager->getFirstLocal()->draw(mode);
}

void GLWidget::updateMesh()
{
	m_camera.setSceneBBox(minPos, maxPos);
	// update ground attribute
	Vector3d dist = maxPos - minPos;
	m_groundCenter   = (minPos + maxPos) * .5;
	m_groundCenter.z = minPos.z;
	m_dGroundWidth = Max(dist.x, dist.y) * 5;  //使场景和背景的比例显示恰当，背景宽度是Max(dist.x, dist.y) 的5倍
}

//鼠标跟踪事件
void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	//cout<<lastPos.x()<<"  "<<lastPos.y()<<endl;
	if(event->button() == Qt::LeftButton)
	{
		m_camera.beginDrag(lastPos.x(), lastPos.y());
	}
	else if(event->button() == Qt::RightButton)
	{
	}
	else if(event->button() == Qt::MidButton)
	{
		
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	/*double length1=maxPos.x-minPos.x;
	double width1=maxPos.y-minPos.y;
	currentPos=QPoint(event->pos().x(),event->pos().y());
	double posx = (width()/2-currentPos.x())/length1;
	double posy = (currentPos.y() - height()/2)/width1;

	QString s1 = QString::number(posx);
	QString s2 = QString::number(posy);
	QString s = s1 + "," + s2;
	QPoint p = event->pos();
	p.setX( p.x());
	p.setY (p.y());
	QToolTip::showText(p, s);*/

	if(event->buttons() & Qt::LeftButton)
	{
		m_camera.mouseMove(event->pos().x(), event->pos().y());
		updateGL();
	}
	//else if(is_change_antennapos&&(event->buttons() & Qt::RightButton))
	//{
	//	flag_rightbutton=true;
	//	currentPos=QPoint(event->pos().x(),height()-event->pos().y());  //鼠标获取的窗口y坐标与实际窗口y坐标互补，故用height()减去获取的y值，窗口坐标原点在窗口左上角处
	//	updateGL();
	//}
 	else if(event->buttons() & Qt::MidButton)  //支持图形移动功能
	{
		m_camera.setModelTranslate(event->pos().x() - lastPos.x(), event->pos().y() - lastPos.y());	
		lastPos = event->pos();
		updateGL();
	}	  
}
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if ((event->button() == Qt::LeftButton) && (event->type() == QEvent::MouseButtonRelease)) 
		m_camera.endDrag(event->pos().x(), event->pos().y());
	
}

//通过鼠标滚轮实现图形缩放
void GLWidget::wheelEvent(QWheelEvent *event)
{
	double zoomValue = 1.3;
	//event->delta()当鼠标滑轮在滚动时用于返回滑动的距离，该值等于鼠标旋转角度的8倍。
	if((event->delta())>0)   //正数值表示滑轮相对于用户在向前滑动
		m_camera.zoomIn(zoomValue);
	else if((event->delta())<0)  //负数值表示滑轮相对于用户是向后滑动的。
		m_camera.zoomOut(zoomValue);
 	updateGL();	
}

/************************************************************************/
/* 设置顶点数组、颜色数组、法向量数组                                                     */
/************************************************************************/

void GLWidget::setTriangleModel(emxModel* TriangleData)
{
	TriangleModel = TriangleData; 

	TriangleData->GetBBox(minPos,maxPos);
	//added by ligen  2016/12/12
	//根据材质信息来确定颜色数组
		bool materialFindFlag=false;
		int faceNum=TriangleData->NumF();

		indices.clear();
		colorVector.clear();
		vertices.clear();
	


		//设置点
		 for (int i=0;i<TriangleData->NumV();i++)
		 {
			Vector3d tmp = TriangleModel->GetVertex(i);
			vertices.push_back(tmp[0]);
			vertices.push_back(tmp[1]);
			vertices.push_back(tmp[2]);
		 }


		map<int,int>vertex2material;
		for (int i=0;i<TriangleData->NumV();i++)
		{
			 vertex2material.insert(make_pair(i,defaultMaterial));
		}
		//两层循环，设置颜色和索引
		for (size_t i = 0; i < faceNum; ++i)
		{
			Vector3i f=TriangleData->GetFace(i);
			indices.push_back(f[0]);
			indices.push_back(f[1]);
			indices.push_back(f[2]);


			for(int j = 0;j < materials.size();j++)
			{
				if (materials[j].Id == TriangleModel->getMtlId(i))
				{
				   vertex2material[f[0]]=j;
				   vertex2material[f[1]]=j;
				   vertex2material[f[2]]=j;

					materialFindFlag=true;
					break;//有一个等于即可，颜色信息是相同的
				}
			}

		}
	  if (vertex2material.size()!=TriangleData->NumV())
	  {
		  return;
	  }
	  map<int,int>::iterator it=vertex2material.begin();
	  for (;it!=vertex2material.end();it++)
	  {
		  colorVector.push_back(materials[it->second].color.r/256.f);
		  colorVector.push_back(materials[it->second].color.g/256.f);
		  colorVector.push_back(materials[it->second].color.b/256.f);
		  colorVector.push_back(vis_factor_scence);
	  }
	  glGenBuffers(1, &vertexBufferID);  
	  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);  
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW); 
	  glGenBuffers(1, &colorBufferID);
	  glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colorVector.size(), &colorVector[0], GL_STATIC_DRAW); 
	
	 // drawTriangleScene = true;
}

void GLWidget::drawPlane()
{
	
	glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL );
	for (int id=0;id<AP_EPoints.size();id++)
	{
		if (sceneIsDislpay[id])  //选中的小区才能显示仿真结果
		{
			vector<EField> &EPoint = AP_EPoints[id];
			double emax = EPoint[0].MolStrength;
			double emin = EPoint[0].MolStrength;
			for(size_t m = 1; m < EPoint.size(); m++)
			{

				if (EPoint[m].pathsize> 0||EPoint[m].Path.size()>0) //若EPoint[i].pathsize == 0 说明，此接收点没有路径到达，信号强度值未知
				{
					if(EPoint[m].MolStrength > emax)
					{
						emax = EPoint[m].MolStrength;
					}
					if(EPoint[m].MolStrength < emin)
					{
						emin = EPoint[m].MolStrength;
					}
				}
			}
		//	cout<<"the minimal point id is:"<<min_id<<endl;
			for (size_t n = 0; n< EPoint.size();n++)
			{
				if (EPoint[n].pathsize== 0&&EPoint[n].Path.size()==0) //对于没有路径到达的接收点，信号强度设为最小值
				{
					EPoint[n].MolStrength = emin;
				}
			}
			Tmax = emax;
			Tmin = emin;

			//更新场景的数据
			minPos=EPoint[0].Position;
			maxPos=EPoint[EPoint.size()-1].Position;
			updateMesh();
			drawColorbar();

			double length = emax - emin;
			glEnable(GL_BLEND); //启用混合
			glPushMatrix();
			glBegin(GL_QUADS);

			for(int i = 0; i < veticalNum[id]-1; i++)
			{
				for(int j = 0; j < horizonNum[id]-1; j++)
				{
					Vector3d v1 = EPoint[j*veticalNum[id] + i].Position;
					Vector3d v4 = EPoint[j*veticalNum[id] + i + 1].Position;
					Vector3d v2 = EPoint[(j+1)*veticalNum[id] + i].Position;
					Vector3d v3 = EPoint[(j+1)*veticalNum[id] + i + 1].Position;
					double c1 = (EPoint[j*veticalNum[id] + i].MolStrength - emin)/length;
					double c4 = (EPoint[j*veticalNum[id] + i +1].MolStrength - emin)/length;
					double c2 = (EPoint[(j+1)*veticalNum[id] + i].MolStrength - emin)/length;
					double c3 = (EPoint[(j+1)*veticalNum[id] + i + 1].MolStrength - emin)/length;

					Color result(0.5, 0.0, 0.0); 

					LoadUniformColor(c1,result);
					glColor4d(result.r, result.g, result.b, vis_factor_face);
					glVertex3d(v1.x, v1.y, v1.z);
					LoadUniformColor(c2,result);
					glColor4d(result.r, result.g, result.b, vis_factor_face);
					glVertex3d(v2.x, v2.y, v2.z);
					LoadUniformColor(c3,result);
					glColor4d(result.r, result.g, result.b, vis_factor_face);
					glVertex3d(v3.x, v3.y, v3.z);
					LoadUniformColor(c4,result);
					glColor4d(result.r, result.g, result.b, vis_factor_face);
					glVertex3d(v4.x, v4.y, v4.z);
				}
			}
			glEnd();
		
			glPopMatrix();
			glDisable(GL_BLEND);
		}		
	}
}

void GLWidget::LoadUniformColor(double currentVaule,Color &result)
{ 
	if(currentVaule <= 0.2) 
		result = Color(0.0,currentVaule * 5,1.0);
	else if(currentVaule <= 0.4)
		result = Color(0.0, 1.0, (0.4 - currentVaule) * 5);
	else if(currentVaule <= 0.6)
		result = Color((currentVaule - 0.4) * 5,1.0,0.0);
	else if(currentVaule <= 0.8)
		result = Color(1.0, (0.8 - currentVaule) * 2.5 + 0.5, 0.0);
	else if(currentVaule <= 1.0)
		result = Color(1.0,(1.0 - currentVaule) * 2.5,0.0);

}

//绘制坐标轴
void GLWidget::drawCoordinates()
{
	GLUquadricObj* quadratic=gluNewQuadric(); 
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
	gluCylinder(quadratic, 1.0f, 1.0f, 12.0f ,32, 32);
	glTranslatef(0.0, 0.0, 12.0f); 
	gluCylinder(quadratic, 2.0f, 0.0f, 4.0f, 32, 32);
	glRasterPos3f (0.0, 0.0, 8);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'z');
	glTranslatef(0.0, 0.0, -12.0f);
	glRotated(-90, 1.0, 0.0, 0.0); 
	glColor3f( 0.0, 1.0, 0.0 );
	gluCylinder(quadratic, 1.0f, 1.0f, 12.0f ,32, 32); 
	glTranslatef(0.0, 0.0, 12.0f);
	gluCylinder(quadratic, 2.0f, 0.0f, 4.0f, 32, 32);
	glRasterPos3f (0.0, 0.0, 8); 
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'y'); 
	glTranslatef(0.0, 0.0, -12.0f); 
	glRotated(90, 0.0, 1.0, 0.0); 
	glColor3f(0.0, 0.0, 1.0); 
	gluCylinder(quadratic, 1.0f, 1.0f, 12.0f ,32, 32); 
	glTranslatef(0.0, 0.0, 12.0f); 
	gluCylinder(quadratic, 2.0f, 0.0f, 4.0f, 32, 32);
	glRasterPos3f (0.0, 0.0, 8); 
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'x');

	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 

	glViewport(0, 0, (GLint)width(), (GLint)height());  //视口还原为原始的整个屏幕
	glDisable(GL_BLEND);
	gluDeleteQuadric(quadratic); 
}

void GLWidget::drawColorbar()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
    
	int viewWidth = width() / 10;
	if (viewWidth>80) viewWidth = 80;
	int viewheight = height() / 2;

	glViewport(width() - viewWidth - 20, height() - viewheight - 20, viewWidth, viewheight);  //视口（窗口）变换
	gluOrtho2D(0, viewWidth, 0, viewheight);  //正投影

	int cx = 0,cy = 0,cw = viewWidth / 4,ch = viewheight;
	
	// Draw Color Bar 6种颜色 
	cy = ch;
	ch = - ch;

	glBegin (GL_QUAD_STRIP);      //

	glColor3f( 1.0 ,0.0 ,0.0 ) ;	   //红 
	glVertex2i(cx+cw,cy);
	glVertex2i(cx,cy);

	glColor3f(  1.0 ,0.5 ,0.0 );	   //桔黄
	glVertex2i(cx+cw,cy+ch/5);
	glVertex2i(cx,cy+ch/5);

	glColor3f( 1.0 ,1.0 ,0.0);	   //黄
	glVertex2i(cx+cw,cy+2*ch/5);
	glVertex2i(cx,cy+2*ch/5);

	glColor3f( 0.0 ,1.0 ,0.0);	   //绿
	glVertex2i(cx+cw,cy+3*ch/5);
	glVertex2i(cx,cy+3*ch/5);

	glColor3f( 0.0 ,1.0 ,1.0);	   //青 
	glVertex2i(cx+cw,cy+4*ch/5);
	glVertex2i(cx,cy+4*ch/5);

	glColor3f( 0.0 ,0.0 ,1.0);	   //蓝
	glVertex2i(cx+cw,cy+5*ch/5);
	glVertex2i(cx,cy+5*ch/5);

	glEnd(); 

	// Draw the outline of color bar
	glColor3ub(0,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(cx,cy);
	glVertex2i(cx+cw,cy);
	glVertex2i(cx+cw,cy+ch);
	glVertex2i(cx,cy+ch);
	glEnd();

	// Draw the coordinate values
	double y;
	double strength[6];
	for(int i=0;i<=5;i++)
	{
		y=cy+i*ch/5.0;
		glBegin(GL_LINES);
		glVertex2i(cx+cw,y);
		glVertex2i(cx+cw+1,y);
		glEnd();
		
		strength[i] = Tmax - (Tmax - Tmin)*i/5.0;

		char* format;
		format = new char[30]; 
		
		/*
		函数gcvt(double number,size_t ndigits,char *buf)，把浮点数number转换成字符串(包含小数点和正负符号)，
		参数ndigits表示需显示的位数(仅包含数字),并返回指向该字符串的指针buf
		*/
		if(strength[i]>=-1000 && strength[i]<-100)
		{
			gcvt(strength[i], 5, format);  
		}
		else if(strength[i]>=-100 && strength[i]<-10)
		{
			gcvt(strength[i], 4, format);
		}
		else if (strength[i]>=-10 && strength[i]<10)
		{
			gcvt(strength[i], 3, format);
		}
		else if (strength[i]>=10 && strength[i]<100)
		{
			gcvt(strength[i], 4, format);
		}
		else if (strength[i]>=100 && strength[i]<1000)
		{
			gcvt(strength[i], 5, format);
		}
		else if (strength[i]>=1000 && strength[i]<10000)
		{
			gcvt(strength[i], 6, format);
		}
		else if (strength[i]>=10000 && strength[i]<100000)
		{
			gcvt(strength[i], 7, format);
		}

		char *s;
		glRasterPos2f(cx+cw+2,y);
		for(s = format; *s; ++s)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *s);
		delete[] format;
	}
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix();
	glViewport(0, 0, (GLint)width(), (GLint)height()); //视口还原为原始的整个屏幕
}