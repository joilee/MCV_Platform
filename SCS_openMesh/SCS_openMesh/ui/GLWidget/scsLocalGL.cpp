#include "scsLocalGL.h"

scsLocalGL::scsLocalGL(QWidget *parent /*= 0*/) :scsGLWidget(parent)
{
	LocalModelID = -111;
	materials.clear();
	defaultMaterial = 10;
	drawLocalPoint = false;;
	drawLocalLine = true;
	drawLocalFace = true;
	modelAlpha = 1.0;
}

scsLocalGL::~scsLocalGL()
{

}

void scsLocalGL::setModelAlpha(int a)
{
	modelAlpha =( (double)a )/ 100.0;
}

void scsLocalGL::drawLocalScene()
{
	globalContext *globalCtx = globalContext::GetInstance();
	minPos = globalCtx->modelManager->getLocalModelByID(LocalModelID)->getMin();
	maxPos = globalCtx->modelManager->getLocalModelByID(LocalModelID)->getMax();
	updateMesh();
	vector<bool> mode;
	mode.push_back(drawLocalPoint);
	mode.push_back(drawLocalLine);
	mode.push_back(drawLocalFace);
	globalCtx->modelManager->getLocalModelByID(LocalModelID)->draw(mode,modelAlpha);
}

void scsLocalGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //���趨�ĵ�ǰ���ֵ���ָ���Ļ�����

	m_camera.setupModelMatrix();

	drawCoordinates();

	globalContext *globalCtx = globalContext::GetInstance();
	LocalModelID = globalCtx->modelManager->getLocalShowID();
	if (globalCtx->modelManager->checkLocalExistByID(LocalModelID))
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
}
