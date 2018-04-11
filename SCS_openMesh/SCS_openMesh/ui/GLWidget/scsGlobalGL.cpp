#include "scsGlobalGL.h"

scsGlobalGL::scsGlobalGL(QWidget *parent /*= 0*/)
{
	drawLocalPoint = false;
	drawLocalLine = true;
	drawLocalFace = true;

}

void scsGlobalGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //用设定的当前清除值清除指定的缓冲区

	m_camera.setupModelMatrix();

	drawCoordinates();
	globalContext *globalCtx = globalContext::GetInstance();
	if (globalCtx->modelManager->checkCityExist())
	{
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		drawCity();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_BLEND);
	}
}

void scsGlobalGL::drawCity()
{
	globalContext *globalCtx = globalContext::GetInstance();
	abstractGlobalModel *tmpModel = globalCtx->modelManager->getFirstCity();
	minPos = tmpModel->getMinPoint();
	maxPos = tmpModel->getMaxPoint();
	updateMesh();
	vector<bool> mode;
	mode.push_back(drawLocalPoint);
	mode.push_back(drawLocalLine);
	mode.push_back(drawLocalFace);
	tmpModel->draw(mode, 1);
}

