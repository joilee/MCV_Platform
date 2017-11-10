#include "IO_obj.h"
#include <iostream>
void WriteOBJ(FILE *f, MESH_PTR pMesh)
{
	//输出点
	VERTEX2D_PTR pVtx=pMesh->pVerArr;
	int numV=pMesh->vertex_num;
	for(int i=3;i<numV+3;i++)
	{
		double x,y,z;
		x=((VERTEX2D_PTR)(pMesh->pVerArr+i))->x;
		y=((VERTEX2D_PTR)(pMesh->pVerArr+i))->y;
		z=((VERTEX2D_PTR)(pMesh->pVerArr+i))->z;
		fprintf(f, "v %f %f %f\n", x,y,z);
	}

#ifdef DEBUG_MODE
	cout<<"point is wrote in file"<<endl;
#endif


	//输出三角形
	int tri_index = 0;
	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int* pi;
	while(pTri != NULL)	
	{
		if (pTri->changeFlag==true)
		{
			pTri = pTri->pNext;
			continue;
		}
		tri_index++;
		int id[3];
		pi = &(pTri->i1);
		for (int j=0; j<3; j++)	
		{	
			id[j] = *pi++;	
			id[j]=id[j]-2;

		}
		fprintf(f, "f %d %d %d\n", id[0], id[1], id[2]); 
#ifdef DEBUG_MODE
		//cout<<"triangle loaded"<<tri_index<<endl;
#endif
		pTri = pTri->pNext;
	}
	UnInitMesh(pMesh);
}

void WriteOBJ_No_simplify(FILE *f, MESH_PTR pMesh)
{
	//输出点
	VERTEX2D_PTR pVtx=pMesh->pVerArr;
	int numV=pMesh->vertex_num;
	for(int i=3;i<numV+3;i++)
	{
		double x,y,z;
		x=((VERTEX2D_PTR)(pMesh->pVerArr+i))->x;
		y=((VERTEX2D_PTR)(pMesh->pVerArr+i))->y;
		z=((VERTEX2D_PTR)(pMesh->pVerArr+i))->z;
		fprintf(f, "v %f %f %f\n", x,y,z);
	}

#ifdef DEBUG_MODE
	cout<<"point is wrote in file"<<endl;
#endif


	//输出三角形
	int tri_index = 0;
	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int* pi;
	while(pTri != NULL)	
	{
		tri_index++;
		int id[3];
		pi = &(pTri->i1);
		for (int j=0; j<3; j++)	
		{	
			id[j] = *pi++;	
			id[j]=id[j]-2;

		}
		fprintf(f, "f %d %d %d\n", id[0], id[1], id[2]); 
#ifdef DEBUG_MODE
		//cout<<"triangle loaded"<<tri_index<<endl;
#endif
		pTri = pTri->pNext;
	}
	UnInitMesh(pMesh);
}