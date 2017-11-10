#ifndef EFIELD_H
#define EFIELD_H
#include "util/vector.h"
#include <complex>
#include <vector>

class Field_Path
{
public:
	double all_distance;
	double power_Loss;
	vector<Vector3d> Path_interPoint;	
	vector<int> propagation_type;   //0�����䣬1����͸�䣬2�������� 
	vector<Vector3d> intersect_faceNormal;  //��͸��·���ཻ��Ƭ�ķ�����
	vector<int>Intersection_FaceID; //��͸��·���ཻ��Ƭ���
	int edge_id;   //����·�����������id��
	Field_Path() {all_distance = 0; power_Loss = 0; edge_id = -1;}
};

struct DiffEdgeInfo
{
	Vector3d start,end;
	Vector3d normal_front,normal_back;
};

class EField
{
public:
	Vector3d Position;  //���յ�λ��
	std::vector<Field_Path> Path;  //Field_Path ���Ա��all_distance��power_loss��Path_interPoint
	int pathsize;
	double MolStrength;   //�ź�ģֵ
	Vector3cd EFieldAll;  //ʸ���ܳ�ǿ
	bool In_or_Out; //���յ��Ƿ��ڽ������ڲ���������־��0��ʾ���ڲ���1��ʾ�����ڲ�
	//complex<double> InitEFstrength;
	//	int ordernum;  //���յ㴦��ǿ��С������
	//int pathnum; //�ܹ�����ý��յ��·����Ŀ
	//complex<double> EFieldStrength;  
	//complex<double> EFieldTotal;  //ģֵ�ܳ�ǿ
	double HorizontalDis;  //���յ�ͷ���Դ֮��ˮƽ����
	double LosDis; //���յ�ͷ���Դ֮���ֱ�����
	EField(): pathsize(0),MolStrength(0.0),EFieldAll(Vector3cd(0, 0, 0)),In_or_Out(1){}
};
#endif