#include "cityGround.h"
#include <fstream>
#include <iostream>
using namespace std;
cityGround::cityGround(const string path, double _xmin, double _xmax, double _ymin, double _ymax, double _base)
{
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;
	base = _base;

	ifstream FileH;
	FileH.open(path, ios::binary);
	//获取行列值
	row = int((ymax - ymin) / base);
	col  = int((xmax - xmin) / base);

	groundMatrix.resize(row, vector<int>(col));

	unsigned char c1, c2;
	char c3[2];
	const int Nc = 80;
	int no = 0, flag = 1;
	int num = 0;
	int colt = col;
	while (FileH.good() && !FileH.eof())
	{
		FileH.read(c3, 2);
		c1 = c3[0];
		c2 = c3[1];

		int tmp = int(c1) * 256 + int(c2);
		if (no <row*col)
		{
			groundMatrix[no / colt][no%colt] = tmp;
			no++;
		}
	}
	FileH.close();
}


//构造函数 从一个整体中取一个局部
cityGround::cityGround(const cityGround &cg, Vector3d AP_position, double LocalRange)
{
	//局部区域的范围 MinPos、MaxPos
	Vector3d MinPos = AP_position - Vector3d(LocalRange / 2, LocalRange / 2, 0);
	Vector3d MaxPos = AP_position + Vector3d(LocalRange / 2, LocalRange / 2, 0);

	int upRow = (int)( (cg.getYmax() - MaxPos[1]) / cg.getPrecesion() );
	int downRow = (int)((cg.getYmax() - MinPos[1]) / cg.getPrecesion()) + 1;
	int leftCol = (int)((MinPos[0] - cg.getXmin()) / cg.getPrecesion());
	int rightCol = (int)((MaxPos[0] - cg.getXmin()) / cg.getPrecesion())+ 1;
	
	//取栅格数据
	row = downRow - upRow + 1;
	col = rightCol - leftCol + 1;
	base = cg.getPrecesion();
	xmin = cg.getXmin() + leftCol*base;
	xmax = cg.getXmin() +rightCol*base;
	ymax = cg.getYmax() - upRow*base;
	ymin = cg.getYmax() - downRow*base;
	groundMatrix.resize(row, vector<int>(col));
	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col;j++)
		{
			groundMatrix[i][j] = cg.getMatrix(i + upRow, j + leftCol);
		}
	}
	cout << "Info: 局部地面的栅格数据 获取成功。" << endl;
}

double  cityGround::getPointAltitude(double x, double y)
{
	if (x<xmin || y>ymax || x > xmax || y < ymin)
	{
		cout << "error:  wrong coordinates from building vector" << endl;
		return 0;
	}
	int pointRow = int((ymax - y) / base);
	int pointCol = int((x - xmin) / base);
	return groundMatrix[pointRow][pointCol];
}

cityGround::~cityGround()
{
	groundMatrix.clear();
}

int cityGround::getMatrix (int x, int y)const
{
	if (x>=0&&x<col&&y>=0&&y<row)
	{
		return this->groundMatrix[x][y];
	}
	cout << "error: wrong x,y in the function ---->int cityGround:: getMatrix(int x, int y)" << endl;
	return -100;
}