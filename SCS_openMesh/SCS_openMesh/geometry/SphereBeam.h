//Դbeam����ģ�ͣ�������ʮ�����ڷ����棬���ĺ����������������ӹ��ɳ�ʼbeam
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
		int l1 , l2 , l3;  //����3���ߵ�����
		emxVertex *p1 , *p2 , *p3;
		face(){ l1=-1 ; l2=-1 ; l3=-1 ; p1=NULL ; p2=NULL ; p3=NULL; }
	};

	vector<line> Lines;
	vector<face> Faces;
	vector<emxVertex*> m_Vertex; //���ϸ�ֺ����ϵĵ�����
	vector<emxFace*> m_Face;  //���ϸ�ֺ����ϵ�������
	emxVertex* SphereNewVertex(line tempLine, double radius);
	int LineIndex(emxVertex* p1, emxVertex* p2);
	void SphereInit(int desity, double radius, vector<emxVertex*>& vertexVec);
	void addFace(emxFace* pFace);
	void addVertex(emxVertex* pVertex);
	SphereBeam* creat(int desity,double radius);
};
#endif