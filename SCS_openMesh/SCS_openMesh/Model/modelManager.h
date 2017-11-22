#pragma  once

#include "abstractModel.h"
#include "abstractModelFactory.h"
#include  "cityModel/cityModelFactory.h"
#include  "../observer/modelSubject.h"
#include <para/modelPara.h>
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
	bool generateLocalModel(vector< Vector3d> center,double range);

	/*
	@brief ����˽�б���
	*/
	modelSubject *getModelSubject(){ return m_subject; }

	/* 
	@brief �����µ�״̬���͵���Ӧ�Ĺ۲���
	@return void
	@note ����ֻ֧��һ�����г�����һ���ֲ��������ڡ���map���ж������ʱ��ֻ��Ա���ʱ���һ����������Ч
	*/
	void sendNewState();

	/*
	@brief ���ֲ�ģ���Ƿ���ڣ����Ƿ��п��Դ��ڵļ����ģ��
	@return bool
	@note 
	*/
	bool checkLocalExist();

	/*
	@brief  ���ؾֲ�ģ��
	@return cityLocalModel
	@note  ����map�������ص�һ���ֲ�ģ�ͣ����û�з���NULL
	*/
	cityLocalModel * getFirstLocal();

	/*
	@brief ���ȫ��ģ���Ƿ���ڣ����Ƿ��п����ʷֵ�ģ��
	*/
	bool ModelManager::checkCityExist();

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

private:

	map<string, abstractModel*> modelMap;
	cityModelFactory *cityFac;
	modelSubject * m_subject;

	//����͸����
	double transparency;
	bool drawTriangleScene; //�ֲ��������ļ���������flag

	//
	ModelPara * modelPara;

public:
	scsMaterialManager * matManager;
};