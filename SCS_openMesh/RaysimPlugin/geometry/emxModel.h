//读取obj文件获取所需的几何信息 
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
#include "height/TriMesh.h"
#include <mesh/meshStruct.h>
#include <cityModule/cityBuilding.h>
#include <para/modelPara.h>
class emxVertex;
class emxFace;

struct building
{
	vector<Vector3d> upper_facePoint;  //建筑物上顶面点坐标
	double height;  //建筑物高度，用上顶面点z坐标减去高度即可获得下底面对应点的高度z坐标
};
class emxModel
{
public:
	/*
	*@brief parse mesh
	*/
  emxModel (const char* filename); 

  emxModel (vector<building>&Local_buildings, MESH_PTR pMesh);
  emxModel(MESH_PTR pMesh, double SimPlane_height);

  /*
  *@brief 从城市场景模型中构建模型
  *@since 6/22/2017
  */
  emxModel(BaseModel *para);

  ~emxModel ();					 // default desconstruction

  // counts
  size_t NumV () const { return V.size(); } // number of vertices
  size_t NumF () const { return F.size(); } // number of faces

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
  int   getMtlId(int faceId);  //获取编号为i的面片所对应的材料

  // bounding box
  void GetBBox (Vector3d& min, Vector3d& max) const { min = minPos; max = maxPos; }

  Vector3d minPos, maxPos;	 //所有点中的最大最小点

  vector<Vedge> AP_Edge_list;
  //找出局部场景中所有的棱边，以便于考虑绕射时所需，实际上仅考虑建筑物棱边
  void GenerateEdge(vector< building > &Local_buildings);

private:
  std::vector<Vector3d> V;					// vertices
  std::vector<Vector3i> F;					// faces
  std::vector<Vector3d> NF;					// normals of faces
  /*std::vector<Vector3d> Vn;                 // normals of vertices*/
  std::vector<int>f_materialId;               //the material of face,面片对应的材料编号
    
  std::vector<emxVertex*>   vertexVec;		// vertices
  std::vector<emxFace*>     faceVec;		// triangle face
};
#endif 
