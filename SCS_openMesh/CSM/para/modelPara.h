#ifndef MODELPARA_H
#define  MODELPARA_H
#include "baseModel.h"

class ModelPara
{
public:
	ModelPara();
	~ModelPara();

public:


	void setMaterialVector(vector<scsMaterial> a){ materialdatabase = a; }
	vector<scsMaterial> & getMaterialVector(){ return materialdatabase; }
	
	void setModels(vector<BaseModel*> &a){ SiteModels = a; }
	void addModel(BaseModel* a){ SiteModels.push_back(a); }
	void clearModel(){ SiteModels.clear(); }
public:

	//材料数组，来自于文件
	vector<scsMaterial> materialdatabase;
	vector<BaseModel*> SiteModels;

};





#endif