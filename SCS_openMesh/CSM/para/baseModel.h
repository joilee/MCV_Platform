#pragma once
#include "../cityModule/cityScene.h"
#include "../mesh/meshStruct.h"
#include "../util/vector.h"
#include "../mesh/point.h"
#include "../material/scsMaterial.h"
#include "../cityModule/ModelType.h"
#include <memory>
//����ģ��
//�����ֲ����г�����������Ƭ
class BaseModel
{
public:
	BaseModel();
	~BaseModel();
	 
	void setCityScene(cityScene*_a){ localScene = _a; }
	cityScene* getLocalScene(){ return localScene; }

	void setGround_Mesh(MESH_PTR a){ ground_pMesh = a; };
	MESH_PTR getGround_Mesh(){ return ground_pMesh; }

	void setVertices(vector<Vector3d> a){ V = a; }
	vector<Vector3d>& getVertices(){ return V; }

	void setFaces(vector<Vector3i> a){ F = a; }
	vector<Vector3i> & getFaces(){ return F; }

	void setNormals(vector<Vector3d> N){ NF = N; }
	vector<Vector3d> &getNormals(){ return NF; }

	void setEachFaceMaterial(vector<int> a){ f_materialId = a; }
	vector<int>& getF_Material(){ return f_materialId; }

	void setApEdgeList(vector<Vedge> a){ AP_Edge_list = a; }
	vector<Vedge> &getAP_Edge_List(){ return AP_Edge_list; }

	void setApEdgeID(vector<int> id){ AP_Edge_ID=id; }
	vector<int>& getAP_Edge_ID(){ return AP_Edge_ID; }

	void setSceneRange(double a){ LocalScene_range = a; }
	double getSceneRange(){ return LocalScene_range; }

	void setModelType(ModelType a){ m_Type = a; }
	ModelType getModelType(){ return m_Type; };

	void setModelID(int a){ id = a; };
	int getModelID(){ return id; }

	/************************************************************************/
	/* �õ�һ�����Ӧ�ĸ߶�                                                                     */
	/************************************************************************/
	double getPointAltitude(double x, double y);
private:
	cityScene *localScene;
	double LocalScene_range;
	//������������
	MESH_PTR ground_pMesh;

	//���еĵ㡢�桢���ϱ��
	std::vector<Vector3d> V;					// vertices
	std::vector<Vector3i> F;					// faces
	std::vector<Vector3d> NF;					 // normals of faces
	std::vector<int>f_materialId;                //the material of face,��Ƭ��Ӧ�Ĳ��ϱ��
	//���������
	vector<Vedge> AP_Edge_list;
	vector<int> AP_Edge_ID;
	int id;//��վ���siteIDһһ��Ӧ
	ModelType m_Type;

};



