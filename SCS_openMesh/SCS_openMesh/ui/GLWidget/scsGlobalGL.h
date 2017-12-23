#pragma once
#include "scsGLWidget.h"
#include "Context/context.h"
#include <material/scsMaterial.h>

class scsGlobalGL:public scsGLWidget
{
public:
	scsGlobalGL(QWidget *parent = 0);
	void paintGL();

private:
	void drawCity();
};
