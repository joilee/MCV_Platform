//源beam构建模型，球内接正20面体，通过多次细分可得到更加密集的beam
#include "geometry/emxFace.h"
#include "geometry/emxVertex.h"
#include "SphereBeam.h"
#include "util/emxRenderUtility.h"
#include "util/emxUtilityInc.h"
using namespace std;
SphereBeam::~SphereBeam()
{
	for(size_t i = 0 ; i < m_Vertex.size(); i++)
	{
		delete m_Vertex[i];
	}
	m_Vertex.clear();
	for(size_t i = 0; i < m_Face.size(); i++){
		m_Face[i]->vertex[0] = NULL;
		m_Face[i]->vertex[1] = NULL;
		m_Face[i]->vertex[2] = NULL;
	    m_Face[i] = NULL;
	}
	m_Face.clear();
}

emxVertex* SphereBeam::SphereNewVertex(line tempLine, double radius)
{
	Vector3d p1 = tempLine.p1->GetPos();
	Vector3d p2 = tempLine.p2->GetPos();
	Vector3d mid = p1 + p2;
	mid *= 0.5;
	Vector3d dir = mid;//方向
	dir = dir.normalize();
	Vector3d dis = dir * radius;
	mid = dis;
	return new emxVertex(mid.x, mid.y, mid.z);
}

int SphereBeam::LineIndex(emxVertex* p1, emxVertex* p2)
{
	for(size_t i = 0; i < Lines.size(); ++i) {
		if((Lines[i].p1 == p1 && Lines[i].p2 == p2) || (Lines[i].p1 == p2 && Lines[i].p2 == p1))
			return static_cast<int>(i);
	}
	line l;
	l.p1 = p1; 
	l.p2 = p2; 
	l.middle = NULL; 
	Lines.push_back(l);
	return static_cast<int>(Lines.size() - 1);
}

void SphereBeam::SphereInit(int desity, double radius, vector<emxVertex *> &vertexVec)
{
	//以坐标轴分八区，作为初始源beam
	Vector3d positions[] = { Vector3d(1, 0, 0), Vector3d(0, 1, 0), Vector3d(-1,0,0) , Vector3d(0,-1,0), Vector3d(0,0,1), Vector3d(0,0,-1) };
	for (size_t i = 0; i < 6; ++i)
	{
		Vector3d pos = positions[i].normalize();
		pos = pos * radius;
		vertexVec.push_back(new emxVertex(pos.x, pos.y, pos.z));
	}

	size_t faces[][3] = { 0,1,4, 1,2,4,2,3,4,3,0,4,0,3,5,3,2,5,2,1,5,1,0,5};
	face f;
	for (size_t i = 0; i < 8; ++i)
	{
		f.l1 = LineIndex(vertexVec[faces[i][0]], vertexVec[faces[i][1]]);
		f.l2 = LineIndex(vertexVec[faces[i][1]], vertexVec[faces[i][2]]);
		f.l3 = LineIndex(vertexVec[faces[i][2]], vertexVec[faces[i][0]]);
		f.p1 = vertexVec[faces[i][0]];
		f.p2 = vertexVec[faces[i][1]];
		f.p3 = vertexVec[faces[i][2]];
		Faces.push_back(f);
	}

	//// 初始化模型为一个二十面体  12个顶点、20个面、30条边  球心即正20面体的中心在原点处，则球上点的坐标即可表示射线的方向向量
	//double phi = 0.5 * (1.0 + sqrt(5.0));
	//Vector3d positions[] = { Vector3d(0, -1, phi), Vector3d(0, 1, phi), Vector3d(0, -1, -phi), Vector3d(0, 1, -phi),
	//	Vector3d(phi, 0,  1), Vector3d(phi, 0, -1), Vector3d(-phi, 0,  1), Vector3d(-phi, 0, -1), 
	//	Vector3d(1,  phi, 0), Vector3d(1, -phi, 0), Vector3d(-1,  phi, 0), Vector3d(-1, -phi, 0) };
	//for(size_t i = 0; i < 12; ++i) 
	//{
	//	Vector3d pos = positions[i].normalize();
	//	pos =  pos * radius;
	//	vertexVec.push_back(new emxVertex(pos.x, pos.y, pos.z));
	//}

	//size_t faces[][3] = { 0, 4, 1, 0, 9, 4, 9, 5, 4, 4, 5, 8, 4, 8, 1, 8, 10, 1, 8, 3, 10, 5, 3, 8, 5, 2, 3, 
	//	2, 7, 3, 7, 10, 3, 7, 6, 10, 7, 11, 6, 11, 0, 6, 0, 1, 6, 6, 1, 10, 9, 0, 11, 
	//	9, 11, 2, 9, 2, 5,7, 2, 11 };
	//face f;
	//for(size_t i = 0; i < 20; ++i) 
	//{
	//	f.l1 = LineIndex(vertexVec[faces[i][0]], vertexVec[faces[i][1]]);
	//	f.l2 = LineIndex(vertexVec[faces[i][1]], vertexVec[faces[i][2]]);
	//	f.l3 = LineIndex(vertexVec[faces[i][2]], vertexVec[faces[i][0]]);
	//	f.p1 = vertexVec[faces[i][0]]; 
	//	f.p2 = vertexVec[faces[i][1]]; 
	//	f.p3 = vertexVec[faces[i][2]];
	//	Faces.push_back(f);
	//}

	//if (desity == -1)
	//{
	//	Vector3d positions[] = { Vector3d(1, 0, 0), Vector3d(0, 1, 0), Vector3d(-1,0,0) , Vector3d(0,-1,0), Vector3d(0,0,1), Vector3d(0,0,-1) };
	//	for (size_t i = 0; i < 6; ++i)
	//	{
	//		Vector3d pos = positions[i].normalize();
	//		pos = pos * radius;
	//		vertexVec.push_back(new emxVertex(pos.x, pos.y, pos.z));
	//	}

	//	size_t faces[][3] = { 0,1,4, 1,2,4,2,3,4,3,0,4,0,3,5,3,2,5,2,1,5,1,0,5};
	//	face f;
	//	for (size_t i = 0; i < 8; ++i)
	//	{
	//		f.l1 = LineIndex(vertexVec[faces[i][0]], vertexVec[faces[i][1]]);
	//		f.l2 = LineIndex(vertexVec[faces[i][1]], vertexVec[faces[i][2]]);
	//		f.l3 = LineIndex(vertexVec[faces[i][2]], vertexVec[faces[i][0]]);
	//		f.p1 = vertexVec[faces[i][0]];
	//		f.p2 = vertexVec[faces[i][1]];
	//		f.p3 = vertexVec[faces[i][2]];
	//		Faces.push_back(f);
	//	}
	//}
	//else    // 初始化模型为一个二十面体  12个顶点、20个面、30条边  球心即正20面体的中心在原点处，则球上点的坐标即可表示射线的方向向量
	//{
	//	double phi = 0.5 * (1.0 + sqrt(5.0));
	//	Vector3d positions[] = { Vector3d(0, -1, phi), Vector3d(0, 1, phi), Vector3d(0, -1, -phi), Vector3d(0, 1, -phi),
	//		Vector3d(phi, 0,  1), Vector3d(phi, 0, -1), Vector3d(-phi, 0,  1), Vector3d(-phi, 0, -1), 
	//		Vector3d(1,  phi, 0), Vector3d(1, -phi, 0), Vector3d(-1,  phi, 0), Vector3d(-1, -phi, 0) };
	//	/*Vector3d positions[] = { Vector3d(0.1, -1, phi), Vector3d(0.1, 1, phi), Vector3d(0.1, -1, -phi), Vector3d(0.1, 1, -phi),
	//	Vector3d(phi, 0.1,  1), Vector3d(phi, 0.1, -1), Vector3d(-phi, 0.1,  1), Vector3d(-phi, 0.1, -1), 
	//	Vector3d(1,  phi, 0.1), Vector3d(1, -phi, 0.1), Vector3d(-1,  phi, 0.1), Vector3d(-1, -phi, 0.1) };*/
	//	for(size_t i = 0; i < 12; ++i) 
	//	{
	//		Vector3d pos = positions[i].normalize();
	//		pos =  pos * radius;
	//		vertexVec.push_back(new emxVertex(pos.x, pos.y, pos.z));
	//	}

	//	size_t faces[][3] = { 0, 4, 1, 0, 9, 4, 9, 5, 4, 4, 5, 8, 4, 8, 1, 8, 10, 1, 8, 3, 10, 5, 3, 8, 5, 2, 3, 
	//		2, 7, 3, 7, 10, 3, 7, 6, 10, 7, 11, 6, 11, 0, 6, 0, 1, 6, 6, 1, 10, 9, 0, 11, 
	//		9, 11, 2, 9, 2, 5,7, 2, 11 };
	//	face f;
	//	for(size_t i = 0; i < 20; ++i) 
	//	{
	//		f.l1 = LineIndex(vertexVec[faces[i][0]], vertexVec[faces[i][1]]);
	//		f.l2 = LineIndex(vertexVec[faces[i][1]], vertexVec[faces[i][2]]);
	//		f.l3 = LineIndex(vertexVec[faces[i][2]], vertexVec[faces[i][0]]);
	//		f.p1 = vertexVec[faces[i][0]]; 
	//		f.p2 = vertexVec[faces[i][1]]; 
	//		f.p3 = vertexVec[faces[i][2]];
	//		Faces.push_back(f);
	//	}
	//}

}

void SphereBeam::addFace(emxFace* pFace)
{
	m_Face.push_back(pFace);
}

void SphereBeam::addVertex(emxVertex* pVertex)
{
	m_Vertex.push_back(pVertex);
}

SphereBeam* SphereBeam::creat(int desity, double radius)  //desity是采样密度，即对球的内接正20面体细分次数
{
	SphereBeam* pSphere = new SphereBeam;
	vector<emxVertex*> vertexVec;
	pSphere->SphereInit(desity, radius,vertexVec);
	emxVertex* pNewVertex;
	int i = 0;
	while(i < desity)
	{
		vector<face> tempFaces;
		int base = static_cast<int>(pSphere->Lines.size());

		pSphere->Lines.resize(base * 2);
		for (unsigned int j = 0; j <pSphere->Faces.size(); j++)
		{
			//单次迭代
			face tempFace=pSphere->Faces[j];
			//生成新三点及新六线
			//前面三条线是原线截得的，后面三条是完全新生的
			line newLine1 , newLine2 , newLine3 , newLine4 , newLine5 , newLine6;
			if (pSphere->Lines[tempFace.l1].middle==NULL)
			{
				pNewVertex = pSphere->SphereNewVertex( pSphere->Lines[tempFace.l1], radius);
				pSphere->Lines[tempFace.l1].middle = pNewVertex;
				vertexVec.push_back(pNewVertex);
				newLine1.p1 = pSphere->Lines[tempFace.l1].middle;
				newLine1.p2 = pSphere->Lines[tempFace.l1].p2;
				newLine1.middle = NULL;
				pSphere->Lines[base+tempFace.l1] = newLine1;
			}
			if (pSphere->Lines[tempFace.l2].middle==NULL)
			{
				pNewVertex = pSphere->SphereNewVertex( pSphere->Lines[tempFace.l2], radius);
				pSphere->Lines[tempFace.l2].middle = pNewVertex;
				vertexVec.push_back(pNewVertex);
				newLine2.p1 = pSphere->Lines[tempFace.l2].middle;
				newLine2.p2 = pSphere->Lines[tempFace.l2].p2;
				newLine2.middle = NULL;
				pSphere->Lines[base+tempFace.l2] = newLine2;
			}
			if (pSphere->Lines[tempFace.l3].middle==NULL)
			{
				pNewVertex = pSphere->SphereNewVertex( pSphere->Lines[tempFace.l3], radius);
				pSphere->Lines[tempFace.l3].middle = pNewVertex;
				vertexVec.push_back(pNewVertex);
				newLine3.p1 = pSphere->Lines[tempFace.l3].middle;
				newLine3.p2 = pSphere->Lines[tempFace.l3].p2;
				newLine3.middle = NULL;
				pSphere->Lines[base+tempFace.l3] = newLine3;
			}
			newLine4.p1 = pSphere->Lines[tempFace.l1].middle;	newLine4.p2 = pSphere->Lines[tempFace.l2].middle;	newLine4.middle = NULL;	pSphere->Lines.push_back(newLine4);
			newLine5.p1 = pSphere->Lines[tempFace.l2].middle;	newLine5.p2 = pSphere->Lines[tempFace.l3].middle;	newLine5.middle = NULL;	pSphere->Lines.push_back(newLine5);
			newLine6.p1 = pSphere->Lines[tempFace.l3].middle;	newLine6.p2 = pSphere->Lines[tempFace.l1].middle;	newLine6.middle = NULL;	pSphere->Lines.push_back(newLine6);
			int index = static_cast<int>(pSphere->Lines.size());
			//生成新四面
			face newFace1 , newFace2 , newFace3 , newFace4;
			newFace1.p1 = tempFace.p1;	newFace1.p2 = pSphere->Lines[tempFace.l1].middle;	newFace1.p3 = pSphere->Lines[tempFace.l3].middle;
			newFace1.l1 = (pSphere->Lines[tempFace.l1].p1==newFace1.p1 ? tempFace.l1 : base+tempFace.l1);
			newFace1.l3 = (pSphere->Lines[tempFace.l3].p1==newFace1.p1 ? tempFace.l3 : base+tempFace.l3);
			newFace1.l2 = index-1;	tempFaces.push_back(newFace1);
			newFace2.p1 = tempFace.p2;	newFace2.p2 = pSphere->Lines[tempFace.l2].middle;	newFace2.p3 = pSphere->Lines[tempFace.l1].middle;
			newFace2.l1 = (pSphere->Lines[tempFace.l2].p1==newFace2.p1 ? tempFace.l2 : base+tempFace.l2);
			newFace2.l3 = (pSphere->Lines[tempFace.l1].p1==newFace2.p1 ? tempFace.l1 : base+tempFace.l1);
			newFace2.l2 = index-3;	tempFaces.push_back(newFace2);			
			newFace3.p1 = tempFace.p3;	newFace3.p2 = pSphere->Lines[tempFace.l3].middle;	newFace3.p3 = pSphere->Lines[tempFace.l2].middle;
			newFace3.l1 = (pSphere->Lines[tempFace.l3].p1==newFace3.p1 ? tempFace.l3 : base+tempFace.l3);
			newFace3.l3 = (pSphere->Lines[tempFace.l2].p1==newFace3.p1 ? tempFace.l2 : base+tempFace.l2);
			newFace3.l2 = index-2;	tempFaces.push_back(newFace3);			
			newFace4.p1 = pSphere->Lines[tempFace.l1].middle;	newFace4.p2 = pSphere->Lines[tempFace.l2].middle;	newFace4.p3 = pSphere->Lines[tempFace.l3].middle;
			newFace4.l1 = index-3;	newFace4.l2 = index-2;	newFace4.l3 = index-1;	tempFaces.push_back(newFace4);
		}
		pSphere->Faces=tempFaces;
		for (unsigned int k = 0; k < pSphere->Lines.size(); k++){
			if(pSphere->Lines[k].middle==NULL)
				break;
			pSphere->Lines[k].p2 = pSphere->Lines[k].middle;
			pSphere->Lines[k].middle = NULL;
		}
		i++;
	}
	// move face
	for (unsigned int i = 0; i < pSphere->Faces.size(); i++)
		pSphere->addFace(new emxFace(pSphere->Faces[i].p1, pSphere->Faces[i].p2, pSphere->Faces[i].p3));
	for (vector<emxVertex*>::iterator it = vertexVec.begin(); it != vertexVec.end(); ++it)
	{
		pSphere->addVertex(*it);
	}
	return pSphere;
}