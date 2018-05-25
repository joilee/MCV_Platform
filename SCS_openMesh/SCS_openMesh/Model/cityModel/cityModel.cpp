#include "cityModel.h"
#include <QString>
#include <QFileDialog>
#include <QJsonValue>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QMessageBox>
#include <Windows.h>  
#include "../../Context/context.h"

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
	for (int m = 0; m < _v.size(); m++)
	{
		vv.push_back(_v.at(m).toStdString());
		hh.push_back(_h.at(m).toStdString());
	}
	if (flag1&&flag2&&flag3&&flag4&&flag5)
	{
		LoadModel(_name.toStdString(), vv, hh, _p.toStdString());
		fileType = ModelType::CITY;
	}
	buildings = city->getTotal_Building();
	initDraw();
}

cityModel::cityModel(string mapName, vector<string> buidlingPath, vector<string> heightPath, string altitudePath)
{
	LoadModel(mapName, buidlingPath, heightPath, altitudePath);
	fileType = ModelType::CITY;
	buildings = city->getTotal_Building();
	initDraw();
}

cityModel::~cityModel()
{

}

void cityModel::LoadModel(string _name, vector<string> _v, vector<string> _h, string _p)
{
	name = _name;

	//if (_v.empty())
	//{
	//	QMessageBox::warning(NULL, "Error", QStringLiteral("���ȵ��뽨�����ά��Ϣ�ļ�"));
	//	return;
	//}
	//if (_h.empty())
	//{
	//	QMessageBox::warning(NULL, "Error", QStringLiteral("���ȵ��뽨����߶���Ϣ�ļ�"));
	//	return;
	//}
	//if (_p.empty())
	//{
	//	QMessageBox::warning(NULL, "Error", QStringLiteral("���ȵ��뺣����Ϣ�ļ�"));
	//	return;
	//}
	DWORD start_time = GetTickCount();
	city = new cityScene(_v, _h, _p);
	DWORD end_time = GetTickCount();
	id = DEFAULT_MODEL_ID;
	cout << "Info: " << name << "���г��浼�����" << endl;
	cout << "Info: " << "ģ�͵��빲��ʱ" << (end_time - start_time) << "ms" << endl;
}

void CALLBACK PolyLine3DBegin(GLenum type)
{
	glBegin(type);
}

void CALLBACK PolyLine3DVertex(GLdouble *vertex)
{
	const GLdouble *pointer = (GLdouble *)vertex;
	glVertex3dv(pointer);
}

void CALLBACK PolyLine3DEnd()
{
	glEnd();
}

GLUtesselator* tesser()
{
	GLUtesselator *tess = gluNewTess();
	gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK*)())&PolyLine3DBegin);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK*)())&PolyLine3DVertex);
	gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK*)())&PolyLine3DEnd);
	return tess;
}

void cityModel::draw(vector<bool> mode, double alpha)
{
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//���Ƶ�
	if (mode[0]){
		glColor4d(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < upperface_points.size(); i++)
		{
			glVertex3d((GLdouble)upperface_points[i].x, (GLdouble)upperface_points[i].y, (GLdouble)upperface_points[i].z);
		}
		for (int i = 0; i < underface_points.size(); i++)
		{
			glVertex3d((GLdouble)underface_points[i].x, (GLdouble)underface_points[i].y, (GLdouble)underface_points[i].z);
		}
		glEnd();
	}
	//�����߿�
	if (mode[1]){
		glLineWidth(2.0f);
		glColor4d(0.0f, 0.0f, 0.0f, 1.0f);

		//�ϱ���
		glBegin(GL_LINES);
		int size = 0;
		for (int i = 0; i < buildings.size(); i++)
		{
			int count = buildings[i].upper_facePoint.size() - 1;
			for (int j = 0; j < count; j++)
			{
				glVertex3d((GLdouble)upperface_points[j + size].x, (GLdouble)upperface_points[j + size].y, (GLdouble)upperface_points[j + size].z);
				glVertex3d((GLdouble)upperface_points[(j + 1) % count + size].x, (GLdouble)upperface_points[(j + 1) % count + size].y, (GLdouble)upperface_points[(j + 1) % count + size].z);
			}
			size += count;
		}

		//�µ���
		size = 0;
		for (int i = 0; i < buildings.size(); i++)
		{
			int count = buildings[i].upper_facePoint.size() - 1;
			for (int j = 0; j < count; j++)
			{
				glVertex3d((GLdouble)underface_points[j + size].x, (GLdouble)underface_points[j + size].y, (GLdouble)underface_points[j + size].z);
				glVertex3d((GLdouble)underface_points[(j + 1) % count + size].x, (GLdouble)underface_points[(j + 1) % count + size].y, (GLdouble)underface_points[(j + 1) % count + size].z);
			}
			size += count;
		}

		//����
		size = 0;
		for (int i = 0; i < buildings.size(); i++)
		{
			int count = buildings[i].upper_facePoint.size() - 1;
			for (int j = 0; j < count; j++)
			{
				glVertex3d((GLdouble)upperface_points[j + size].x, (GLdouble)upperface_points[j + size].y, (GLdouble)upperface_points[j + size].z);
				glVertex3d((GLdouble)underface_points[j + size].x, (GLdouble)underface_points[j + size].y, (GLdouble)underface_points[j + size].z);
			}
			size += count;
		}
		glEnd();
	}

	//������(OpenGL��GL_POLYGONֻ֧��͹����Σ�
	if (mode[2]){
		GLUtesselator* tess = tesser();
		if (!tess)return;

		glColor4d(1.0f, 1.0f, 1.0f, alpha);
		for (int buildings_id = 0; buildings_id < buildings.size(); buildings_id++)
		{
			int count = buildings[buildings_id].upper_facePoint.size() - 1;
			double building_height = buildings[buildings_id].height; //������ĸ߶�

			//�������϶���
			gluTessBeginPolygon(tess, NULL);
			GLdouble(*a)[3] = new GLdouble[count][3];
			gluTessBeginContour(tess);
			for (int id = 0; id<count; id++) //��¼building���������ʱ����ĩ���غϣ���¼���Σ�����   .size����-1
			{
				a[id][0] = buildings[buildings_id].upper_facePoint[id].x;
				a[id][1] = buildings[buildings_id].upper_facePoint[id].y;
				a[id][2] = buildings[buildings_id].upper_facePoint[id].z;
				gluTessVertex(tess, a[id], a[id]);
			}
			gluTessEndContour(tess);
			gluTessEndPolygon(tess);
			delete[]a;

			//�������µ���
			gluTessBeginPolygon(tess, NULL);
			GLdouble(*b)[3] = new GLdouble[count][3];
			gluTessBeginContour(tess);
			for (int id = 0; id < count; id++)
			{
				b[id][0] = buildings[buildings_id].upper_facePoint[id].x;
				b[id][1] = buildings[buildings_id].upper_facePoint[id].y;
				b[id][2] = buildings[buildings_id].upper_facePoint[id].z - building_height;
				gluTessVertex(tess, b[id], b[id]);
			}
			gluTessEndContour(tess);
			gluTessEndPolygon(tess);
			delete[]b;

			//���������
			glBegin(GL_QUADS);
			for (int id = 0; id < count; id++)
			{
				glVertex3d(buildings[buildings_id].upper_facePoint[id].x, buildings[buildings_id].upper_facePoint[id].y, buildings[buildings_id].upper_facePoint[id].z);
				glVertex3d(buildings[buildings_id].upper_facePoint[id + 1].x, buildings[buildings_id].upper_facePoint[id + 1].y, buildings[buildings_id].upper_facePoint[id + 1].z);
				glVertex3d(buildings[buildings_id].upper_facePoint[id + 1].x, buildings[buildings_id].upper_facePoint[id + 1].y, buildings[buildings_id].upper_facePoint[id + 1].z - building_height);
				glVertex3d(buildings[buildings_id].upper_facePoint[id].x, buildings[buildings_id].upper_facePoint[id].y, buildings[buildings_id].upper_facePoint[id].z - building_height);
			}
			glEnd();
		}
		gluDeleteTess(tess);
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void cityModel::initDraw()
{
	int size = 0;
	for (int building_id = 0; building_id < buildings.size(); building_id++)
	{
		int count = buildings[building_id].upper_facePoint.size() - 1;    //��β���غ�
		double height = buildings[building_id].height;                    //������߶�

		//�϶���ĵ�
		for (int i = 0; i < count; i++)
		{
			Vector3d point = buildings[building_id].upper_facePoint[i];
			upperface_points.push_back(point);
		}

		//�µ���ĵ�
		for (int i = 0; i < count; i++)
		{
			Vector3d point = buildings[building_id].upper_facePoint[i];
			double newPointZ = point.z - height;
			underface_points.push_back(Vector3d(point.x, point.y, newPointZ));
		}
		size += count;
	}
}

double cityModel::getAltitude(double x, double y)
{
	return city->getAltitude(x, y);
}

Vector3d cityModel::getMinPoint()
{
	return city->getMinPoint();
}

Vector3d cityModel::getMaxPoint()
{
	return city->getMaxPoint();
}

int cityModel::getBuildingSize()
{
	return city->getBuildingSize();
}

int cityModel::getConcaveSize()
{
	return city->getConcaveNum();
}
