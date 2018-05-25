#include "ModelIdManager.h"
#include <QMessageBox>
#include <QString>
#include <iostream>
ModelIdManager::ModelIdManager()
{
	IdToName.clear();
	NameToId.clear();
}


ModelIdManager::~ModelIdManager()
{
	removeAll();
}



int ModelIdManager::getModelID(string name,int id)
{
	//��������Ƿ��ظ�

	if (isNameExist(name))
	{
		QMessageBox::warning(NULL, "Error", QStringLiteral("ģ�������ظ���"));
		cout << "��ȡIdʧ�ܣ�ģ�������ظ���ModelName is"<<name << endl;
		return -1;
	}
	//���ID�Ƿ��ظ������û�У���ʹ�ø�ID������ظ������ε�����ֱ��Ѱ�ҵ�һ������id
	
	int tmpID = id;
	if (isIDExist(id))
	{		
		tmpID = getNextID(id);
	}
	addIDandName(tmpID, name);
	return tmpID;
}

int ModelIdManager::getNextID(int id)
{
	int res = id;
	while (isIDExist(res))
	{
		id++;
	}
	return res;
}

void ModelIdManager::removeID(int id)
{
	if (isIDExist(id))
	{
		deleteIDandName(id);
	}
	return;
}

void ModelIdManager::removeAll()
{
	IdToName.clear();
	NameToId.clear();
}

void ModelIdManager::addIDandName(int id, string name)
{
	if (isIDExist(id)||isNameExist(name))
	{
		QMessageBox::warning(NULL, "Error", QStringLiteral("ģ��id����name�ظ���"));
		return;
	}
	IdToName.insert(make_pair(id, name));
	NameToId.insert(make_pair(name, id));
}

void ModelIdManager::deleteIDandName(int id)
{
	if (isIDExist(id))
	{
		map<int, string>::iterator it = IdToName.find(id);
		string name = it->second;
		IdToName.erase(id);
		NameToId.erase(name);
	}
}

void ModelIdManager::deleteIDandName(string name)
{
	if (isNameExist(name))
	{
		map<string, int>::iterator it = NameToId.find(name);
		int id = it->second;
		IdToName.erase(id);
		NameToId.erase(name);
	}
}

bool ModelIdManager::isNameExist(string name)
{
	map<string, int>::iterator it = NameToId.find(name);
	if (it==NameToId.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ModelIdManager::isIDExist(int id)
{
	map<int, string>::iterator it2 = IdToName.find(id);
	if (it2==IdToName.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
