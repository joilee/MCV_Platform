#pragma once
#include "../util/vector.h"
#include <vector>
using namespace  std;

struct Vedge
{
	Vector3d start;  //��ʼ��
	Vector3d end;  //��ֹ��
	//���������ķ�����
	Vector3d normal_front;
	Vector3d normal_back;
	int materialId;  //�������object�Ĳ�������
};


class Building
{
public:
	Building();
	~Building();
	vector<Vector3d> upper_facePoint;  //�������϶��������
	double height;  //������߶ȣ����϶����z�����ȥ�߶ȼ��ɻ���µ����Ӧ��ĸ߶�z����
};