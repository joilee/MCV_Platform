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
	inline string  getName(){ return name; }
	inline cityScene* getCity(){ return city; }
private:
	string name;
	
	//城市模型场景 包括建筑物 地面模型  以及两者合成的三角面片模型

	//导入模型函数
	void LoadModel(string _name, vector<string> _v, vector<string> _h, string _p);

	//建筑物 
	cityScene *city;  //整体的建筑物模型

	//材料


};

