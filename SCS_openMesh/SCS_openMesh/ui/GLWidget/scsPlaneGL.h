#pragma once
#include "ui/scsGLWidget.h"
#include "Context/context.h"
#include <material/scsMaterial.h>

class scsPlaneGL :public scsGLWidget
{
public:
	scsPlaneGL(QWidget *parent = 0);
	void paintGL();
	void setPCI(int site, int pci);
	void setAlpha(double a);
	void setModelAlpha(double a);
	void setModel();

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


	void LoadUniformColor(double currentVaule, Color &result);

	int pci;
	int site;
	int model;
	Cell_Data *cell;
	double Tmax;
	double Tmin;
	double vis_factor_face;//������͸����
	double model_alpha;
};
