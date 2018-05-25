#include "scsPlaneGL.h"

scsPlaneGL::scsPlaneGL(QWidget *parent /*= 0*/)
{
	plane_alpha = 1.0;
	model_alpha = 1.0;
	Tmax = 100;
	Tmin = 0;
	pci = -1;
	site = -1;
	model = -1;
}

void scsPlaneGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //用设定的当前清除值清除指定的缓冲区
	m_camera.setupModelMatrix();
	drawCoordinates();
	drawPlane();
	drawSite();
}

void scsPlaneGL::initData()
{
	globalContext *globalCtx = globalContext::GetInstance();
	EFieldContainer *container = globalCtx->visualManager->getContainer();
	if (container->isPCIExist(pci))
	{
		cell = container->getCellDataByPCI(pci);
		int siteID = container->getSiteIDByPCI(pci);
		assert(siteID == site);
		abstractLocalModel *tmpModel = globalCtx->modelManager->getLocalModelByID(siteID);
		maxPos = tmpModel->getMax();
		minPos = tmpModel->getMin();
		updateMesh();

		double emax = cell->efildVec[0]->MolStrength;
		double emin = cell->efildVec[0]->MolStrength;

		for (size_t m = 1; m < cell->efildVec.size(); m++)
		{
			if (cell->efildVec[m]->pathsize>0 || cell->efildVec[m]->Path.size() > 0)
			{
				if (cell->efildVec[m]->MolStrength > emax)
				{
					emax = cell->efildVec[m]->MolStrength;
				}
				if (cell->efildVec[m]->MolStrength < emin)
				{
					emin = cell->efildVec[m]->MolStrength;
				}
			}
		}
		for (size_t n = 0; n < cell->efildVec.size(); n++)
		{
			if (cell->efildVec[n]->pathsize == 0 && cell->efildVec[n]->Path.size() == 0)
			{
				cell->efildVec[n]->MolStrength = emin;
			}
		}
		Tmax = emax;
		Tmin = emin;


	}
	else
	{

	}


}

void scsPlaneGL::drawPlane()
{
	globalContext *globalCtx = globalContext::GetInstance();
	EFieldContainer *container = globalCtx->visualManager->getContainer();
	if (container->isPCIExist(pci))
	{

		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		//更新场景的数据
		//minPos = cell->efildVec[0]->Position;
		//maxPos = cell->efildVec[cell->efildVec.size() - 1]->Position;
		//updateMesh();
		drawColorBar();

		double length = Tmax - Tmin;
		glEnable(GL_BLEND);
		glPushMatrix();
		glBegin(GL_QUADS);

		int row = cell->row;
		int col = cell->col;
		for (int i = 0; i < cell->row - 1; i++)
		{
			for (int j = 0; j < cell->col - 1; j++)
			{
				Vector3d v1 = cell->efildVec[i*row + j]->Position;
				Vector3d v2 = cell->efildVec[i*row + j + 1]->Position;
				Vector3d v3 = cell->efildVec[(i + 1)*row + j]->Position;
				Vector3d v4 = cell->efildVec[(i + 1)*row + j + 1]->Position;

				double c1 = (cell->efildVec[i*row + j]->MolStrength - Tmin) / length;
				double c4 = (cell->efildVec[i*row + j + 1]->MolStrength - Tmin) / length;
				double c2 = (cell->efildVec[(i + 1)*row + j]->MolStrength - Tmin) / length;
				double c3 = (cell->efildVec[(i + 1)*row + j + 1]->MolStrength - Tmin) / length;

				Color result(0.5, 0.0, 0.0);				
				LoadUniformColor(c3, result);
				glColor4d(result.r, result.g, result.b, plane_alpha);
				glVertex3d(v3.x, v3.y, v3.z);
				LoadUniformColor(c4, result);
				glColor4d(result.r, result.g, result.b, plane_alpha);
				glVertex3d(v4.x, v4.y, v4.z);
				LoadUniformColor(c2, result);
				glColor4d(result.r, result.g, result.b, plane_alpha);
				glVertex3d(v2.x, v2.y, v2.z);
				LoadUniformColor(c1, result);
				glColor4d(result.r, result.g, result.b, plane_alpha);
				glVertex3d(v1.x, v1.y, v1.z);
			}
		}
		glEnd();

		glPopMatrix();
		glDisable(GL_BLEND);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_BLEND);
	}	
}

void scsPlaneGL::LoadUniformColor(double currentVaule, Color &result)
{
	if (currentVaule <= 0.2)
		result = Color(0.0, currentVaule * 5, 1.0);
	else if (currentVaule <= 0.4)
		result = Color(0.0, 1.0, (0.4 - currentVaule) * 5);
	else if (currentVaule <= 0.6)
		result = Color((currentVaule - 0.4) * 5, 1.0, 0.0);
	else if (currentVaule <= 0.8)
		result = Color(1.0, (0.8 - currentVaule) * 2.5 + 0.5, 0.0);
	else if (currentVaule <= 1.0)
		result = Color(1.0, (1.0 - currentVaule) * 2.5, 0.0);
}

void scsPlaneGL::setPCI(int siteID,int PCI)
{
	pci = PCI;
	site = siteID;
	initData();
	setModel();
}

void scsPlaneGL::drawColorBar()
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

void scsPlaneGL::setPlaneAlpha(double a)
{
	plane_alpha = ((double)a) / 100.0;
}

void scsPlaneGL::setModelAlpha(double a)
{
	model_alpha = ((double)a) / 100.0;
}

void scsPlaneGL::setModel()
{
	globalContext *globalCtx = globalContext::GetInstance();
	EFieldContainer *container = globalCtx->visualManager->getContainer();
	if (container->isPCIExist(pci))
	{
		model= container->getSiteIDByPCI(pci);	//siteID == modelID
	}
	else
	{
		model = -1;
	}

}

void scsPlaneGL::drawSite()
{
	globalContext *globalCtx = globalContext::GetInstance();
	vector<bool> mode;
	mode.push_back(false);
	mode.push_back(false);
	mode.push_back(true);
	if (globalCtx->modelManager->checkLocalExistByID(model))
	{
		glEnable(GL_BLEND);
		globalCtx->modelManager->getLocalModelByID(model)->draw(mode, model_alpha);
		glDisable(GL_BLEND);
	}
}


