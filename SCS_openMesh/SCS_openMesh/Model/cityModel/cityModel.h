#pragma once
#include "../abstractModel.h"
#include <cityModule/cityScene.h>


class cityModel :public abstractModel
{
public:
	cityModel(string jsonPath);
	~cityModel();
	void draw(vector<bool> mode);
	void initDraw();
	inline cityScene* getCity(){ return city; }
private:

	//����ģ�ͳ��� ���������� ����ģ��  �Լ����ߺϳɵ�������Ƭģ��

	//����ģ�ͺ���
	void LoadModel(string _name, vector<string> _v, vector<string> _h, string _p);

	//������ 
	cityScene *city;  //����Ľ�����ģ��
};

