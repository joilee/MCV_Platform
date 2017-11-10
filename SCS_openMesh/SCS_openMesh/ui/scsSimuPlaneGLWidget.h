//仿真面展示模块
#pragma once
#include "scsGLWidget.h"
#include "util/EField.h"

class scsSimuPlaneGLWidget:public scsGLWidget
{
public:
	scsSimuPlaneGLWidget(QWidget *parent = 0);

	~scsSimuPlaneGLWidget();

	void setGrid(vector<int> level, vector<int> vertical);

	void setSimPlane(vector<vector<EField>> &PlacePoint, vector<int> h, vector<int> v);

private:
	vector<int> horizonNum;
	vector<int> veticalNum;
	double Tmax;
	double Tmin;
	vector<vector<EField>> AP_EPoints;  //记录接收点处相关信息，以便仿真面的绘制
	vector<bool> sceneIsDislpay;   //是否显示场景仿真面结果
	double vis_factor_face;
	bool drawSimplaneFlag;
	void drawPlane();
	void drawColorbar();
};
