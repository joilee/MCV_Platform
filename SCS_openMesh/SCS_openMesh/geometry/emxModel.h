//��ȡobj�ļ���ȡ����ļ�����Ϣ 
/**
 * Mesh class.
 * Contains all vertices and faces of a mesh.
 * Parses input file.
 *
 **/

#ifndef __EMXMODEL_H__
#define __EMXMODEL_H__
#include "util\emxUtilityInc.h"
#include "emxVertex.h"
#include "emxFace.h"
#include <mesh/meshStruct.h>
#include "dataStruct.h"
#include <cityModule/cityBuilding.h>
class emxVertex;
class emxFace;

struct building
{
	vector<Vector3d> upper_facePoint;  //�������϶��������
	double height;  //������߶ȣ����϶����z�����ȥ�߶ȼ��ɻ���µ����Ӧ��ĸ߶�z����
};
class emxModel
{
public:
  emxModel (const char* filename); // parse mesh
  emxModel (vector<building>&Local_buildings, MESH_PTR pMesh);//��ȡ�˽�������Ѿ��ʷֺõĵ���������Ƭ�������ڹ��캯�����ʷ��˽����ﶥ��
  emxModel(MESH_PTR pMesh, double SimPlane_height);
  ~emxModel ();					 // default desconstruction

  // counts
  size_t NumV () const { return V.size(); } // number of vertices
  size_t NumF () const { return F.size(); } // number of faces
  size_t NumMaterialF()const {return f_materialId.size();}
  // vertex access
  bool CheckValid(size_t i)       { return (i < NumV()); }
  Vector3d GetVertex (size_t i)       { return V[i]; }          //normal member function
  const Vector3d GetVertex (size_t i) const { return V[i]; }         //const member function

  // face access
  Vector3i GetFace   (size_t i)       { return F[i]; }
  const Vector3i GetFace   (size_t i) const { return F[i]; }
  // get face by index
  emxFace* GetFacep(size_t index) { assert(index < faceVec.size()); return faceVec[index]; }

  Vector3d GetCenter(size_t i);

  // normal access
  Vector3d GetNormal(size_t i)        { return NF[i]; }
  const Vector3d GetNormal(size_t i)  const { return NF[i]; }
  int   getMtlId(int faceId);  //��ȡ���Ϊi����Ƭ����Ӧ�Ĳ���

  // bounding box
  void GetBBox (Vector3d& min, Vector3d& max) const { min = minPos; max = maxPos; }


  vector<Vedge> AP_Edge_list;
  //�ҳ��ֲ����������е���ߣ��Ա��ڿ�������ʱ���裬ʵ���Ͻ����ǽ��������
  void GenerateEdge(vector< building > &Local_buildings);

  Vector3d minPos, maxPos;	 //���е��е������С��

private:
  std::vector<Vector3d> V;					// vertices
  std::vector<Vector3i> F;					// faces
  std::vector<Vector3d> NF;					// normals of faces
  /*std::vector<Vector3d> Vn;                 // normals of vertices*/
  std::vector<int>f_materialId;               //the material of face,��Ƭ��Ӧ�Ĳ��ϱ��
    
  std::vector<emxVertex*>   vertexVec;		// vertices
  std::vector<emxFace*>     faceVec;		// triangle face
};
#endif 
