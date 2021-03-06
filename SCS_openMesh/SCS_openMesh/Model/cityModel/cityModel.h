#pragma once
#include "Model/abstractGlobalModel/abstractGlobalModel.h"
#include <cityModule/cityScene.h>
#include <GL/freeglut.h>


class cityModel :public abstractGlobalModel
{
public:
	cityModel(string jsonPath);
	cityModel(string mapName, vector<string> buidlingPath, vector<string> heightPath, string altitudePath);
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
	vector<Building> buildings;         //场景中的建筑物
	vector<Vector3d> upperface_points;  //场景上表面的点
	vector<Vector3d> underface_points;  //场景下底面的点
};

