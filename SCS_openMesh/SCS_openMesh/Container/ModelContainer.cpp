#include "ModelContainer.h"
ModelContainer::ModelContainer()
{
	localMap.clear();
	globalMap.clear();
}

ModelContainer::~ModelContainer()
{
}

void ModelContainer::insertGlobalModel(abstractGlobalModel* model)
{
	int id = model->getID();
	auto it = globalMap.find(id);
	if (it != globalMap.end())
	{
		cout << "error: Model 已经存在，请重新检查站点文件的SiteName字段，重复字段为" << id << ". 插入失败，已经退出";
		return;
	}
	globalMap.insert(make_pair(id, model));
}



void ModelContainer::insertLocalModel(abstractLocalModel* model)
{
	int id = model->getID();
	auto it = localMap.find(id);
	if (it != localMap.end())
	{
		cout << "error: Model 已经存在，请重新检查站点文件的SiteName字段，重复字段为" << id << ". 插入失败，已经退出";
		return;
	}
	localMap.insert(make_pair(id, model));
}

void ModelContainer::deleteGlobalModel(abstractGlobalModel* model)
{
	auto it = globalMap.begin();
	for (; it !=globalMap.end();)
	{
		if (it->second == model)
		{
			globalMap.erase(it++);
			cout << "success: Model 已经删除" << endl;
			return;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: 城市容器中模型数量为" << globalMap.size() << endl;
	return;
}

void ModelContainer::deleteGlobalModel(string name)
{
	auto it = globalMap.begin();
	for (; it !=globalMap.end();)
	{
		if (it->second->getName() == name)
		{
			globalMap.erase(it++);
			cout << "success: Model 已经删除" << endl;
			return;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: 城市容器中模型数量为" <<globalMap.size() << endl;
	return;
}


void ModelContainer::deleteGlobalModel(int id)
{
	auto it =globalMap.find(id);
	if (it != globalMap.end())
	{
		globalMap.erase(it);
		cout << "success: Model 已经删除 ,id是" << id << endl;
	}
	return;
}



void ModelContainer::deletelocalModel(abstractLocalModel* model)
{
	auto it = localMap.begin();
	for (; it != localMap.end();)
	{
		if (it->second == model)
		{
			localMap.erase(it++);
			cout << "success: Model 已经删除" << endl;
			return;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: 局部容器中模型数量为" << globalMap.size() << endl;
	return;
}

void ModelContainer::deletelocalModel(string name)
{
	auto it = localMap.begin();
	for (; it != localMap.end();)
	{
		if (it->second->getName() == name)
		{
			localMap.erase(it++);
			cout << "success:局部 Model 已经删除" << endl;
			return;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: 局部容器中模型数量为" << globalMap.size() << endl;
	return;

}

void ModelContainer::deletelocalModel(int id)
{
	auto it = localMap.find(id);
	if (it != localMap.end())
	{
		localMap.erase(it);
		cout << "success: 局部 Model 已经删除 ,id是" << id << endl;
	}
	return;
}

bool ModelContainer::isLocalExist()
{
	if (localMap.size()>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ModelContainer::isLocalType(int id)
{
	auto it = localMap.find(id);
	if (it!=localMap.end())
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
	auto it = globalMap.find(id);
	if (it != globalMap.end())
	{
		ModelType a = it->second->getType();
		if (a == ModelType::CITY || a == ModelType::OBJ)
		{
			return true;
		}
	}
	return false;
}

abstractLocalModel * ModelContainer::getFirstLocal()
{
	if (localMap.size()>0)
	{
		auto it = localMap.begin();
		return it->second;
	}
	return nullptr;
}

abstractLocalModel * ModelContainer::getLocalModelByID(int id)
{
	auto it = localMap.find(id);
	if (it!=localMap.end())
	{
		return it->second;
	}
	return nullptr;
}

bool ModelContainer::isCityExist()
{
	if (globalMap.size()>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ModelContainer::isLocalIDExist(int id)
{
	auto it = localMap.find(id);
	if (it !=localMap.end())
	{
		return true;
	}
	return false;
}

abstractGlobalModel* ModelContainer::getFirstCity()
{
	if (globalMap.size()>0)
	{
		auto it = globalMap.begin();
		return it->second;
	}
	

	return nullptr;
}

std::vector<int> ModelContainer::getGlobalIDs()
{
	auto it = globalMap.begin();
	vector<int> res;
	while (it != globalMap.end())
	{
		res.push_back(it->first);
		it++;
	}
	return res;
}



std::vector<int> ModelContainer::getLocalModelID()
{
	auto it = localMap.begin();
	vector<int> res;
	while (it != localMap.end())
	{
	    res.push_back(it->first);
		it++;
	}
	return res;
}

std::vector<std::string> ModelContainer::getGlobalNames()
{
	auto it = globalMap.begin();
	vector<string> res;
	while (it != globalMap.end())
	{
		res.push_back(it->second->getName());
		it++;
	}
	return res;
}

std::vector<std::string> ModelContainer::getlocalNames()
{
	auto it = localMap.begin();
	vector<string> res;
	while (it != localMap.end())
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
	abstractGlobalModel* model = getFirstCity();
	vector<Vector3d> res;
	if (model!=nullptr)
	{
		res.push_back(model->getMinPoint());
		res.push_back(model->getMaxPoint());
	}
	return res;
}

int ModelContainer::getCityBuildingNum()
{
	if (globalMap.size() > 0)
	{
		abstractGlobalModel* model = getFirstCity();
		return model->getBuildingSize();
	}
	return 0;
}

int ModelContainer::getCityConcaveNum()
{
	if (globalMap.size()>0)
	{
		abstractGlobalModel* model = getFirstCity();
		return model->getConcaveSize();
	}
	return 0;
}

int ModelContainer::getLocalFaceNum(int id/*=NO_MODEL_ID*/)
{
	if (id==NO_LOCAL_MODEL_ID)
	{
		return -1;
	}
	abstractLocalModel*tmp = getLocalModelByID(id);
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
	abstractLocalModel *tmp = getLocalModelByID(id);
	if (tmp == nullptr)
	{
		return res;
	}
	res.push_back(tmp->getMin());
	res.push_back(tmp->getMax());
	return res;
}
