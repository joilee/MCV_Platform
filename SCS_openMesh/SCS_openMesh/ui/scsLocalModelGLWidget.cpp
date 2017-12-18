#include "scsLocalModelGLWidget.h"



scsLocalModelGLWidget::scsLocalModelGLWidget(QWidget *parent /*= 0*/) :scsGLWidget(parent)
{
	LocalModelID = -111;
	materials.clear();
	defaultMaterial = 10;
	drawLocalPoint = false;;
	drawLocalLine = false;
	drawLocalFace = false;
}

scsLocalModelGLWidget::~scsLocalModelGLWidget()
{

}

void scsLocalModelGLWidget::setModelAlpha(int a)
{
	modelAlpha =( (double)a )/ 100.0;
}

void scsLocalModelGLWidget::drawLocalScene()
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

void scsLocalModelGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //用设定的当前清除值清除指定的缓冲区

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
