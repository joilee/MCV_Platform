#pragma  once


#include  "../../observer/modelSubject.h"
#include "observer/LocalModelSubject.h"
#include "Container/ModelContainer.h"
#include "../Material/materialManager.h"
#include <map>
class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	/*
	@brief ��map�в���ģ��
	*/
	void  insertModel(abstractModel* model);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ��ָ��
	*/
	void  deleteModel(abstractModel* model);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ�Ͷ�һ�޶�������
	*/
	void  deleteModel(string name);

	/*
	@brief ��map��ɾ��ģ�ͣ�����ģ��id
	*/
	void deleteModel(int id);
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
	@brief ��ģ�����ƺͳ��г������ݷ��͵���Ӧ�Ĺ۲���
	@return void
	@note ���ͽ���������
	*/
	void sendNewState();

	void sendLocalStateByID(int id);

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
	cityLocalModel * getFirstLocal();
	cityLocalModel * getLocalModelByID(int id);
	/*
	@brief ���ȫ��ģ���Ƿ���ڣ����Ƿ��п����ʷֵ�ģ��
	*/
	bool checkCityExist();

	/*
	@brief ���ص�һ������ģ��
	*/
	cityModel * getFirstCity();

	/*
	@brief ���ز�͸����
	*/
	inline double getAlpha(){ return transparency; }

	/*
	@brief �Ƿ�Ҫ���ƾֲ�����
	*/
	inline bool drawLocalFlag(){ return drawTriangleScene; }

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

	bool getWholeModelFlag();

	void setWholeModelFlag(bool a);

	void setLocalShowID(int id);
private:

	
	cityModelFactory *cityFac;
	modelSubject * m_subject;
	LocalModelSubject * m_local_subject;

	//��Ҫչʾ�ľֲ�ģ�ͣ�����ģ��ֻҪָ����ʾ���
	bool drawTriangleScene; //�ֲ��������ļ���������flag
	int LocalModelID_SHOW;
	bool wholeModel_Flag;

	//����͸����
	double transparency;


	//
	ModelPara * modelPara;
	ModelContainer *modelContainer;
public:
	scsMaterialManager * matManager;
};