#pragma once
#include <string>
#include <vector>
#include "../util/vector.h"
#include "Configuration.h"
using namespace std;
class cityGround
{
private:
	//����դ�� 
	std::vector< std::vector<int>> groundMatrix;
	int row, col;//������к���
	double base;//����
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
	double getPointAltitude(double x, double y);//��������õ����θ߶�ֵ
	int getMatrix(int x, int y)const;//�����������õ���Ӧ����ֵ
};