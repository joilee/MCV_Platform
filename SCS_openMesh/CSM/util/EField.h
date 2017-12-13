#ifndef EFIELD_H
#define EFIELD_H
#include "vector.h"
#include <complex>
#include <vector>
#include <map>
#define MinMol -1000
class Field_Path
{
public:
	double all_distance;
	double power_Loss;
	vector<Vector3d> Path_interPoint;	
	vector<int> propagation_type;   //0代表反射，1代表透射，2代表绕射 
	vector<Vector3d> intersect_faceNormal;  //反透射路径相交面片的法向量
	vector<int>Intersection_FaceID; //反透射路径相交面片编号
	int edge_id;   //绕射路径经过的棱边id号
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
	Vector3d Position;  //接收点位置
	std::vector<Field_Path> Path;  //Field_Path 类成员：all_distance、power_loss、Path_interPoint
	int pathsize;//显示专用
	double MolStrength;   //信号模值
	Vector3cd EFieldAll;  //矢量总场强
	bool In_or_Out; //接收点是否在建筑物内部的特征标志，0表示在内部，1表示不在内部
	//complex<double> InitEFstrength;
	//	int ordernum;  //接收点处场强大小的排序
	//int pathnum; //能够到达该接收点的路径数目
	//complex<double> EFieldStrength;  
	//complex<double> EFieldTotal;  //模值总场强
	double HorizontalDis;  //接收点和发射源之间水平距离
	double LosDis; //接收点和发射源之间的直达距离
	EField(): pathsize(0),MolStrength(0.0),EFieldAll(Vector3cd(0, 0, 0)),In_or_Out(1){}
	EField(const EField& a)
	{
		Position = a.Position;
		Path = a.Path;
		pathsize = a.pathsize;
		MolStrength = a.MolStrength;
		EFieldAll = a.EFieldAll;
		In_or_Out = a.In_or_Out;
		HorizontalDis = a.HorizontalDis;
		LosDis = a.LosDis;
	}

};


/*
保存每个Cell得到的结果数据，
*/
class Cell_Data
{
public:
	vector<EField*> efildVec;//一维数据
	int pci;//站点id
	string cell_name;
	int row;//y轴方向的跨度
	int col;//x轴方向的跨度
	int pricision;//地图精度 一般是5m
	Cell_Data(int PCI,string name="");
	~Cell_Data();
};

class Site_Data
{
public:
	Site_Data(int id);
	~Site_Data();
	int siteID;
	//一个站点下面，有多个cell，每个cell有独特的pci标识
	map<int, Cell_Data*> cellsMap;
private:
	void init();
};


#endif