//�ֲ�����չʾģ��(�ʷֽ����

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
	//������Ϣ
	vector<scsMaterial> materials;
	int defaultMaterial;
	bool drawLocalPoint;
	bool drawLocalLine;
	bool drawLocalFace;

	int LocalModelID;
	double modelAlpha;
	void drawLocalScene();//�ֲ���Ϣ��ֻ��������Ƭ

	void paintGL();
};
