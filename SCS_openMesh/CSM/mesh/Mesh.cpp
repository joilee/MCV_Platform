#include "Mesh.h"
void Input(char* pFile, MESH_PTR pMesh)
{
	FILE* fp = fopen(pFile, "r");

	if (!fp)
	{
		fprintf(stderr, "Error:%s open failed\n", pFile);
		exit(1);
	}

	//int face;
	int amount;

	//fscanf( fp, "%d", &face);
	fscanf(fp, "%d", &amount);
	if (amount < 3)
	{
		fprintf(stderr, "Error:vertex amount should be greater than 2, but it is %d \n", amount);
		exit(1);
	}

	InitMesh(pMesh, amount);

	REAL x, y, z;
	for (int j = 3; j < amount + 3; ++j)
	{
		fscanf(fp, "%lg %lg %lg", &x, &y, &z);
		((VERTEX2D_PTR)(pMesh->pVerArr + j))->x = x;
		((VERTEX2D_PTR)(pMesh->pVerArr + j))->y = y;
	}


	fclose(fp);
}

// Algorithm IncrementalDelaunay(V)
// Input: 由n个点组成的二维点集V
// Output: Delaunay三角剖分DT
//	1.add a appropriate triangle boudingbox to contain V ( such as: we can use triangle abc, a=(0, 3M), b=(-3M,-3M), c=(3M, 0), M=Max({|x1|,|x2|,|x3|,...} U {|y1|,|y2|,|y3|,...}))
//	2.initialize DT(a,b,c) as triangle abc
//	3.for i <- 1 to n 
//		do (Insert(DT(a,b,c,v1,v2,...,vi-1), vi))   
//	4.remove the boundingbox and relative triangle which cotains any vertex of triangle abc from DT(a,b,c,v1,v2,...,vn) and return DT(v1,v2,...,vn).
void IncrementalDelaunay(MESH_PTR pMesh)
{
	// Add a appropriate triangle boudingbox to contain V
	AddBoundingBox(pMesh);

	// Get a vertex/point vi from V and Insert(vi)
	for (int i = 3; i < pMesh->vertex_num + 3; i++)
	{
		Insert(pMesh, i);
	}

	// Remove the bounding box
	RemoveBoundingBox(pMesh);
}

// The format of output file should be as follows:
// triangle index
// x1 y1 (the coordinate of first vertex of triangle)
// x2 y2 (the coordinate of second vertex of triangle)
// x3 y3 (the coordinate of third vertex of triangle)
void Output(char* pFile, MESH_PTR pMesh)
{
	FILE* fp = fopen(pFile, "w");
	if (!fp)
	{
		fprintf(stderr, "Error:%s open failed\n", pFile);

		UnInitMesh(pMesh);
		exit(1);
	}

	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int* pi;
	int vertex_index;
	int tri_index = 0;
	while (pTri != NULL)
	{
		fprintf(fp, "Triangle: %d\n", ++tri_index);

		pi = &(pTri->i1);
		for (int j = 0; j < 3; j++)
		{
			vertex_index = *pi++;
			fprintf(fp, "%lg %lg\n", ((VERTEX2D_PTR)(pMesh->pVerArr + vertex_index))->x, ((VERTEX2D_PTR)(pMesh->pVerArr + vertex_index))->y);
		}

		pTri = pTri->pNext;
	}

	fclose(fp);

	UnInitMesh(pMesh);
}


// Allocate memory to store vertices and triangles
void InitMesh(MESH_PTR pMesh, int ver_num)
{
	// Allocate memory for vertex array
	pMesh->pVerArr = (VERTEX2D_PTR)malloc((ver_num + 3)*sizeof(VERTEX2D));
	if (pMesh->pVerArr == NULL)
	{
		fprintf(stderr, "Error:Allocate memory for mesh failed\n");
		exit(1);
	}

	pMesh->vertex_num = ver_num;

}

// Deallocate memory
void UnInitMesh(MESH_PTR pMesh)
{
	// free vertices
	if (pMesh->pVerArr != NULL)
		free(pMesh->pVerArr);

	// free triangles
	TRIANGLE_PTR pTri = pMesh->pTriArr;
	TRIANGLE_PTR pTemp = NULL;
	while (pTri != NULL)
	{
		pTemp = pTri->pNext;
		free(pTri);
		pTri = pTemp;
	}
}

void AddBoundingBox(MESH_PTR pMesh)
{
	REAL max = 0;
	REAL max_x = 0;
	REAL max_y = 0;
	REAL t;

	for (int i = 3; i < pMesh->vertex_num + 3; i++)
	{
		t = abs(((VERTEX2D_PTR)(pMesh->pVerArr + i))->x);
		if (max_x < t)
		{
			max_x = t;
		}

		t = abs(((VERTEX2D_PTR)(pMesh->pVerArr + i))->y);
		if (max_y < t)
		{
			max_y = t;
		}
	}

	max = max_x > max_y ? max_x : max_y;

	//TRIANGLE box;
	//box.v1 = VERTEX2D(0, 3*max);
	//box.v2 = VERTEX2D(-3*max, 3*max);
	//box.v3 = VERTEX2D(3*max, 0);

	VERTEX2D v1 = { 0, 4 * max };
	VERTEX2D v2 = { -4 * max, -4 * max };
	VERTEX2D v3 = { 4 * max, 0 };

	// Assign to Vertex array
	*(pMesh->pVerArr) = v1;
	*(pMesh->pVerArr + 1) = v2;
	*(pMesh->pVerArr + 2) = v3;

	// add the Triangle boundingbox
	AddTriangleNode(pMesh, NULL, 0, 1, 2);
}

void RemoveBoundingBox(MESH_PTR pMesh)
{
	int statify[3] = { 0, 0, 0 };
	int vertex_index;
	int* pi;
	int k = 1;

	// Remove the first triangle-boundingbox
	//pMesh->pTriArr = pMesh->pTriArr->pNext;
	//pMesh->pTriArr->pPrev = NULL; // as head

	TRIANGLE_PTR pTri = pMesh->pTriArr;
	TRIANGLE_PTR pNext = NULL;
	while (pTri != NULL)
	{
		pNext = pTri->pNext;

		statify[0] = 0;
		statify[1] = 0;
		statify[2] = 0;

		pi = &(pTri->i1);
		for (int j = 0, k = 1; j < 3; j++, k *= 2)
		{
			vertex_index = *pi++;

			if (vertex_index == 0 || vertex_index == 1 || vertex_index == 2) // bounding box vertex
			{
				statify[j] = k;
			}
		}

		switch (statify[0] | statify[1] | statify[2])
		{
		case 0: // no statify
			break;
		case 1:
		case 2:
		case 4: // 1 statify, remove 1 triangle, 1 vertex
			RemoveTriangleNode(pMesh, pTri);
			break;
		case 3:
		case 5:
		case 6: // 2 statify, remove 1 triangle, 2 vertices
			RemoveTriangleNode(pMesh, pTri);
			break;
		case 7: // 3 statify, remove 1 triangle, 3 vertices
			RemoveTriangleNode(pMesh, pTri);
			break;
		default:
			break;
		}

		// go to next item
		pTri = pNext;
	}
}


// Return a positive value if the points pa, pb, and
// pc occur in counterclockwise order; a negative
// value if they occur in clockwise order; and zero
// if they are collinear. The result is also a rough
// approximation of twice the signed area of the
// triangle defined by the three points.
REAL CounterClockWise(VERTEX2D_PTR pa, VERTEX2D_PTR pb, VERTEX2D_PTR pc)
{
	return ((pb->x - pa->x)*(pc->y - pb->y) - (pc->x - pb->x)*(pb->y - pa->y));
}

// Adjust if the point lies in the triangle abc
REAL InTriangle(MESH_PTR pMesh, VERTEX2D_PTR pVer, TRIANGLE_PTR pTri)
{
	int vertex_index;
	VERTEX2D_PTR pV1, pV2, pV3;

	vertex_index = pTri->i1;
	pV1 = (VERTEX2D_PTR)(pMesh->pVerArr + vertex_index);
	vertex_index = pTri->i2;
	pV2 = (VERTEX2D_PTR)(pMesh->pVerArr + vertex_index);
	vertex_index = pTri->i3;
	pV3 = (VERTEX2D_PTR)(pMesh->pVerArr + vertex_index);

	REAL ccw1 = CounterClockWise(pV1, pV2, pVer);
	REAL ccw2 = CounterClockWise(pV2, pV3, pVer);
	REAL ccw3 = CounterClockWise(pV3, pV1, pVer);

	REAL r = -1;
	if (ccw1 > 0 && ccw2 > 0 && ccw3 > 0)
	{
		r = 1;
	}
	else if (ccw1*ccw2*ccw3 == 0 && (ccw1*ccw2 > 0 || ccw1*ccw3 > 0 || ccw2*ccw3 > 0))
	{
		r = 0;
	}

	return r;
}

// Algorithm Insert(DT(a,b,c,v1,v2,...,vi-1), vi)
// 1.find the triangle vavbvc which contains vi // FindTriangle()
// 2.if (vi located at the interior of vavbvc)  
// 3.    then add triangle vavbvi, vbvcvi and vcvavi into DT // UpdateDT()
// FlipTest(DT, va, vb, vi)
// FlipTest(DT, vb, vc, vi)
// FlipTest(DT, vc, va, vi)
// 4.else if (vi located at one edge (E.g. edge vavb) of vavbvc) 
// 5.    then add triangle vavivc, vivbvc, vavdvi and vivdvb into DT (here, d is the third vertex of triangle which contains edge vavb) // UpdateDT()
// FlipTest(DT, va, vd, vi)
// FlipTest(DT, vc, va, vi)
// FlipTest(DT, vd, vb, vi)
// FlipTest(DT, vb, vc, vi)
// 6.return DT(a,b,c,v1,v2,...,vi)
void Insert(MESH_PTR pMesh, int ver_index)
{
	VERTEX2D_PTR pVer = (VERTEX2D_PTR)(pMesh->pVerArr + ver_index);
	TRIANGLE_PTR pTargetTri = NULL;
	TRIANGLE_PTR pEqualTri1 = NULL;
	TRIANGLE_PTR pEqualTri2 = NULL;

	int j = 0;
	TRIANGLE_PTR pTri = pMesh->pTriArr;
	while (pTri != NULL)
	{
		REAL r = InTriangle(pMesh, pVer, pTri);
		if (r > 0) // should be in triangle
		{
			pTargetTri = pTri;
		}
		else if (r == 0) // should be on edge
		{
			if (j == 0)
			{
				pEqualTri1 = pTri;
				j++;
			}
			else
			{
				pEqualTri2 = pTri;
			}

		}

		pTri = pTri->pNext;
	}

	if (pEqualTri1 != NULL && pEqualTri2 != NULL)
	{
		InsertOnEdge(pMesh, pEqualTri1, ver_index);
		InsertOnEdge(pMesh, pEqualTri2, ver_index);
	}
	else
	{
		InsertInTriangle(pMesh, pTargetTri, ver_index);
	}
}

void InsertInTriangle(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index)
{
	int index_a, index_b, index_c;
	TRIANGLE_PTR pTri = NULL;
	TRIANGLE_PTR pNewTri = NULL;

	pTri = pTargetTri;
	if (pTri == NULL)
	{
		return;
	}

	// Inset p into target triangle
	index_a = pTri->i1;
	index_b = pTri->i2;
	index_c = pTri->i3;

	// Insert edge pa, pb, pc
	for (int i = 0; i < 3; i++)
	{
		// allocate memory
		if (i == 0)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_a, index_b, ver_index);
		}
		else if (i == 1)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_b, index_c, ver_index);
		}
		else
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_c, index_a, ver_index);
		}

		// go to next item
		if (pNewTri != NULL)
		{
			pTri = pNewTri;
		}
		else
		{
			pTri = pTri;
		}
	}

	// Get the three sub-triangles
	pTri = pTargetTri;
	TRIANGLE_PTR pTestTri[3];
	for (int i = 0; i < 3; i++)
	{
		pTestTri[i] = pTri->pNext;

		pTri = pTri->pNext;
	}

	// remove the Target Triangle
	RemoveTriangleNode(pMesh, pTargetTri);

	for (int i = 0; i < 3; i++)
	{
		// Flip test
		FlipTest(pMesh, pTestTri[i]);
	}
}

void InsertOnEdge(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index)
{
	int index_a, index_b, index_c;
	TRIANGLE_PTR pTri = NULL;
	TRIANGLE_PTR pNewTri = NULL;

	pTri = pTargetTri;
	if (pTri == NULL)
	{
		return;
	}

	// Inset p into target triangle
	index_a = pTri->i1;
	index_b = pTri->i2;
	index_c = pTri->i3;

	// Insert edge pa, pb, pc
	for (int i = 0; i < 3; i++)
	{
		// allocate memory
		if (i == 0)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_a, index_b, ver_index);
		}
		else if (i == 1)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_b, index_c, ver_index);
		}
		else
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_c, index_a, ver_index);
		}

		// go to next item
		if (pNewTri != NULL)
		{
			pTri = pNewTri;
		}
		else
		{
			pTri = pTri;
		}
	}

	// Get the two sub-triangles
	pTri = pTargetTri;
	TRIANGLE_PTR pTestTri[2];
	for (int i = 0; i < 2; i++)
	{
		pTestTri[i] = pTri->pNext;
		pTri = pTri->pNext;
	}

	// remove the Target Triangle
	RemoveTriangleNode(pMesh, pTargetTri);

	for (int i = 0; i < 2; i++)
	{
		// Flip test
		FlipTest(pMesh, pTestTri[i]);
	}
}

// Precondition: the triangle satisfies CCW order
// Algorithm FlipTest(DT(a,b,c,v1,v2,...,vi), va, vb, vi)
// 1.find the third vertex (vd) of triangle which contains edge vavb // FindThirdVertex()
// 2.if(vi is in circumcircle of abd)  // InCircle()
// 3.    then remove edge vavb, add new edge vivd into DT // UpdateDT()
//		  FlipTest(DT, va, vd, vi)
//		  FlipTest(DT, vd, vb, vi)

bool FlipTest(MESH_PTR pMesh, TRIANGLE_PTR pTestTri)
{
	bool flipped = false;

	int index_a = pTestTri->i1;
	int index_b = pTestTri->i2;
	int index_p = pTestTri->i3;

	int statify[3] = { 0, 0, 0 };
	int vertex_index;
	int* pi;
	int k = 1;

	// find the triangle which has edge consists of start and end
	TRIANGLE_PTR pTri = pMesh->pTriArr;

	int index_d = -1;
	while (pTri != NULL)
	{
		statify[0] = 0;
		statify[1] = 0;
		statify[2] = 0;

		pi = &(pTri->i1);
		for (int j = 0, k = 1; j < 3; j++, k *= 2)
		{
			vertex_index = *pi++;
			if (vertex_index == index_a || vertex_index == index_b)
			{
				statify[j] = k;
			}
		}

		switch (statify[0] | statify[1] | statify[2])
		{
		case 3:
			if (CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr + index_a), (VERTEX2D_PTR)(pMesh->pVerArr + index_b), (VERTEX2D_PTR)(pMesh->pVerArr + pTri->i3)) < 0)
			{
				index_d = pTri->i3;
			}

			break;
		case 5:
			if (CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr + index_a), (VERTEX2D_PTR)(pMesh->pVerArr + index_b), (VERTEX2D_PTR)(pMesh->pVerArr + pTri->i2)) < 0)
			{
				index_d = pTri->i2;
			}

			break;
		case 6:
			if (CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr + index_a), (VERTEX2D_PTR)(pMesh->pVerArr + index_b), (VERTEX2D_PTR)(pMesh->pVerArr + pTri->i1)) < 0)
			{
				index_d = pTri->i1;
			}

			break;

		default:
			break;
		}

		if (index_d != -1)
		{
			VERTEX2D_PTR pa = (VERTEX2D_PTR)(pMesh->pVerArr + index_a);
			VERTEX2D_PTR pb = (VERTEX2D_PTR)(pMesh->pVerArr + index_b);
			VERTEX2D_PTR pd = (VERTEX2D_PTR)(pMesh->pVerArr + index_d);
			VERTEX2D_PTR pp = (VERTEX2D_PTR)(pMesh->pVerArr + index_p);

			if (InCircle(pa, pb, pp, pd) < 0) // not local Delaunay
			{
				flipped = true;

				// add new triangle adp,  dbp, remove abp, abd.
				// allocate memory for adp
				TRIANGLE_PTR pT1 = AddTriangleNode(pMesh, pTestTri, pTestTri->i1, index_d, pTestTri->i3);
				// allocate memory for dbp
				TRIANGLE_PTR pT2 = AddTriangleNode(pMesh, pT1, index_d, pTestTri->i2, index_p);
				// remove abp
				RemoveTriangleNode(pMesh, pTestTri);
				// remove abd
				RemoveTriangleNode(pMesh, pTri);

				FlipTest(pMesh, pT1); // pNewTestTri satisfies CCW order
				FlipTest(pMesh, pT2); // pNewTestTri2  satisfies CCW order

				break;
			}
		}

		// go to next item	
		pTri = pTri->pNext;
	}

	return flipped;
}

// In circle test, use vector cross product
REAL InCircle(VERTEX2D_PTR pa, VERTEX2D_PTR pb, VERTEX2D_PTR pp, VERTEX2D_PTR  pd)
{
	REAL det;
	REAL alift, blift, plift, bdxpdy, pdxbdy, pdxady, adxpdy, adxbdy, bdxady;

	REAL adx = pa->x - pd->x;
	REAL ady = pa->y - pd->y;

	REAL bdx = pb->x - pd->x;
	REAL bdy = pb->y - pd->y;

	REAL pdx = pp->x - pd->x;
	REAL pdy = pp->y - pd->y;

	bdxpdy = bdx * pdy;
	pdxbdy = pdx * bdy;
	alift = adx * adx + ady * ady;

	pdxady = pdx * ady;
	adxpdy = adx * pdy;
	blift = bdx * bdx + bdy * bdy;

	adxbdy = adx * bdy;
	bdxady = bdx * ady;
	plift = pdx * pdx + pdy * pdy;

	det = alift * (bdxpdy - pdxbdy)
		+ blift * (pdxady - adxpdy)
		+ plift * (adxbdy - bdxady);

	return -det;
}

// Remove a node from the triangle list and deallocate the memory
void RemoveTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pTri)
{
	if (pTri == NULL)
	{
		return;
	}

	// remove from the triangle list
	if (pTri->pPrev != NULL)
	{
		pTri->pPrev->pNext = pTri->pNext;
	}
	else // remove the head, need to reset the root node
	{
		pMesh->pTriArr = pTri->pNext;
	}

	if (pTri->pNext != NULL)
	{
		pTri->pNext->pPrev = pTri->pPrev;
	}

	// deallocate memory
	free(pTri);
}

// Create a new node and add it into triangle list
TRIANGLE_PTR AddTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pPrevTri, int i1, int i2, int i3)
{
	// test if 3 vertices are co-linear
	if (CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr + i1), (VERTEX2D_PTR)(pMesh->pVerArr + i2), (VERTEX2D_PTR)(pMesh->pVerArr + i3)) == 0)
	{
		return NULL;
	}

	// allocate memory
	TRIANGLE_PTR pNewTestTri = (TRIANGLE_PTR)malloc(sizeof(TRIANGLE));

	pNewTestTri->i1 = i1;
	pNewTestTri->i2 = i2;
	pNewTestTri->i3 = i3;

	// insert after prev triangle
	if (pPrevTri == NULL) // add root
	{
		pMesh->pTriArr = pNewTestTri;
		pNewTestTri->pNext = NULL;
		pNewTestTri->pPrev = NULL;
	}
	else
	{
		pNewTestTri->pNext = pPrevTri->pNext;
		pNewTestTri->pPrev = pPrevTri;

		if (pPrevTri->pNext != NULL)
		{
			pPrevTri->pNext->pPrev = pNewTestTri;
		}

		pPrevTri->pNext = pNewTestTri;
	}

	return pNewTestTri;
}

// Create a new node and add it into triangle list
TRIANGLE_PTR AddTriangleNode2(MESH_PTR pMesh, TRIANGLE_PTR pPrevTri, int i1, int i2, int i3)
{
	// test if 3 vertices are co-linear
	if (CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr + i1), (VERTEX2D_PTR)(pMesh->pVerArr + i2), (VERTEX2D_PTR)(pMesh->pVerArr + i3)) == 0)
	{
		return NULL;
	}

	// allocate memory
	TRIANGLE_PTR pNewTestTri = (TRIANGLE_PTR)malloc(sizeof(TRIANGLE));

	pNewTestTri->i1 = i1;
	pNewTestTri->i2 = i2;
	pNewTestTri->i3 = i3;
	pNewTestTri->changeFlag = false;
	// insert after prev triangle
	if (pPrevTri == NULL) // add root
	{
		pMesh->pTriArr = pNewTestTri;
		pNewTestTri->pNext = NULL;
		pNewTestTri->pPrev = NULL;
	}
	else
	{
		pNewTestTri->pNext = pPrevTri->pNext;
		pNewTestTri->pPrev = pPrevTri;

		if (pPrevTri->pNext != NULL)
		{
			pPrevTri->pNext->pPrev = pNewTestTri;
		}

		pPrevTri->pNext = pNewTestTri;
	}

	return pNewTestTri;
}