#ifndef   RDGRID_H 
#define   RDGRID_H  
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include "point.h"
#include <string>
#include "defaultPara.h"
//#include "array2D.h"

using namespace std;

/*******动态申请二维数组***********/
template <typename T>
T** new_Array2D(int row, int col)
{
	int size = sizeof(T);
	int point_size = sizeof(T*);
	//先申请内存，其中sizeof(T*) * row表示存放row个行指针
	T **arr = (T **) malloc(point_size * row + size * row * col);
	if (arr != NULL)
	{	
		T *head = (T*)((int)arr + point_size * row);
		for (int i = 0; i < row; ++i)
		{
			arr[i] =  (T*)((int)head + i * col * size);
			for (int j = 0; j < col; ++j)
				new (&arr[i][j]) T;
		}
	}
	return (T**)arr;
}
/***********释放二维数组*************/
template <typename T>
void delete_Array2D(T **arr, int row, int col)
{
	for (int i = 0; i < row; ++i)
		for (int j = 0; j < col; ++j)
			arr[i][j].~T();
	if (arr != NULL)
		free((void**)arr);
}

//vector<int> height;
class GridAll
{
public:
	int **p;
	int row,col;
	GridAll(int **pp,int roww,int coll)
	{
		p=pp;
		row = roww;
		col = coll;
	}

};
class GridHeight
{
public:
	vector< vector<int> > p;
	int row,col;
	GridHeight(vector< vector<int> > pp,int roww,int coll)
	{
		p=pp;
		row = roww;
		col = coll;
	}

};


void readHeightGrid(string index,vector<GridHeight> &gridR,vector<int>&rowNumber,vector<int> &colNumber,vector<double>&bases,vector<double> &xmins,vector<double>&ymaxs );

double getPointAltitude(GridHeight &gridR,double x,double y,int row,int col,double base,double xmin,double ymax);

//vector<GridAll> gridR;/***height数据vector数组***/
class PotIntxy{
public:
	int x;
	int y;
	PotIntxy(int xx,int yy)
	{
		x=xx;
		y=yy;
	}
	PotIntxy(const PotIntxy &p)
	{
		x = p.x;
		y = p.y;
	}
};
int cmp( PotIntxy a, PotIntxy b);
#endif
