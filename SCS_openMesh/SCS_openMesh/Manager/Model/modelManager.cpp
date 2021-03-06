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
	objFac = new objModelFactory;
	objLocalFac = new objLocalModelFactory;
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
	cout << "success: 城市模型已插入至容器" << endl;
	sendAllModelName();
}

void ModelManager::deleteGlobalModel(abstractGlobalModel* model)
{
	modelContainer->deleteGlobalModel(model);
	cout << "Info: 容器中模型数量为" << modelContainer->getGlobalModelSize() << endl;
	sendAllModelName();
}


void ModelManager::deleteGlobalModel(int id)
{
	modelContainer->deleteGlobalModel(id);
	cout << "Info: 容器中模型数量为" << modelContainer->getGlobalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::insertLocalModel(abstractLocalModel* model)
{
	modelContainer->insertLocalModel(model);
	cout << "Info: 容器中模型数量为" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::deleteGLobalModel(string name)
{
	modelContainer->deleteGlobalModel(name);
	cout << "Info: 容器中模型数量为" << modelContainer->getGlobalModelSize() << endl;
	sendAllModelName();
}



void ModelManager::deleteLocalModel(abstractLocalModel* model)
{
	modelContainer->deletelocalModel(model);
	cout << "Info: 容器中模型数量为" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::deleteLocalModel(string name)
{
	modelContainer->deletelocalModel(name);
	cout << "Info: 容器中模型数量为" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::deleteLocalModel(int id)
{
	modelContainer->deletelocalModel(id);
	cout << "Info: 容器中模型数量为" << modelContainer->getLocalModelSize() << endl;
	sendAllModelName();
}

void ModelManager::loadCityModel(string path)
{
	abstractGlobalModel * tmp=cityFac->loadModel(path);
	cout << "success: 模型已经读入" << endl; 
	insertGlobalModel(tmp);
}

void ModelManager::loadCityModel(string name, vector<string> bPath, vector<string> hPath, string aPath)
{
	abstractGlobalModel * tmp = cityFac->loadModel(name,bPath,hPath,aPath);
	cout << "success: 模型已经读入" << endl;
	insertGlobalModel(tmp);
}

//读入obj模型
void ModelManager::loadGlobalObj(string objPath)
{
	abstractGlobalModel * tmp = objFac->loadModel(objPath);
	cout << "success: 模型已经读入" << endl;
	insertGlobalModel(tmp);
}


void ModelManager::loadLocalObj(vector<Vector3d> center, double localRange, vector<int> siteName)
{
	//step1 找到大的城市场景 这是全局唯一的
	abstractGlobalModel * city = modelContainer->getFirstCity();
	if (city == nullptr)
	{
		QMessageBox::warning(NULL, "Error", QStringLiteral("室外场景加载有误，请重新导入！"));
		return ;
	}
	cout << "开始生成新场景,共" << center.size() << "个场景" << endl;

	if (city->getType() == ModelType::OBJ)
	{
		for (int i = 0; i < center.size(); i++)//依次添加模型
		{
			stringstream stream;
			stream << siteName[i];
			string name= stream.str();   //此处也可以用 stream>>string_temp  

			abstractLocalModel * localModel = objLocalFac->loadModel(center[i], localRange, name,(objModel*)city);
			insertLocalModel(localModel);
			cout << "success: 第" << i + 1 << "个局部模型已经构造！" << endl;
		}
	}
}


bool ModelManager::generateLocalModel(vector<Vector3d> center, vector<int> siteName,double range)
{
	//目前只针对遇到的第一个城市场景操作，如果没有，则退出，返回false

	//step1 找到大的城市场景 这是全局唯一的
	abstractGlobalModel * city = modelContainer->getFirstCity();

	if (city ==nullptr)
	{
		QMessageBox::warning(NULL, "Error", QStringLiteral("室外场景加载有误，请重新导入！"));
		return false;
	}
	cout << "开始生成新场景,共" << center.size() << "个场景" << endl;
	if (city->getType()==ModelType::CITY)
	{
		for (int i = 0; i < center.size(); i++)//依次添加模型
		{
			abstractLocalModel * localModel = localFac->loadModel(center[i], siteName[i], range, (cityModel*)city);
			insertLocalModel(localModel);
			cout << "success: 第" << i + 1 << "个局部模型已经构造！" << endl;
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

			tmpBase->setCityScene(tmp->getScene());
			tmpBase->setApEdgeList(tmp->getScene()->getAPEdge());
			tmpBase->setApEdgeID(tmp->getScene()->getAPEdgeID());


			tmpBase->setGround_Mesh(tmp->getMesh());

			tmpBase->setSceneRange(tmp->getRange());
			tmpBase->setVertices(tmp->getVertices());
			tmpBase->setFaces(tmp->getFaces());
			tmpBase->setNormals(tmp->getNF());
			tmpBase->setEachFaceMaterial(tmp->getF_material());
			tmpBase->setModelType(ModelType::CITY_LOCAL);
			tmpBase->setModelID(tmp->getID());

			modelPara->addModel(tmpBase);
			//tmpBase->outputOBJ();
			count++;
			modelPara->mType = ModelType::CITY_LOCAL;
		}
		else if (tmpModel->getType() == ModelType::OBJ_LOCAL)
		{
			objLocalModel *tmp = (objLocalModel*)tmpModel;
			BaseModel* tmpBase = new BaseModel();

			tmpBase->setSceneRange(tmp->getRange());
			tmpBase->setVertices(tmp->getVertices());
			tmpBase->setFaces(tmp->getFaces());
			tmpBase->setNormals(tmp->getNF());
			tmpBase->setEachFaceMaterial(tmp->getF_material());
			tmpBase->setModelType(ModelType::OBJ_LOCAL);
			tmpBase->setModelID(tmp->getID());

			modelPara->addModel(tmpBase);
			count++;
			modelPara->mType = ModelType::OBJ_LOCAL;
		}
		
	}

	modelPara->setMaterialVector(matManager->getMaterialVector());
	cout << "设置模型参数成功" << endl;
	QString s(QStringLiteral("模型剖分完成，共生成"));
	s.append(QString::number(count)).append(QStringLiteral("个站点！"));

	QMessageBox::information(NULL, "Info", s);
}


