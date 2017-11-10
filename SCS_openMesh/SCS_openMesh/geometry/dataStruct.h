//���ṩ�ĳ���������Ϣ����ת�����õ�����ʹ�õ�OBJ��ʽ��Ϣ���Լ�����ʱ����������Ϣ

#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include "util/vector.h"
#include "util/Color.h"
#include <vector>
#include <iostream>
using namespace std;

class Color;

struct  material
{
	int Id; //���ʱ��
	string name; //��������
	double thickness; //���ʺ��
	Color color; //������ɫ
	double frequency;  //�������Զ�Ӧ��Ƶ��
	double dielectric; //������Խ�糣��
	double permeability; //���ʴŵ��ʣ�һ�������Ϊ1
	double conductivity; //���ʵ絼��
	double Transmission_Loss; //����͸����ģ���λdB
	double Reflection_Loss; //���ʷ�����ģ���λdB
};



#endif