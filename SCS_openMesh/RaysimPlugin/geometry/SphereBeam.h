//源beam构建模型，用正二十面体内分球面，球心和球面上三角面连接构成初始beam
#ifndef SPHEREBEAM_H
#define SPHEREBEAM_H
#include <vector>
#include <iostream>
using namespace std;

class emxVertex;
class emxFace;
class SphereBeam
{  
public:
    ~SphereBeam();
	struct line 
	{
		emxVertex *p1 , *p2;
		emxVertex *middle;
		line(){ p1=NULL ; p2=NULL ; middle=NULL; }
	};
	struct face 
	{
		int l1 , l2 , l3;  //面中3条边的索引
		emxVertex *p1 , *p2 , *p3;
		face(){ l1=-1 ; l2=-1 ; l3=-1 ; p1=NULL ; p2=NULL ; p3=NULL; }
	};

	vector<line> Lines;
	vector<face> Faces;
	vector<emxVertex*> m_Vertex; //多次细分后球上的点坐标
	vector<emxFace*> m_Face;  //多次细分后球上的三角面
	emxVertex* SphereNewVertex(line tempLine, double radius);
	int LineIndex(emxVertex* p1, emxVertex* p2);
	void SphereInit(int desity, double radius, vector<emxVertex*>& vertexVec);
	void addFace(emxFace* pFace);
	void addVertex(emxVertex* pVertex);
	SphereBeam* creat(int desity,double radius);
};
#endif