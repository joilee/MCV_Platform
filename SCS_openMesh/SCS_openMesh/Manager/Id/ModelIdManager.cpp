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
	//检测名称是否重复

	if (isNameExist(name))
	{
		QMessageBox::warning(NULL, "Error", QStringLiteral("模型名字重复！"));
		cout << "获取Id失败，模型名称重复，ModelName is"<<name << endl;
		return -1;
	}
	//检测ID是否重复，如果没有，就使用该ID，如果重复，依次递增，直至寻找到一个可用id
	
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
		QMessageBox::warning(NULL, "Error", QStringLiteral("模型id或者name重复！"));
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
