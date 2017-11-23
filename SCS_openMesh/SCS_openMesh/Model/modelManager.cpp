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

bool ModelManager::generateLocalModel(vector<Vector3d> center, double range)
{
	//Ŀǰֻ��������ĵ�һ�����г������������û�У����˳�������false

	//step1 �ҵ���ĳ��г��� ����ȫ��Ψһ��
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
		QMessageBox::critical(NULL, QStringLiteral("ģ��"), QStringLiteral("û�г��г����������¶��룡"), QMessageBox::Yes, QMessageBox::Yes);
		return false;
	}
	cout << "ȥ���ֲ�����ģ��" << endl;
	cout << "��ʼ�����³���,��" << center.size() << "������" << endl;
	for (int i = 0; i < center.size();i++)//�������ģ��
	{
		abstractModel * localModel = cityFac->loadModel(center[i], range, city);
		string oldName = localModel->getName();
		oldName.append("_ID_").append(std::to_string(i));
		insertModel(localModel);
		cout << "success: �ֲ�ģ���Ѿ����죡" << endl;
		
		cityLocalModel *tmp = dynamic_cast<cityLocalModel*>(localModel);
		//tmp->writeToObj();
	}
	drawTriangleScene = true;
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
	cout << "����ģ�Ͳ����ɹ�" << endl;
	QString s(QStringLiteral("ģ�Ͳ������سɹ�,����"));
	s.append(QString::number(count)).append(QStringLiteral("��վ�㡣"));

	QMessageBox::information(NULL, QStringLiteral("ģ�Ͳ���"), s);
}