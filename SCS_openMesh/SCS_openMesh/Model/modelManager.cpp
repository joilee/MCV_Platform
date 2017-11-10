#include "modelManager.h"
#include "cityModel/cityModel.h"
#include <QDebug>
#include <iostream>
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
		cout<<"error: Model �Ѿ����ڣ�����ģ������.����Ϊ"<<name<<". ����ʧ�ܣ��Ѿ��˳�";
		return;
	}
	modelMap.insert(make_pair(name, model));
	cout << "success: ģ���Ѳ���������" << endl;

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
			cout << "success: Model �Ѿ�ɾ��" << endl;
		}
		else
		{
			it++;
		}
	}
	cout << "Info: ������ģ������Ϊ" << modelMap.size() << endl;
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
		cout << "success: Model �Ѿ�ɾ��" << endl;
	}
	else
	{
		cout << "error: ������û��ƥ�������" << endl;
	}

	cout << "Info: ������ģ������Ϊ" << modelMap.size() << endl;
}

void ModelManager::loadCityModel(string path)
{
	abstractModel * tmp=cityFac->loadModel(path);
	cout << "success: ģ���Ѿ�����" << endl;
	insertModel(tmp);
	
}

bool ModelManager::generateLocalModel(Vector3d center, double range)
{
	//Ŀǰֻ��������ĵ�һ�����г������������û�У����˳�������false
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
		return false;
	}
	abstractModel * localModel = cityFac->loadModel(center, range, city);
	cout << "success: �ֲ�ģ���Ѿ����죡" << endl;
	insertModel(localModel);
	drawTriangleScene = true;
	cityLocalModel *tmp = dynamic_cast<cityLocalModel*>(localModel);
	tmp->writeToObj();
	return true;
}
void ModelManager::sendNewState()
{
	map<string, abstractModel*>::iterator it = modelMap.begin();
	//vector<bool> exist;//��¼�Ƿ��г��г����;ֲ�����
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
			m_subject->visItem->setBuildingNum(cityNum);
			m_subject->visItem->setConcaveNum(concave);
			m_subject->visItem->setcityMax(MaxPos);
			m_subject->visItem->setcityMin(MinPos);
			m_subject->visItem->modelList[0] = city->getName();
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
			m_subject->visItem->setlocalMin(minPs);
			m_subject->visItem->setlocalMax(maxPs);
			m_subject->visItem->setLocalFaceNum(num);
			m_subject->visItem->modelList[1] = tmp->getName();
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
	cityLocalModel *tmp = getFirstLocal();
	modelPara->setCityScene(tmp->getScene());
	modelPara->setSceneRange(tmp->getRange());
	modelPara->setMaterialVector(matManager->getMaterialVector());
	modelPara->setGround_Mesh(tmp->getMesh());
	modelPara->setVertices(tmp->getVertices());
	modelPara->setFaces(tmp->getFaces());
	modelPara->setNormals(tmp->getNF());
	modelPara->setEachFaceMaterial(tmp->getF_material());
	modelPara->setApEdgeList(tmp->getScene()->getAPEdge());
}