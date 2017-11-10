
#ifndef POT 
#define POT
#include <math.h>
class Pot{
public:
	double x;
	double y;
	double z;
	
	bool spec;//��͹�ԣ�͹1����0
	int mark;//������
	Pot(double xx,double yy)
	{
		x =xx;
		y =yy;
		spec = true;
	}
	Pot(double xx,double yy,double zz)
	{
		x =xx;
		y =yy;
		z =zz;
	}
	Pot()
	{
		x=0;y=0;z=0;
	}
	Pot(double xx,double yy,int mm,double zz)
	{
		x =xx;
		y =yy;
		z =zz;
		mark = mm;
		spec = true;
	}

	double getLength()
	{
	return sqrt(x*x+y*y+z*z);
	}
	void setSpec0()
	{
		spec = false;
	}
	void setSpec1()
	{
		spec = true;
	}

	//��ά�������, ��˵Ľ����ʵ������������ֱ������������ɵ�ƽ�棬��������ֻ��Ҫ���С�ͷ���
	double CrossProduct(const Pot vec)
	{
		return x*vec.y - y*vec.x;
	}

	double DotProduct(const Pot vec)
	{
		return x* vec.x+ y * vec.y;
	}

	//��ά��������
	Pot Minus(const Pot vec) const
	{
		return Pot(x- vec.x, y - vec.y);
	}

	//�жϵ�M,N�Ƿ���ֱ��AB��ͬһ��
	static bool IsPotAtSameSideOfLine(const Pot &PotM, const Pot &PotN, const Pot &PotA, const Pot &PotB)
	{
		Pot AB = PotB.Minus(PotA);
		Pot AM = PotM.Minus(PotA);
		Pot AN = PotN.Minus(PotA);

		//����0ʱ��ʾĳ������ֱ����
		return AB.CrossProduct(AM) * AB.CrossProduct(AN) >= 0;
	}
};

double ComputeTriangleArea(Pot PotA_, Pot PotB_, Pot PotC_);
bool IsPotInTriangle3(Pot PotA_, Pot PotB_, Pot PotC_,const Pot PotP);
#endif