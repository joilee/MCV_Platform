#pragma  once


#include  "../../observer/modelSubject.h"
#include "observer/LocalModelSubject.h"
#include "Container/ModelContainer.h"
#include "../Material/materialManager.h"
#include <map>
#include "Model/OBJModel/objModelFactory.h"
#include "Model/OBJModel/objLocalModelFactory.h"


class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	/*
	@brief ��map�в���ģ��
	*/
	void  insertGlobalModel(abstractGlobalModel* model);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ��ָ��
	*/
	void  deleteGlobalModel(abstractGlobalModel* model);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ�Ͷ�һ�޶�������
	*/
	void  deleteGLobalModel(string name);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ��id
	*/
	void deleteGlobalModel(int id);


	/*
	@brief ��map�в���ģ��
	*/
	void  insertLocalModel(abstractLocalModel* model);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ��ָ��
	*/
	void  deleteLocalModel(abstractLocalModel* model);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ�Ͷ�һ�޶�������
	*/
	void  deleteLocalModel(string name);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ��id
	*/
	void deleteLocalModel(int id);


	/*
	@brief �������ģ��
	*/
	void loadCityModel(string path);
	
	/*
	@brief �ӳ��г����й���ֲ�����
	@param [in] Vector3d center
	@param [in]  double range
	@return bool
	@note �����г���������ʱ�򣬷����쳣
	@since 5/14/2017
	*/
	bool generateLocalModel(vector< Vector3d> center, vector<int> siteName, double range);

	/*
	@brief ����˽�б���
	*/
	modelSubject *getModelSubject(){ return m_subject; }

	LocalModelSubject * getLocalSubject(){ return m_local_subject; }
	/* 
	@brief ������ģ�����ƺͳ��г������ݷ��͵���Ӧ�Ĺ۲���
	@return void
	@note ���ͽ���������
	*/
	void getGlobalNewState();

	void getLocalNewState();

	void sendLocalStateByID(int id);

	void sendAllModelName();
	/*
	@brief ���ֲ�ģ���Ƿ���ڣ����Ƿ��п��Դ��ڵļ����ģ��
	@return bool
	@note 
	*/
	bool checkLocalExist();

	bool checkLocalExistByID(int id);

	/*
	@brief  ���ؾֲ�ģ��
	@return cityLocalModel
	@note  ����map�������ص�һ���ֲ�ģ�ͣ����û�з���NULL
	*/
	abstractLocalModel * getFirstLocal();
	abstractLocalModel * getLocalModelByID(int id);
	/*
	@brief ���ȫ��ģ���Ƿ���ڣ����Ƿ��п����ʷֵ�ģ��
	*/
	bool checkCityExist();

	/*
	@brief ���ص�һ������ģ��
	*/
	abstractGlobalModel * getFirstCity();

	/*
	@brief ���ؼ�������Ҫ��ģ�Ͳ���
	*/
	ModelPara* getModelPara(){ return modelPara; }


	//************************************
	// Method:    setModelPara
	// Note: ���ü������
	// FullName:  ModelManager::setModelPara
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void setModelPara();

	/*
	@brief �Ƿ�Ҫ���Ƴ��г���
	*/
	bool getWholeModelFlag();

	void setWholeModelFlag(bool a);

	void setLocalShowID(int id);

	int getLocalShowID();

	void loadGlobalObj(string objPath);

	void loadLocalObj(vector<Vector3d> center, double localRange, vector<int> siteName);
private:

	cityLocalModelFactory *localFac;
	cityModelFactory *cityFac;
	objModelFactory *objFac;
	objLocalModelFactory *objLocalFac;
	modelSubject * m_subject;
	LocalModelSubject * m_local_subject;

	//��Ҫչʾ�ľֲ�ģ��
	int LocalModelID_SHOW;



	//
	ModelPara * modelPara;
	ModelContainer *modelContainer;
public:
	scsMaterialManager * matManager;
};