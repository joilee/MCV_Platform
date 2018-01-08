#pragma once
#include "ui/scsGLWidget.h"
#include "Context/context.h"
#include <material/scsMaterial.h>

class scsPlaneGL :public scsGLWidget
{
public:
	scsPlaneGL(QWidget *parent = 0);
	void paintGL();

private:
	//************************************
	// Method:    initData
	// FullName:  scsPlaneGL::initData
	// Access:    private 
	// Returns:   void
	// Qualifier: �������µ�pci�����³���opengl����
	//************************************
	void initData();

	void drawColorBar();
	void drawPlane();
	void drawSite();

	void setPCI(int a);
	void setAlpha(double a);
	void setModelAlpha(double a);
	void setModel();
	void LoadUniformColor(double currentVaule, Color &result);

	int pci;
	int model;
	Cell_Data *cell;
	double Tmax;
	double Tmin;
	double vis_factor_face;//������͸����
	double model_alpha;
};
