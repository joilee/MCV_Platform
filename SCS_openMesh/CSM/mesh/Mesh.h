#ifndef H_MESH
#define H_MESH

//
// File: Delaunay.cpp
// Description: Delaunay class to triangluate points set in 2D. 
// TODO: The procedure uses Double List for holding data, it can be optimized by using another data structure such as DAG, Quad-edge, etc.
// Author: SoRoMan
// Date: 05/10/2007
// Version: 1.0
// History: 
//

// INCLUDES ///////////////////////////////////////////////
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

#include "meshStruct.h"
#ifndef NOMINMAX                       
#  define NOMINMAX
#endif
#include "windows.h" // for time statistics

// DEFINES ////////////////////////////////////////////////
#define MAX_VERTEX_NUM 540000




// PROTOTYPES ///////////////////////////////////////////
// Delaunay triangulation functions
void InitMesh(MESH_PTR pMesh, int ver_num);
void UnInitMesh(MESH_PTR pMesh);

void AddBoundingBox(MESH_PTR pMesh);
void RemoveBoundingBox(MESH_PTR pMesh);;
void IncrementalDelaunay(MESH_PTR pMesh);

void Insert(MESH_PTR pMesh, int ver_index);
bool FlipTest(MESH_PTR pMesh, TRIANGLE_PTR pTestTri);

REAL InCircle(VERTEX2D_PTR pa, VERTEX2D_PTR pb, VERTEX2D_PTR pp, VERTEX2D_PTR  pd);
REAL InTriangle(MESH_PTR pMesh, VERTEX2D_PTR pVer, TRIANGLE_PTR pTri);

void InsertInTriangle(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index);
void InsertOnEdge(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index);

// Helper functions
void RemoveTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pTri);
TRIANGLE_PTR AddTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pPrevTri, int i1, int i2, int i3);

// I/O functions
void Input(char* pFile, MESH_PTR pMesh);
void Output(char* pFile, MESH_PTR pMesh);

TRIANGLE_PTR AddTriangleNode2(MESH_PTR pMesh, TRIANGLE_PTR pPrevTri, int i1, int i2, int i3);
// GLOBALS ////////////////////////////////////////////////

//
//// FUNCTIONS //////////////////////////////////////////////
//int main(int argc, char** argv)
//{
//	if(argc!=3)
//	{
//		fprintf(stderr,"Usage:%s InputFileName OutputFileName\n",argv[0]);
//		exit(1);
//	}
//
//	MESH mesh;
//	double last_time, this_time;
//	//int ver_num;
//	//int tri_num;
//
//	Input(argv[1], &mesh);
//
//	last_time = GetTickCount();		
//
//	IncrementalDelaunay(&mesh);
//	//Sleep(1000);
//	this_time = GetTickCount();	
//
//	printf("Elapsed Time for Incremental Delaunay: %lg ms", this_time - last_time);
//	
//	Output(argv[2], &mesh);
//
//	return 0;
//}

// The format of input file should be as follows:
// The First Line: amount of vertices (the amount of vertices/points needed to be triangulated)
// Other Lines: x y z (the vertices/points coordinates, z should be 0 for 2D)
// E.g. 
// 4
// -1 -1 0
// -1 1 0
// 1 1 0
// 1 -1 0

#endif