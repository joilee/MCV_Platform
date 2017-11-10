#ifndef TRIMESH
#define TRIMESH
//#include "point.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <cmath>

#include "canny.h"
#include "readGrid.h"

#include "IO_obj.h"
#include <mesh/Mesh.h>
#include "TriSimplify.h"
using namespace std;

double disP2P(Pot p,Pot q);
Pot get_Normal(Pot p1,Pot p2,Pot p3);
void getAdjPoint(vector<Pot> &adjPoints,vector<vector<int> >&heights, int row,int col,double stdlen,int i,int j,double xmin,double ymax);
Pot GetNormalPoint(Pot src,vector<Pot> adjPoint );

void localGetNormalMatrix(vector<vector<double> >& normals,vector<vector<int> > &heights,int startRow,int endRow,int startCol,int endCol,double stdlen,double xmin,double ymax,int row,int col);

void height2canny(GridHeight &heightR,vector<vector<int> > &cannyPoint,int rowNum,int colNum,double stdLen,double xmin,double ymax,int area[4]);

int getFeaturePoint3(int steps,int localRow,int localCol,vector<vector<int> >& cannyPoint);
int setMeshPtr3(MESH_PTR pMesh,int steps,int localRow,int localCol,vector<vector<int> >& cannyPoint,GridHeight &heightR,double xmin,double ymax,double stdLen,int startRow,int startCol);




#endif