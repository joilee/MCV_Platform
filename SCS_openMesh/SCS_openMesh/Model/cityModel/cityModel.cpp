#include "cityModel.h"
#include <QString>
#include <QFileDialog>
#include <QJsonValue>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QMessageBox>
#include<Windows.h>  
cityModel::cityModel(string jsonFile)
{
	QString path = QString::fromStdString(jsonFile);

	//��ȡjson�ļ������ļ��е�Ŀ¼
	QDir jsonPath(path);
	jsonPath.cdUp();
	QString fatherDirectory = jsonPath.path();
	fatherDirectory.append("/");
	
	QFile file(path);
	file.open(QIODevice::ReadWrite);
	QByteArray json = file.readAll();
	QJsonDocument jsDoc;
	jsDoc = QJsonDocument::fromJson(json);

	QString _name;
	QString _m;
	QStringList _v;//vector
	QStringList _h;//height
	QString _p;//plane
	bool flag1 = false, flag2 = false, flag3 = false, flag4 = false, flag5 = false;
	if (jsDoc.isObject())
	{
		QJsonObject obj = jsDoc.object();

		//�������
		if (obj.contains("Name"))
		{
			QJsonValue name_value = obj["Name"];
			if (name_value.isString())
			{
				_name = name_value.toString();
				flag1 = true;
			}
		}

		//������
		if (obj.contains("Material"))
		{
			QJsonValue name_value = obj["Material"];
			if (name_value.isString())
			{
				_m = fatherDirectory + name_value.toString();
				flag2 = true;
			}
		}

		//���vector
		if (obj.contains("Building"))
		{
			QVariantMap result = jsDoc.toVariant().toMap();
			foreach(QVariant build, result["Building"].toList())
			{
				QString fileName = fatherDirectory + build.toString();
				_v.append(fileName);
				flag3 = true;
			}
		}

		//���߶�
		if (obj.contains("Height"))
		{
			QVariantMap result = jsDoc.toVariant().toMap();
			foreach(QVariant heightVar, result["Height"].toList())
			{
				QString fileName = fatherDirectory + heightVar.toString();
				_h.append(fileName);
				flag4 = true;
			}
		}

		//�����溣��
		if (obj.contains("Plane"))
		{
			QJsonValue plane_value = obj["Plane"];
			if (plane_value.isString())
			{
				_p = fatherDirectory + plane_value.toString();
				flag5 = true;
			}
		}

	}
	vector<string> vv;
	vector<string> hh;
	for (int m = 0; m < _v.size();m++)
	{
		vv.push_back(_v.at(m).toStdString());
		hh.push_back(_h.at(m).toStdString());
	}
	if (flag1&&flag2&&flag3&&flag4&&flag5)
	{
		LoadModel(_name.toStdString(), vv, hh,_p.toStdString());
	}
}

cityModel::~cityModel()
{

}

void cityModel::LoadModel(string _name, vector<string> _v, vector<string> _h, string _p)
{
	name = _name;
	
	if (_v.empty()) 
	{
		QMessageBox::warning(NULL, QStringLiteral("�ļ�����"), QStringLiteral("���ȵ��뽨�����ά��Ϣ�ļ�"));
		return;
	}
	if (_h.empty())
	{
		QMessageBox::warning(NULL, QStringLiteral("�ļ�����"), QStringLiteral("���ȵ��뽨����߶���Ϣ�ļ�"));
		return;
	}
	if (_p.empty())
	{
		QMessageBox::warning(NULL, QStringLiteral("�ļ�����"), QStringLiteral("���ȵ��뺣����Ϣ�ļ�"));
		return;
	}
	DWORD start_time = GetTickCount();
	city = new cityScene(_v, _h, _p);
	DWORD end_time = GetTickCount();
	id = DEFAULT_MODEL_ID;
	cout << "Info: "<<name<<"���г��浼�����" << endl;
	cout << "Info: " << "ģ�͵��빲��ʱ" << (end_time - start_time)  << "ms" << endl;

}
void cityModel:: draw(vector<bool> mode)
{

}

void cityModel::initDraw()
{

}