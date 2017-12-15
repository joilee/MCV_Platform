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
	m_subject = new modelSubject;
	m_local_subject = new LocalModelSubject;
	modelContainer = new ModelContainer;
	transparency = 1;
	drawTriangleScene = false;
	wholeModel_Flag = true;
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

void ModelManager::insertModel(abstractModel* model)
{
	modelContainer->insertModel(model);
	cout << "success: 模型已插入至容器" << endl;

	sendNewState();
}
void ModelManager::deleteModel(abstractModel* model)
{
	modelContainer->deleteModel(model);
	cout << "Info: 容器中模型数量为" << modelContainer->getModelSize() << endl;
	sendNewState();
}
void ModelManager::deleteModel(string name)
{
	modelContainer->deleteModel(name);
	cout << "Info: 容器中模型数量为" << modelContainer->getModelSize() << endl;
	sendNewState();
}

void ModelManager::deleteModel(int id)
{
	modelContainer->deleteModel(id);
	cout << "Info: 容器中模型数量为" << modelContainer->getModelSize() << endl;
	sendNewState();
}

void ModelManager::loadCityModel(string path)
{
	abstractModel * tmp=cityFac->loadModel(path);
	cout << "success: 模型已经读入" << endl; 
	insertModel(tmp);
}

bool ModelManager::generateLocalModel(vector<Vector3d> center, vector<int> siteName,double range)
{
	//目前只针对遇到的第一个城市场景操作，如果没有，则退出，返回false

	//step1 找到大的城市场景 这是全局唯一的
	cityModel * city = modelContainer->getFirstCity();

	if (city ==nullptr)
	{
		QMessageBox::critical(NULL, QStringLiteral("模型"), QStringLiteral("没有城市场景！请重新读入！"), QMessageBox::Yes, QMessageBox::Yes);
		return false;
	}
	cout << "开始生成新场景,共" << center.size() << "个场景" << endl;
	for (int i = 0; i < center.size();i++)//依次添加模型
	{
		abstractModel * localModel = cityFac->loadModel(center[i], siteName[i],range, city);
		insertModel(localModel);
		cout << "success: 第"<<i+1<<"个局部模型已经构造！" << endl;
		//cityLocalModel *tmp = dynamic_cast<cityLocalModel*>(localModel);
		//tmp->writeToObj();
	}
	drawTriangleScene = true;
	return true;
}
void ModelManager::sendNewState()
{
	if (modelContainer->isCityExist())
	{
		int cityNum = modelContainer->getCityBuildingNum();
		int concave = modelContainer->getCityConcaveNum();
		vector<Vector3d> box = modelContainer->getCityBoundingBox();
		Vector3d MinPos = box[0];
		Vector3d MaxPos = box[1];
		m_subject->visItem->setFlag(true);
		m_subject->visItem->addCity(cityNum, concave, MinPos, MaxPos);
		m_subject->visItem->setName(modelContainer->getNames());
		m_subject->visItem->setModelID(modelContainer->getIDs());
	}
	else
	{
		m_subject->visItem->setFlag(false);
	}
	
	m_subject->notify();
}

void ModelManager::sendLocalStateByID(int id)
{

		cityLocalModel * tmp = modelContainer->getLocalModelByID(id);
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

bool ModelManager::checkCityExist()
{
	return modelContainer->isCityExist();
}


bool ModelManager::getWholeModelFlag()
{
	return wholeModel_Flag;
}

void ModelManager::setWholeModelFlag(bool a)
{
	wholeModel_Flag = a;
}

void ModelManager::setLocalShowID(int id)
{
	if (modelContainer->isLocalIDExist(id))
	{

	}
}

bool ModelManager::checkLocalExist()
{
	return modelContainer->isLocalExist();
}

bool ModelManager::checkLocalExistByID(int id)
{
	return modelContainer->isLocalIDExist(id);
}

cityLocalModel *ModelManager::getFirstLocal()
{
	return modelContainer->getFirstLocal();
}

cityLocalModel * ModelManager::getLocalModelByID(int id)
{
	return modelContainer->getLocalModelByID(id);
}

cityModel  *ModelManager:: getFirstCity()
{
	return modelContainer->getFirstCity();
}

void ModelManager:: setModelPara()
{
	vector<int> ids = modelContainer->getLocalCityModelID();
	
	modelPara->clearModel();
	int count=0;

	for (int i = 0; i < ids.size();i++)
	{
		cityLocalModel*tmp = modelContainer->getLocalModelByID(ids[i]);
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

	modelPara->setMaterialVector(matManager->getMaterialVector());
	cout << "设置模型参数成功" << endl;
	QString s(QStringLiteral("模型参数加载成功,共有"));
	s.append(QString::number(count)).append(QStringLiteral("个站点。"));

	QMessageBox::information(NULL, QStringLiteral("模型参数"), s);
}