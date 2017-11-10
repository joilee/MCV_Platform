#include "scsSimuPlaneGLWidget.h"
#include <gl/freeglut.h>
scsSimuPlaneGLWidget::scsSimuPlaneGLWidget(QWidget *parent = 0) :scsGLWidget(parent) {
    horizonNum.clear();
    veticalNum.clear();
    Tmax = 100;
    Tmin = 0;
    AP_EPoints.clear();
    sceneIsDislpay.clear();
    drawSimplaneFlag = false;
	vis_factor_face = 1;
}

void scsSimuPlaneGLWidget::setGrid(vector<int> level, vector<int> vertical) {
    horizonNum = level;
    veticalNum = vertical;
    for (int i = 0; i < horizonNum.size(); i++) {
        horizonNum[i]++;
    }
    for (int i = 0; i < veticalNum.size(); i++) {
        veticalNum[i]++;
    }
}

void scsSimuPlaneGLWidget::setSimPlane(vector<vector<EField>> &PlacePoint, vector<int> h, vector<int> v) {
    AP_EPoints = PlacePoint;
    setGrid(h, v);
    sceneIsDislpay.push_back(true);
    drawSimplaneFlag = true;
}

void scsSimuPlaneGLWidget::drawPlane() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int id = 0; id<AP_EPoints.size(); id++)
	{
		if (sceneIsDislpay[id])  //选中的小区才能显示仿真结果
		{
			vector<EField> &EPoint = AP_EPoints[id];
			double emax = EPoint[0].MolStrength;
			double emin = EPoint[0].MolStrength;
			for (size_t m = 1; m < EPoint.size(); m++)
			{

				if (EPoint[m].pathsize> 0 || EPoint[m].Path.size()>0) //若EPoint[i].pathsize == 0 说明，此接收点没有路径到达，信号强度值未知
				{
					if (EPoint[m].MolStrength > emax)
					{
						emax = EPoint[m].MolStrength;
					}
					if (EPoint[m].MolStrength < emin)
					{
						emin = EPoint[m].MolStrength;
					}
				}
			}
			//	cout<<"the minimal point id is:"<<min_id<<endl;
			for (size_t n = 0; n < EPoint.size(); n++)
			{
				if (EPoint[n].pathsize == 0 && EPoint[n].Path.size() == 0) //对于没有路径到达的接收点，信号强度设为最小值
				{
					EPoint[n].MolStrength = emin;
				}
			}
			Tmax = emax;
			Tmin = emin;

			//更新场景的数据
			minPos = EPoint[0].Position;
			maxPos = EPoint[EPoint.size() - 1].Position;
			updateMesh();
			drawColorbar();

			double length = emax - emin;
			glEnable(GL_BLEND); //启用混合
			glPushMatrix();
			glBegin(GL_QUADS);

			for (int i = 0; i < veticalNum[id] - 1; i++)
			{
				for (int j = 0; j < horizonNum[id] - 1; j++)
				{
					Vector3d v1 = EPoint[j*veticalNum[id] + i].Position;
					Vector3d v4 = EPoint[j*veticalNum[id] + i + 1].Position;
					Vector3d v2 = EPoint[(j + 1)*veticalNum[id] + i].Position;
					Vector3d v3 = EPoint[(j + 1)*veticalNum[id] + i + 1].Position;
					double c1 = (EPoint[j*veticalNum[id] + i].MolStrength - emin) / length;
					double c4 = (EPoint[j*veticalNum[id] + i + 1].MolStrength - emin) / length;
					double c2 = (EPoint[(j + 1)*veticalNum[id] + i].MolStrength - emin) / length;
					double c3 = (EPoint[(j + 1)*veticalNum[id] + i + 1].MolStrength - emin) / length;

					Color result(0.5, 0.0, 0.0);

					LoadUniformColor(c1, result);
					glColor4d(result.r, result.g, result.b, vis_factor_face);
					glVertex3d(v1.x, v1.y, v1.z);
					LoadUniformColor(c2, result);
					glColor4d(result.r, result.g, result.b, vis_factor_face);
					glVertex3d(v2.x, v2.y, v2.z);
					LoadUniformColor(c3, result);
					glColor4d(result.r, result.g, result.b, vis_factor_face);
					glVertex3d(v3.x, v3.y, v3.z);
					LoadUniformColor(c4, result);
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

void scsSimuPlaneGLWidget::drawColorbar()
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

	int cx = 0, cy = 0, cw = viewWidth / 4, ch = viewheight;

	// Draw Color Bar 6种颜色 
	cy = ch;
	ch = -ch;

	glBegin(GL_QUAD_STRIP);      //

	glColor3f(1.0, 0.0, 0.0);	   //红 
	glVertex2i(cx + cw, cy);
	glVertex2i(cx, cy);

	glColor3f(1.0, 0.5, 0.0);	   //桔黄
	glVertex2i(cx + cw, cy + ch / 5);
	glVertex2i(cx, cy + ch / 5);

	glColor3f(1.0, 1.0, 0.0);	   //黄
	glVertex2i(cx + cw, cy + 2 * ch / 5);
	glVertex2i(cx, cy + 2 * ch / 5);

	glColor3f(0.0, 1.0, 0.0);	   //绿
	glVertex2i(cx + cw, cy + 3 * ch / 5);
	glVertex2i(cx, cy + 3 * ch / 5);

	glColor3f(0.0, 1.0, 1.0);	   //青 
	glVertex2i(cx + cw, cy + 4 * ch / 5);
	glVertex2i(cx, cy + 4 * ch / 5);

	glColor3f(0.0, 0.0, 1.0);	   //蓝
	glVertex2i(cx + cw, cy + 5 * ch / 5);
	glVertex2i(cx, cy + 5 * ch / 5);

	glEnd();

	// Draw the outline of color bar
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(cx, cy);
	glVertex2i(cx + cw, cy);
	glVertex2i(cx + cw, cy + ch);
	glVertex2i(cx, cy + ch);
	glEnd();

	// Draw the coordinate values
	double y;
	double strength[6];
	for (int i = 0; i <= 5; i++)
	{
		y = cy + i*ch / 5.0;
		glBegin(GL_LINES);
		glVertex2i(cx + cw, y);
		glVertex2i(cx + cw + 1, y);
		glEnd();

		strength[i] = Tmax - (Tmax - Tmin)*i / 5.0;

		char* format;
		format = new char[30];

		/*
		函数gcvt(double number,size_t ndigits,char *buf)，把浮点数number转换成字符串(包含小数点和正负符号)，
		参数ndigits表示需显示的位数(仅包含数字),并返回指向该字符串的指针buf
		*/
		if (strength[i] >= -1000 && strength[i]<-100)
		{
			gcvt(strength[i], 5, format);
		}
		else if (strength[i] >= -100 && strength[i]<-10)
		{
			gcvt(strength[i], 4, format);
		}
		else if (strength[i] >= -10 && strength[i]<10)
		{
			gcvt(strength[i], 3, format);
		}
		else if (strength[i] >= 10 && strength[i]<100)
		{
			gcvt(strength[i], 4, format);
		}
		else if (strength[i] >= 100 && strength[i]<1000)
		{
			gcvt(strength[i], 5, format);
		}
		else if (strength[i] >= 1000 && strength[i]<10000)
		{
			gcvt(strength[i], 6, format);
		}
		else if (strength[i] >= 10000 && strength[i]<100000)
		{
			gcvt(strength[i], 7, format);
		}

		char *s;
		glRasterPos2f(cx + cw + 2, y);
		for (s = format; *s; ++s)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *s);
		delete[] format;
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glViewport(0, 0, (GLint)width(), (GLint)height()); //视口还原为原始的整个屏幕
}
