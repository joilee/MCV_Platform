//������չʾģ��
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
	vector<vector<EField>> AP_EPoints;  //��¼���յ㴦�����Ϣ���Ա������Ļ���
	vector<bool> sceneIsDislpay;   //�Ƿ���ʾ������������
	double vis_factor_face;
	bool drawSimplaneFlag;
	void drawPlane();
	void drawColorbar();
	void LoadUniformColor(double currentVaule, Color &result);
};
