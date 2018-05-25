#include "modelPara.h"
ModelPara::ModelPara()
{
	clearModel();
	materialdatabase.clear();
}

ModelPara::~ModelPara()
{

}

BaseModel* ModelPara::getModelById(int id)
{
	for (int i = 0; i < SiteModels.size();i++)
	{
		if (SiteModels[i]->getModelID() == id)
			return SiteModels[i];
	}
	return NULL;
}

