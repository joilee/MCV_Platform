#include "point.h"

double ComputeTriangleArea(Pot PotA_, Pot PotB_, Pot PotC_)
{
	//依据两个向量的叉乘来计算，可参考http://blog.csdn.net/zxj1988/article/details/6260576
	Pot AB = PotB_.Minus(PotA_);
	Pot BC = PotC_.Minus(PotB_);
	return fabs(AB.CrossProduct(BC) / 2.0);
}



bool IsPotInTriangle3(Pot PotA_, Pot PotB_, Pot PotC_,const Pot PotP)
{
	Pot AB = PotB_.Minus(PotA_);
	Pot AC = PotC_.Minus(PotA_);
	Pot AP = PotP.Minus(PotA_);
	double dot_ac_ac = AC.DotProduct(AC);
	double dot_ac_ab = AC.DotProduct(AB);
	double dot_ac_ap = AC.DotProduct(AP);
	double dot_ab_ab = AB.DotProduct(AB);
	double dot_ab_ap = AB.DotProduct(AP);

	double tmp = 1.0 / (dot_ac_ac * dot_ab_ab - dot_ac_ab * dot_ac_ab);

	double u = (dot_ab_ab * dot_ac_ap - dot_ac_ab * dot_ab_ap) * tmp;
	if(u < 0 || u > 1)
		return false;
	double v = (dot_ac_ac * dot_ab_ap - dot_ac_ab * dot_ac_ap) * tmp;
	if(v < 0 || v > 1)
		return false;

	return u + v <= 1;
}

