//局部场景展示模块(剖分结果）

#pragma once
#include "ui/scsGLWidget.h"
#include "Context/context.h"
struct  scsMaterial;

class scsLocalGL:public scsGLWidget
{
public:
	scsLocalGL(QWidget *parent = 0);
	~scsLocalGL();
	void setMaterial(vector<scsMaterial> &materialdatabase)
	{
		materials = materialdatabase;
	}
	void setLocalModelID(int a)
	{
		LocalModelID = a;
	}
	void setPoint(bool a){ drawLocalPoint = a; }
	void setLine(bool a){ drawLocalLine = a; }
	void setFace(bool a){ drawLocalFace = a; }

	void setModelAlpha(int a);

private:
	//材料信息
	vector<scsMaterial> materials;
	int defaultMaterial;
	bool drawLocalPoint;
	bool drawLocalLine;
	bool drawLocalFace;

	int LocalModelID;
	double modelAlpha;
	void drawLocalScene();//局部信息，只有三角面片

	void paintGL();
};

