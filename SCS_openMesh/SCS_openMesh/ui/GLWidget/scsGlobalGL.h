#pragma once
#include "ui/scsGLWidget.h"
#include "Context/context.h"
#include <material/scsMaterial.h>

class scsGlobalGL:public scsGLWidget
{
public:
	scsGlobalGL(QWidget *parent = 0);
	void paintGL();

	void setPoint(bool a){ drawLocalPoint = a; }
	void setLine(bool a){ drawLocalLine = a; }
	void setFace(bool a){ drawLocalFace = a; }

private:
	void drawCity();
	bool drawLocalPoint;
	bool drawLocalLine;
	bool drawLocalFace;
	
};
