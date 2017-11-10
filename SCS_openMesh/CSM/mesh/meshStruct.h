

#ifndef MESHSTRUCT
#define MESHSTRUCT

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif


// TYPES //////////////////////////////////////////////////
typedef struct VERTEX2D_TYP
{
	REAL x;
	REAL y;
	REAL z;// added by ligen
} VERTEX2D, *VERTEX2D_PTR;

typedef struct EDGE_TYP
{
	VERTEX2D v1;
	VERTEX2D v2;

} EDGE, *EDGE_PTR;

typedef struct TRIANGLE_TYP
{
	int i1; // vertex index
	int i2; 
	int i3; 

	TRIANGLE_TYP* pNext;
	TRIANGLE_TYP* pPrev;

	bool changeFlag;

} TRIANGLE, *TRIANGLE_PTR;

typedef struct MESH_TYP
{
	int vertex_num;
	int triangle_num;

	VERTEX2D_PTR pVerArr; // point to outer vertices arrary
	TRIANGLE_PTR pTriArr; // point to outer triangles arrary

} MESH, *MESH_PTR;

#endif