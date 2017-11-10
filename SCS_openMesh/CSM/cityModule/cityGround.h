#pragma once
#include <string>
#include <vector>
#include "../util/vector.h"
using namespace std;
class cityGround
{
private:
	//地面栅格 
	std::vector< std::vector<int>> groundMatrix;
	int row, col;//地面的行和列
	double base;//精度
	double xmin,xmax;
	double ymax,ymin;
public:
	cityGround(const string path, double _xmin, double _xmax, double _ymin, double _ymax, double _base);
	cityGround(const cityGround &cg, Vector3d center, double range);
	~cityGround();
	std::vector< std::vector<int>> getGroundMatrix(){ return this->groundMatrix; }
	double getXmin() const { return this->xmin; }
	double getXmax() const { return this->xmax; }
	double getYmin() const {  return this->ymin; }
	double getYmax() const { return this->ymax; }
	

	int getRow(){ return this->row; }
	int getCol() { return this->col; }
	double getPrecesion() const { return this->base; }
	double getPointAltitude(double x, double y);//根据坐标得到海拔高度值
	int getMatrix(int x, int y)const;//根据行列数得到对应坐标值
};