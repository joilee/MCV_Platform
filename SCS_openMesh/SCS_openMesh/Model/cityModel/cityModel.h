#pragma once
#include "Model/abstractGlobalModel/abstractGlobalModel.h"
#include <cityModule/cityScene.h>


class cityModel :public abstractGlobalModel
{
public:
	cityModel(string jsonPath);
	~cityModel();
	void draw(vector<bool> mode,double alpha);
	void initDraw();
	double getAltitude(double x, double y);
	Vector3d getMinPoint();
	 Vector3d getMaxPoint();
	 int getBuildingSize();
	 int getConcaveSize();
	inline cityScene* getCity(){ return city; }
private:

	//����ģ�ͳ��� ���������� ����ģ��  �Լ����ߺϳɵ�������Ƭģ��

	//����ģ�ͺ���
	void LoadModel(string _name, vector<string> _v, vector<string> _h, string _p);

	//������ 
	cityScene *city;  //����Ľ�����ģ��
};

