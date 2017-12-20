#include "modelManager.h"
#include "Model/cityModel/cityModel.h"
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <memory>
ModelManager::ModelManager()
{
	matManager = new scsMaterialManager;
	modelPara = new ModelPara;
	cityFac = new cityModelFactory;
	localFac = new cityLocalModelFactory;
	m_subject = new modelSubject;
	m_local_subject = new LocalModelSubject;
	modelContainer = new ModelContainer;
	LocalModelID_SHOW = -1111;
}

ModelManager::~ModelManager()
{
	if (modelPara != NULL)
	{
		delete modelPara;
	}
	delete modelContainer;
	delete matManager;
	delete cityFac;
	delete m_subject;
}

void ModelManager::insertGlobalModel(abstractGlobalModel* model)
{
	modelContainer->insertGlobalModel(model);
	cout << "success: ����ģ���Ѳ���������" << endl;
	sendAllModelName();
}

void ModelManager::deleteGlobalModel(abstractGlobalModel* model)
{
	modelContainer->deleteGlobalModel(model);
	cout << "Info: ������ģ������Ϊ" << modelContainer->getGlobalModelSize() << endl;
	sendAllModelName();
}


void ModelManager::deleteGlobalModel(int id)
{
	modelContainer->deleteGlobalModel(id);
	cout << "Info: ������ģ������Ϊ" << modelContainer->getGlobalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::insertLocalModel(abstractLocalModel* model)
{
	modelContainer->insertLocalModel(model);
	cout << "Info: ������ģ������Ϊ" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::deleteGLobalModel(string name)
{
	modelContainer->deleteGlobalModel(name);
	cout << "Info: ������ģ������Ϊ" << modelContainer->getGlobalModelSize() << endl;
	sendAllModelName();
}



void ModelManager::deleteLocalModel(abstractLocalModel* model)
{
	modelContainer->deletelocalModel(model);
	cout << "Info: ������ģ������Ϊ" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::deleteLocalModel(string name)
{
	modelContainer->deletelocalModel(name);
	cout << "Info: ������ģ������Ϊ" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::deleteLocalModel(int id)
{
	modelContainer->deletelocalModel(id);
	cout << "Info: ������ģ������Ϊ" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::loadCityModel(string path)
{
	abstractGlobalModel * tmp=cityFac->loadModel(path);
	cout << "success: ģ���Ѿ�����" << endl; 
	insertGlobalModel(tmp);
}

//����objģ��
void ModelManager::loadGlobalObj(string objPath)
{
	abstractGlobalModel * tmp = cityFac->loadModel(objPath);
	cout << "success: ģ���Ѿ�����" << endl;
	insertGlobalModel(tmp);
}


bool ModelManager::generateLocalModel(vector<Vector3d> center, vector<int> siteName,double range)
{
	//Ŀǰֻ��������ĵ�һ�����г������������û�У����˳�������false

	//step1 �ҵ���ĳ��г��� ����ȫ��Ψһ��
	abstractGlobalModel * city = modelContainer->getFirstCity();

	if (city ==nullptr)
	{
		QMessageBox::critical(NULL, QStringLiteral("ģ��"), QStringLiteral("û�г��г����������¶��룡"), QMessageBox::Yes, QMessageBox::Yes);
		return false;
	}
	cout << "��ʼ�����³���,��" << center.size() << "������" << endl;
	if (city->getType()==ModelType::CITY)
	{
		for (int i = 0; i < center.size(); i++)//�������ģ��
		{
			abstractLocalModel * localModel = localFac->loadModel(center[i], siteName[i], range, (cityModel*)city);
			insertLocalModel(localModel);
			cout << "success: ��" << i + 1 << "���ֲ�ģ���Ѿ����죡" << endl;
		}
	}

	return true;
}

void ModelManager::getGlobalNewState()
{
	if (modelContainer->isCityExist())
	{
		int cityNum = modelContainer->getCityBuildingNum();
		int concave = modelContainer->getCityConcaveNum();
		vector<Vector3d> box = modelContainer->getCityBoundingBox();
		Vector3d MinPos = box[0];
		Vector3d MaxPos = box[1];
		m_subject->visItem->setGlobalFlag(true);
		m_subject->visItem->addCity(cityNum, concave, MinPos, MaxPos);
		m_subject->visItem->setGlobalName(modelContainer->getGlobalNames());
		m_subject->visItem->setGlobalModelID(modelContainer->getGlobalIDs());
	}
	else
	{
		m_subject->visItem->setGlobalFlag(false);
	}

}


void ModelManager::getLocalNewState()
{
	if (modelContainer->isLocalExist())
	{
		m_subject->visItem->setLocalFlag(true);
		m_subject->visItem->setLocalModelID(modelContainer->getLocalModelID());
		m_subject->visItem->setLocalModelName(modelContainer->getlocalNames());
	}
	else
	{
		m_subject->visItem->setLocalFlag(false);
	}
}

void ModelManager::sendLocalStateByID(int id)
{

		abstractLocalModel * tmp = modelContainer->getLocalModelByID(id);
		if ((tmp)!=nullptr)
		{
			int faceNum = modelContainer->getLocalFaceNum(id);
			vector<Vector3d> vec = modelContainer->getLocalBountdingBox(id);
			m_local_subject->visItem->addLocalScene(faceNum, vec[0], vec[1]);
			m_local_subject->visItem->setFlag(true);
		}
		else
		{
			m_local_subject->visItem->setFlag(false);
		}
		m_local_subject->notify();
}

void ModelManager::sendAllModelName()
{
	getGlobalNewState();
	getLocalNewState();

	m_subject->notify();
}

bool ModelManager::checkCityExist()
{
	return modelContainer->isCityExist();
}



void ModelManager::setLocalShowID(int id)
{
	if (modelContainer->isLocalIDExist(id))
	{
		LocalModelID_SHOW = id;
		sendLocalStateByID(id);
	}

}

int ModelManager::getLocalShowID()
{
	return LocalModelID_SHOW;
}

bool ModelManager::checkLocalExist()
{
	return modelContainer->isLocalExist();
}

bool ModelManager::checkLocalExistByID(int id)
{
	return modelContainer->isLocalIDExist(id);
}

abstractLocalModel *ModelManager::getFirstLocal()
{
	return modelContainer->getFirstLocal();
}

abstractLocalModel * ModelManager::getLocalModelByID(int id)
{
	return modelContainer->getLocalModelByID(id);
}

abstractGlobalModel  *ModelManager:: getFirstCity()
{
	return modelContainer->getFirstCity();
}

void ModelManager:: setModelPara()
{
	vector<int> ids = modelContainer->getLocalModelID();
	
	modelPara->clearModel();
	int count=0;

	for (int i = 0; i < ids.size();i++)
	{
		abstractLocalModel*tmpModel = modelContainer->getLocalModelByID(ids[i]);
		if (tmpModel->getType()==ModelType::CITY_LOCAL)
		{
			cityLocalModel *tmp = (cityLocalModel*)tmpModel;

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
		
	}

	modelPara->setMaterialVector(matManager->getMaterialVector());
	cout << "����ģ�Ͳ����ɹ�" << endl;
	QString s(QStringLiteral("ģ�Ͳ������سɹ�,����"));
	s.append(QString::number(count)).append(QStringLiteral("��վ�㡣"));

	QMessageBox::information(NULL, QStringLiteral("ģ�Ͳ���"), s);
}

