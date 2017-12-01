#pragma once
#include "scsGLWidget.h"
#include "Context/context.h"
struct  scsMaterial;

class scsLocalModelGLWidget:public scsGLWidget
{
public:
	scsLocalModelGLWidget(QWidget *parent = 0);
	~scsLocalModelGLWidget();
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
private:
	//������Ϣ
	vector<scsMaterial> materials;
	int defaultMaterial;
	bool drawLocalPoint;
	bool drawLocalLine;
	bool drawLocalFace;

	int LocalModelID;

	void drawLocalScene();//�ֲ���Ϣ��ֻ��������Ƭ

	void paintGL();
};

