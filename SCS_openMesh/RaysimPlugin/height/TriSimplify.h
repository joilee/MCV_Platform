#ifndef TRISIM
#define TRISIM


#include <mesh/Mesh.h>
#include <iostream>
#include <math.h>
#include <map>
#include <vector>
#include <queue>
#include <set>
using namespace std;

class SimTri;
class SimEdge
{
public:
	VERTEX2D_PTR v1;
	VERTEX2D_PTR v2;
	int id1,id2;
	vector<SimTri *> triPtr;
	

	bool inQueue;
	bool isDeleted;
	SimEdge(int _id1,int _id2):id1(_id1),id2(_id2)
	{
		
		inQueue=false;
		isDeleted=false;

	}
	
};

class SimTri
{
public:
	int id1;
	int id2;
	int id3;
	SimEdge *e[3];
	bool isDeleted;//记录是否被合并。

	SimTri(int i1,int i2,int i3):id1(i1),id2(i2),id3(i3)
	{
	 isDeleted=false;
	}

	int getAnotherID(int a,int b)
	{
		if(id1!=a&&id1!=b)return id1;
		if(id2!=a&&id2!=b)return id2;
		if(id3!=a&&id3!=b)return id3;
	}

	bool isEqual2(int a[3])
	{//
		bool flag=true;
		set<int> tmp1;
		tmp1.insert(id1);tmp1.insert(id2);tmp1.insert(id3);
		for (int i=0;i<3;i++)
		{
			set<int>::iterator it2=tmp1.find(a[i]);
			if(it2==tmp1.end())//有一个没查询到，于是不相等
			{
				flag=false;
				break;
			}
		}
		return flag;
	}

	bool isEqual(SimTri* other)
	{
		bool flag=true;
		set<int> tmp1,tmp2;
		tmp1.insert(id1);tmp1.insert(id2);tmp1.insert(id3);
		tmp2.insert(other->id1);tmp2.insert(other->id2);tmp2.insert(other->id3);
		for(set<int>::iterator it=tmp1.begin();it!=tmp1.end();it++)
		{
			set<int>::iterator it2=tmp2.find(*it);
			if(it2==tmp2.end())//有一个没查询到，于是不相等
			{
				flag=false;
				break;
			}
		}
		return flag;
	}
	
protected:
private:
};

bool isLine(double x[4],double y[4],int id1,int id2,int id3);
void  simplifyTri(MESH_PTR pMesh);

#endif // !TRISIM
