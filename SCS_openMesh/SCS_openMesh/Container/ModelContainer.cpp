#include "ModelContainer.h"
ModelContainer::ModelContainer()
{
	modelMap.clear();
}

ModelContainer::~ModelContainer()
{
}

void ModelContainer::insertModel(abstractModel* model)
{
	int id = model->getID();
	auto it = modelMap.find(id);
	if (it!=modelMap.end())
	{
		cout << "error: Model 已经存在，请重新检查站点文件的SiteName字段，重复字段为" << id << ". 插入失败，已经退出";
		return;
	}
	modelMap.insert(make_pair(id, model));
}

void ModelContainer::deleteModel(abstractModel* model)
{
	auto it = modelMap.begin();
	for (; it != modelMap.end();)
	{
		if (it->second == model)
		{
			modelMap.erase(it++);
			cout << "success: Model 已经删除" << endl;
			return;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: 容器中模型数量为" << modelMap.size() << endl;
	return;
}

void ModelContainer::deleteModel(string name)
{
	auto it = modelMap.begin();
	for (; it != modelMap.end();)
	{
		if (it->second->getName() == name)
		{
			modelMap.erase(it++);
			cout << "success: Model 已经删除" << endl;
			return;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: 容器中模型数量为" << modelMap.size() << endl;
	return;
}

void ModelContainer::deleteModel(int id)
{
	auto it = modelMap.find(id);
	if (it!=modelMap.end())
	{
		modelMap.erase(it);
		cout << "success: Model 已经删除 ,id是" <<id<< endl;
	}
	return;
}

bool ModelContainer::isLocalExist()
{
	auto it = modelMap.begin();
	for (; it != modelMap.end();it++)
	{
		if (isLocalType(it->first)) 
		{
			return true;
		}
	}
	return false;
}

bool ModelContainer::isLocalType(int id)
{
	auto it = modelMap.find(id);
	if (it!=modelMap.end())
	{
		ModelType a= it->second->getType();
		if (a==ModelType::CITY_LOCAL||a==ModelType::OBJ_LOCAL)
		{
			return true;
		}
	}
	return false;
}

bool ModelContainer::isWholeType(int id)
{
	auto it = modelMap.find(id);
	if (it != modelMap.end())
	{
		ModelType a = it->second->getType();
		if (a == ModelType::CITY || a == ModelType::OBJ)
		{
			return true;
		}
	}
	return false;
}

cityLocalModel * ModelContainer::getFirstLocal()
{
	auto it = modelMap.begin();
	while (it!=modelMap.end())
	{
		if (isLocalType(it->first))
		{
			return dynamic_cast<cityLocalModel*>(it->second);
		}
		it++;
	}
	return nullptr;
}

cityLocalModel * ModelContainer::getLocalModelByID(int id)
{
	auto it = modelMap.find(id);
	if (it!=modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityLocalModel))
		{
			return dynamic_cast<cityLocalModel*>(it->second);
		}
	}
	return nullptr;
}

bool ModelContainer::isCityExist()
{
	auto it = modelMap.begin();
	while (it!=modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityModel))
		{
			return true;
		}
		it++;
	}
	return false;
}

bool ModelContainer::isLocalIDExist(int id)
{
	auto it = modelMap.find(id);
	if (it != modelMap.end() && (typeid(*(it->second)) == typeid(cityLocalModel)))
	{
		return true;
	}
	return false;
}

cityModel* ModelContainer::getFirstCity()
{
	auto it = modelMap.begin();
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityModel))
		{
			return  dynamic_cast<cityModel*>(it->second);
		}
		it++;
	}
	return nullptr;
}

std::vector<int> ModelContainer::getIDs()
{
	auto it = modelMap.begin();
	vector<int> res;
	while (it!=modelMap.end())
	{
		res.push_back(it->first);
		it++;
	}
	return res;
}

std::vector<int> ModelContainer::getLocalCityModelID()
{
	auto it = modelMap.begin();
	vector<int> res;
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityLocalModel))
		{
			res.push_back(it->first);
		}
		it++;
	}
	return res;
}

std::vector<std::string> ModelContainer::getNames()
{
	auto it = modelMap.begin();
	vector<string> res;
	while (it != modelMap.end())
	{
		res.push_back(it->second->getName());
		it++;
	}
	return res;
}


/**
  * @Method:    getCityBoundingBox
  * @Note: 	获得最小最大的boundingbox,如果不存在返回空
  * @Author:    Li Gen
  * @ Date:     2017/11/29
  * @Returns:   std::vector<Vector3d>
*/
std::vector<Vector3d> ModelContainer::getCityBoundingBox()
{
	cityModel* model = getFirstCity();
	vector<Vector3d> res;
	if (model!=nullptr)
	{
		res.push_back(model->getCity()->getMinPoint());
		res.push_back(model->getCity()->getMaxPoint());
	}
	return res;
}

int ModelContainer::getCityBuildingNum()
{
	cityModel* model = getFirstCity();
	int res = 0;
	if (model!=nullptr)
	{
		res = model->getCity()->getBuildingSize();
	}
	return res;
}

int ModelContainer::getCityConcaveNum()
{
	cityModel* model = getFirstCity();
	int res = 0;
	if (model != nullptr)
	{
		res = model->getCity()->getConcaveNum();
	}
	return res;
}

int ModelContainer::getLocalFaceNum(int id/*=NO_MODEL_ID*/)
{
	if (id==NO_LOCAL_MODEL_ID)
	{
		return -1;
	}
	cityLocalModel *tmp = getLocalModelByID(id);
	if (tmp==nullptr)
	{
		return -1;
	}
	return tmp->getTriangleNum();
}

std::vector<Vector3d> ModelContainer::getLocalBountdingBox(int id /*= NO_MODEL_ID*/)
{
	vector<Vector3d> res; 
	if (id == NO_LOCAL_MODEL_ID)
	{
		return res;
	}
	cityLocalModel *tmp = getLocalModelByID(id);
	if (tmp == nullptr)
	{
		return res;
	}
	res.push_back(tmp->getMin());
	res.push_back(tmp->getMax());
	return res;
}
