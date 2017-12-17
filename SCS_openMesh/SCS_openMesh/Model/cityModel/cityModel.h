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

	//城市模型场景 包括建筑物 地面模型  以及两者合成的三角面片模型

	//导入模型函数
	void LoadModel(string _name, vector<string> _v, vector<string> _h, string _p);

	//建筑物 
	cityScene *city;  //整体的建筑物模型
};

