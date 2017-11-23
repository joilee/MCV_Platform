#include "modelManager.h"
#include "cityModel/cityModel.h"
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <memory>
ModelManager::ModelManager()
{
	matManager = new scsMaterialManager;
	modelPara = new ModelPara;
	cityFac = new cityModelFactory;
	m_subject = new modelSubject;
	transparency = 1;
	drawTriangleScene = false;
}

ModelManager::~ModelManager()
{
	if (modelPara != NULL)
	{
		delete modelPara;
	}
}

void ModelManager::insertModel(abstractModel* model)
{
	string name = model->getName();

	if (modelMap.find(name)!=modelMap.end())
	{
		cout<<"error: Model 已经存在，或者模型重名.名字为"<<name<<". 插入失败，已经退出";
		return;
	}
	modelMap.insert(make_pair(name, model));
	cout << "success: 模型已插入至容器" << endl;

	sendNewState();
}
void ModelManager::deleteModel(abstractModel* model)
{
	map<string, abstractModel*>::iterator it;
	for (it = modelMap.begin(); it != modelMap.end();)
	{
		if (it->second == model)
		{
			modelMap.erase(it++);
			cout << "success: Model 已经删除" << endl;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: 容器中模型数量为" << modelMap.size() << endl;
}
void ModelManager::deleteModel(string name)
{
	map<string, abstractModel*>::iterator it = modelMap.find(name);
	if (it!=modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityLocalModel))
		{
			drawTriangleScene = false;
		}
		modelMap.erase(it);
		cout << "success: Model 已经删除" << endl;
	}
	else
	{
		cout << "error: 容器中没有匹配的名字" << endl;
	}

	cout << "Info: 容器中模型数量为" << modelMap.size() << endl;
}

void ModelManager::loadCityModel(string path)
{
	abstractModel * tmp=cityFac->loadModel(path);
	cout << "success: 模型已经读入" << endl;
	insertModel(tmp);
	
}

bool ModelManager::generateLocalModel(vector<Vector3d> center, double range)
{
	//目前只针对遇到的第一个城市场景操作，如果没有，则退出，返回false

	//step1 找到大的城市场景 这是全局唯一的
	map<string, abstractModel*>::iterator it = modelMap.begin();
	cityModel * city = NULL;
	while (it != modelMap.end())
	{
		//cout << typeid(*(it->second)).name() << endl;
		if (typeid(*(it->second)) == typeid(cityModel))
		{
			city = dynamic_cast<cityModel*>(it->second);
			break;
		}

	}
	if (city == NULL)
	{
		QMessageBox::critical(NULL, QStringLiteral("模型"), QStringLiteral("没有城市场景！请重新读入！"), QMessageBox::Yes, QMessageBox::Yes);
		return false;
	}
	cout << "去除局部场景模型" << endl;
	cout << "开始生成新场景,共" << center.size() << "个场景" << endl;
	for (int i = 0; i < center.size();i++)//依次添加模型
	{
		abstractModel * localModel = cityFac->loadModel(center[i], range, city);
		string oldName = localModel->getName();
		oldName.append("_ID_").append(std::to_string(i));
		insertModel(localModel);
		cout << "success: 局部模型已经构造！" << endl;
		
		cityLocalModel *tmp = dynamic_cast<cityLocalModel*>(localModel);
		//tmp->writeToObj();
	}
	drawTriangleScene = true;
	return true;
}
void ModelManager::sendNewState()
{
	map<string, abstractModel*>::iterator it = modelMap.begin();
	//vector<bool> exist;//记录是否有城市场景和局部场景
	while (it != modelMap.end())
	{
		//cout << typeid(*(it->second)).name() << endl;
		if (typeid(*(it->second)) == typeid(cityModel))
		{
			cityModel * city = dynamic_cast<cityModel*>(it->second);
			int cityNum = city->getCity()->getBuildingSize();
			int concave = city->getCity()->getConcaveNum();
			Vector3d MinPos = city->getCity()->getMinPoint();
			Vector3d MaxPos = city->getCity()->getMaxPoint();
			m_subject->visItem->addCity(cityNum, concave, MinPos, MaxPos,city->getName());
			break;
		}
		it++;
	}
	it = modelMap.begin();
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityLocalModel))
		{
			cityLocalModel *tmp = dynamic_cast<cityLocalModel*>(it->second);
			int num = tmp->getTriangleNum();
			Vector3d minPs = tmp->getMin();
			Vector3d maxPs = tmp->getMax();
			m_subject->visItem->addLocalScene(num, minPs, maxPs, tmp->getName());
			break;
		}
		it++;
	}
	m_subject->notify();
}

bool ModelManager::checkCityExist()
{
	map<string, abstractModel*>::iterator it = modelMap.begin();
	it = modelMap.begin();
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityModel))
		{
			return true;
		}
		it++;
	}
	return false;
}


bool ModelManager::checkLocalExist()
{
	map<string, abstractModel*>::iterator it = modelMap.begin();
	it = modelMap.begin();
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityLocalModel))
		{
			return true;
		}
		it++;
	}
	return false;
}

cityLocalModel *ModelManager::getFirstLocal()
{
	map<string, abstractModel*>::iterator it = modelMap.begin();
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityLocalModel))
		{
			return dynamic_cast<cityLocalModel*>(it->second);
		}
		it++;
	}
	return NULL;
}

cityModel  *ModelManager:: getFirstCity()
{
	map<string, abstractModel*>::iterator it = modelMap.begin();
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityModel))
		{
			return dynamic_cast<cityModel*>(it->second);
		}
		it++;
	}
	return NULL;
}

void ModelManager:: setModelPara()
{
	map<string, abstractModel*>::iterator it = modelMap.begin();
	modelPara->clearModel();
	int count=0;
	while (it != modelMap.end())
	{
		if (typeid(*(it->second)) == typeid(cityLocalModel))
		{
			cityLocalModel*tmp= dynamic_cast<cityLocalModel*>(it->second);
			BaseModel* tmpBase = new BaseModel();
			std::shared_ptr<cityScene> tmpScene(tmp->getScene());
			tmpBase->setCityScene(tmpScene);

			tmpBase->setSceneRange(tmp->getRange());

			std::shared_ptr<MESH> tmpMesh(tmp->getMesh());
			tmpBase->setGround_Mesh(tmpMesh);

			tmpBase->setVertices(tmp->getVertices());
			tmpBase->setFaces(tmp->getFaces());
			tmpBase->setNormals(tmp->getNF());
			tmpBase->setEachFaceMaterial(tmp->getF_material());
			tmpBase->setApEdgeList(tmp->getScene()->getAPEdge());

			modelPara->addModel(tmpBase);
			count++;
		}
		it++;
	}
	modelPara->setMaterialVector(matManager->getMaterialVector());
	cout << "设置模型参数成功" << endl;
	QString s(QStringLiteral("模型参数加载成功,共有"));
	s.append(QString::number(count)).append(QStringLiteral("个站点。"));

	QMessageBox::information(NULL, QStringLiteral("模型参数"), s);
}