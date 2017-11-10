//读取几何文件obj，提取信息存放在vector容器（记录顶点容器V、记录面容器F）中

#include "geometry/emxModel.h"
#include "util/emxUtilityInc.h"
using namespace std;

int IsWhiteSpace (char c)
{
	if ( c==' ' || c=='\t' || c=='\n' || c=='\r' || c=='\v' || c=='\f')
		return 1;
	else
		return 0;
}

//找出局部场景中所有的棱边，以便于考虑绕射时所需，实际上仅考虑建筑物棱边
void emxModel:: GenerateEdge(vector< building > &Local_buildings)
{
	Vedge edge_now1,edge_now2;
	for (int buildings_id = 0; buildings_id < Local_buildings.size(); buildings_id++)
	{
		int count = Local_buildings[buildings_id].upper_facePoint.size()-1;
		double building_height = Local_buildings[buildings_id].height;
		for (int id = 0; id < count; id++) //记录building顶面点坐标时，首末点重合，记录两次，所以   .size（）-1
		{
			Vector3d point1 = Local_buildings[buildings_id].upper_facePoint[id];
			Vector3d point2 = Local_buildings[buildings_id].upper_facePoint[(id + 1)%count];
			Vector3d point3 = Local_buildings[buildings_id].upper_facePoint[(id - 1 + count)%count];
			edge_now1.start = point1;   //上顶面以buildings[buildings_id][id]为起点的棱边
			edge_now1.end = point2;
			edge_now2.start = point1;  //以buildings[buildings_id][id]为起点的侧棱边
			double under_height = point1.z - building_height;
			edge_now2.end = Vector3d(point1.x,point1.y,under_height); 


			Vector3d a=VectorCross((edge_now1.end-edge_now1.start),(edge_now2.end - edge_now2.start));
			Vector3d b=VectorCross((edge_now1.end - edge_now1.start),(point1 - point3));
			Vector3d c=VectorCross((point1 - point3),(edge_now2.end-edge_now2.start));

			if (a!=0&&b!=0&&c!=0)
			{
				edge_now1.normal_front = a.normalize();  
				edge_now1.normal_back = b.normalize();
				edge_now2.normal_front =c.normalize(); 
				edge_now2.normal_back = edge_now1.normal_front;  
				edge_now1.materialId = 11;
				edge_now2.materialId = 11;

				AP_Edge_list.push_back(edge_now1);
				AP_Edge_list.push_back(edge_now2);	
			}else
			{
				cout<<"something wrong!"<<endl;
			}	
		}		
	}

}

emxModel::emxModel (const char* filename)
{
	V.clear();
	F.clear();
	f_materialId.clear();
	NF.clear();
	// delete all faces
	for(vector<emxFace*>::iterator pos = faceVec.begin(); pos != faceVec.end(); ++pos)
		delete *pos;
	faceVec.clear();
	// clear all vertices
	for(vector<emxVertex*>::iterator pos = vertexVec.begin(); pos != vertexVec.end(); ++pos)
		delete *pos;
	vertexVec.clear();

	FILE *file = fopen(filename, "r");
	if(file == NULL)
	{
		cout<<"Can't open file "<<filename<<endl;
		return;
	}
  
	char buffer[1024];
	int everything_ok = 1; 
    int Id = -1;
	while(everything_ok && fgets(buffer, 1024, file)!=NULL)
	{
		if (strncmp(buffer,"v ",2) == 0) 
		{ 
			double px, py, pz;
			if (sscanf(buffer, "v %lf %lf %lf", &px, &py, &pz) == 3) 
			{
				V.push_back(Vector3d(px, py, pz));
				vertexVec.push_back(new emxVertex(px, py, pz));
			}
			else
			{
				everything_ok = 0;
				cout<<"parse error, vertex "<<NumV()<<endl;
			}
		}
		else if (strncmp(buffer,"# material",10) == 0)
		{
			if(sscanf(buffer, "# material %d", &Id) == 1)
			{
				//材料默认是只有一种材料，整个obj文件只包含一个ID，该id指向的材料一般是混凝土
			}
			else
			{
				everything_ok = 0;
				cout<<"parse error, material "<<endl;
			}
		}
		else if (strncmp(buffer,"f ",2) == 0)	
		{
			int x,y,z;
			std::vector<int> vertexes;  //存放每一行读取面的顶点索引
			if (sscanf(buffer, "f %d %d %d", &x, &y, &z) == 3) 
			{
				vertexes.push_back(x);
				vertexes.push_back(y);
				vertexes.push_back(z);
			}
			else
			{
				everything_ok = 0;
				cout<<"parse error, vertex "<<NumV()<<endl;
			}
			//add face and set connectivity
			bool valid = true;
			for(size_t i = 0; i < vertexes.size(); ++i) 
			{
				if(CheckValid(vertexes[i]-1) != true)
					valid = false;
			}
						
			if(valid) 
			{
				for(size_t i = 2; i < vertexes.size(); ++i)
			 	{
			 		Vector3d E1 = V[vertexes[i-1] - 1] - V[vertexes[0] - 1];
			 		Vector3d E2 = V[vertexes[i]   - 1] - V[vertexes[0] - 1];
									
			 		Vector3d N  = VectorCross(E1, E2);
			 		if(N.norm() > DOUBLE_EPSILON) 
			 		{
			 			F.push_back(Vector3i(vertexes[0] - 1, vertexes[i-1] - 1, vertexes[i] - 1));	//将面片中点的索引对应到下标值，故均需减1
			 			//f_materialId.push_back(Id);   //与面同步记录的材质编号
			 			NF.push_back(N.normalize());  //与面同步记录的法向量

						emxVertex* v1 = vertexVec.at(vertexes[0]-1);
						emxVertex* v2 = vertexVec.at(vertexes[i-1]-1);
						emxVertex* v3 = vertexVec.at(vertexes[i]-1);
						faceVec.push_back(new emxFace(v1,v2,v3));
			 		}
			 		else 
			 		{
					
			 		}
			 	}
			}
		}
	}

	// calculate the bounding box
	std::vector<Vector3d>::const_iterator v = V.begin();
	minPos = (*v);
	maxPos = (*v);
	for(++v; v!=V.end(); ++v) 
	{
		minPos = Min(minPos, (*v));
		maxPos = Max(maxPos, (*v));
	}

	if(!everything_ok) 
	{
		V.clear();
		F.clear();
		f_materialId.clear();
		NF.clear();
		// delete all faces
		for(vector<emxFace*>::iterator pos = faceVec.begin(); pos != faceVec.end(); ++pos)
			delete *pos;
		faceVec.clear();

		// clear all vertices
		for(vector<emxVertex*>::iterator pos = vertexVec.begin(); pos != vertexVec.end(); ++pos)
			delete *pos;
		vertexVec.clear();
	}


	fclose(file);
}
emxModel::emxModel (vector<building> &Local_buildings, MESH_PTR pMesh)
{
	V.clear();
	F.clear();
	f_materialId.clear();
	NF.clear();
	// delete all faces
	for(vector<emxFace*>::iterator pos = faceVec.begin(); pos != faceVec.end(); ++pos)
		delete *pos;
	faceVec.clear();

	// clear all vertices
	for(vector<emxVertex*>::iterator pos = vertexVec.begin(); pos != vertexVec.end(); ++pos)
		delete *pos;
	vertexVec.clear();

	int concave_polygonNum = 0;
	for (int buildings_id = 0; buildings_id < Local_buildings.size(); buildings_id++)
	{
		//每栋建筑物上顶面点坐标按照顺时针存储的，所以剖分生成面片时，要注意面片点的顺序，确保面片法向量均朝外
		int count = Local_buildings[buildings_id].upper_facePoint.size()-1; //记录building顶面点坐标时，首末点重合，记录两次，所以   .size（）-1
		double building_height = Local_buildings[buildings_id].height;
		int V_size = V.size();
		vector<int> upper_PointIndex; //存储上顶面点的索引值，以备上顶面剖分生成面片时所用

		//点的存储操作
		//上顶面的点
		for (int id = 0; id < count; id++)
		{
			Vector3d point = Local_buildings[buildings_id].upper_facePoint[id];
			V.push_back(point);
			upper_PointIndex.push_back(V.size()-1);
			vertexVec.push_back(new emxVertex(point.x,point.y,point.z));
		}
		//下底面的点
		for (int id = 0; id < count; id++)
		{
			Vector3d point = Local_buildings[buildings_id].upper_facePoint[id];
			double under_height = point.z - building_height;
			V.push_back(Vector3d(point.x,point.y,under_height));
			vertexVec.push_back(new emxVertex(point.x,point.y,under_height));
		}

		Vector3d E1,E2,N;
		//面片的操作
		//建筑物侧面剖分生成面片
		for (int id = 0; id < count; id++) 
		{
			int i1,i2,i3,i4;

			i1 = id + V_size;
			i2 = (id+1)%count + V_size;
			i3 = (id+1)%count + count + V_size;
			i4 = id  + count + V_size;

			E1 = V[i2] - V[i1];
			E2 = V[i3] - V[i2];
			N  = VectorCross(E1, E2);

			if(N.norm() > DOUBLE_EPSILON)
			{
				F.push_back(Vector3i(i1,i2,i3));
				emxVertex* v1 = vertexVec.at(i1);
				emxVertex* v2 = vertexVec.at(i2);
				emxVertex* v3 = vertexVec.at(i3);
				faceVec.push_back(new emxFace(v1,v2,v3));
				NF.push_back(N.normalize());
			}

			E1 = V[i3] - V[i1];
			E2 = V[i4] - V[i3];
			N  = VectorCross(E1, E2);

			if(N.norm() > DOUBLE_EPSILON)
			{
				F.push_back(Vector3i(i1,i3,i4));
				emxVertex* v1 = vertexVec.at(i1);
				emxVertex* v2 = vertexVec.at(i3);
				emxVertex* v3 = vertexVec.at(i4);
				faceVec.push_back(new emxFace(v1,v2,v3));
				NF.push_back(N.normalize());
			}			
		}	

		//建筑物上顶面剖分生成面片，面片法向量均为正z轴方向即Vector3d(0,0,1),凹多边形三角化特殊处理见网址：http://blog.sina.com.cn/s/blog_5a6f39cf0101374h.html
		bool convex = true; //为true时是凸多边形，false时是凹多边形
		vector<Vector3d> upper_face = Local_buildings[buildings_id].upper_facePoint;
		upper_face.pop_back(); //首末点重复，所以要删掉末尾的重复点
		int count1 = upper_face.size();
	//	vector<int> a(count1,1);  //存放点的凹凸性，凹为0，凸为1;仅当对凹点处理生成面片时，此凹点的凹凸性可能会发生变化，其他点凹凸性保持不变
		for (int id = 0; id < count1; id++) 
		{
			Vector3d v1,v2,v3;
			v1 = upper_face[id];
			v2 = upper_face[(id+1)%count1];
			v3 = upper_face[(id+2)%count1];

			if (Dot(VectorCross(v2-v1,v3-v2),Vector3d(0,0,-1)) < DOUBLE_EPSILON)//判断是否为凹
			{
				//a[(id+1)%count1] = 0; //为凹点

				//判断若为凹点，直接剔除掉
				std::vector<Vector3d>::iterator it1 = upper_face.begin()+(id+1)%count1;
				upper_face.erase(it1);
				std::vector<int>::iterator it2 = upper_PointIndex.begin()+(id+1)%count1;
				upper_PointIndex.erase(it2);

				count1--;
				id--;

				convex = false;
			}
		}
		if (!convex)
		{
			concave_polygonNum++;
		}
	
		//凸多边形剖分，选择一个顶点，然后依次寻找下两个顶点组成一个三角形
		int count2 = upper_face.size();
		for (int id = 0; id <= count2 - 3; id++)
		{
			E1 = V[upper_PointIndex[id+2]] - V[upper_PointIndex[0]];
			E2 = V[upper_PointIndex[id+1]] - V[upper_PointIndex[id+2]];
			N  = VectorCross(E1, E2);
			if(N.norm() > DOUBLE_EPSILON)
			{
				F.push_back(Vector3i(upper_PointIndex[0],upper_PointIndex[id+2], upper_PointIndex[id+1]));
				emxVertex* v1 = vertexVec.at(upper_PointIndex[0]);
				emxVertex* v2 = vertexVec.at(upper_PointIndex[id+2]);
				emxVertex* v3 = vertexVec.at(upper_PointIndex[id+1]);
				faceVec.push_back(new emxFace(v1,v2,v3));
				NF.push_back(N.normalize());
			}
		}
	}
	cout<<"the total num of buildings is:"<<Local_buildings.size()<<endl;
	cout<<"the num of concave polygon is: "<<concave_polygonNum<<endl;

	//地面建模 added by ligen  从pMesh中添加三角形和点 记住法向量必须朝外
	int V_num = V.size();
	int F_startNum=F.size();
	int NF_startNum=NF.size();
	VERTEX2D_PTR pVtx=pMesh->pVerArr;
	int numV=pMesh->vertex_num;
	for(int i=3;i<numV+3;i++)//前三个点是bounding triangle
	{
		double x,y,z;
		x=((VERTEX2D_PTR)(pMesh->pVerArr+i))->x;
		y=((VERTEX2D_PTR)(pMesh->pVerArr+i))->y;
		z=((VERTEX2D_PTR)(pMesh->pVerArr+i))->z;
		V.push_back(Vector3d(x,y,z));
		vertexVec.push_back(new emxVertex(x,y,z));
	}

	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int tri_index = 0;
	int* pi;
	while(pTri != NULL)	
	{
		tri_index++;
		int id[3];
		pi = &(pTri->i1);
		for (int j=0; j<3; j++)	
		{	
			id[j] = *pi++;	
			id[j]=id[j]-3+V_num;
		}
		Vector3d E1,E2,N;
		E1 = V[id[1]] - V[id[0]];
		E2 = V[id[2]] - V[id[1]];
		N  = VectorCross(E1, E2);
		if(N.norm() > DOUBLE_EPSILON)
		{
			F.push_back(Vector3i(id[0],id[1],id[2]));
			emxVertex* v1 = vertexVec.at(id[0]);
			emxVertex* v2 = vertexVec.at(id[1]);
			emxVertex* v3 = vertexVec.at(id[2]);
			faceVec.push_back(new emxFace(v1,v2,v3));
			NF.push_back(N.normalize());
		}	

		pTri = pTri->pNext;
	}
	
	int F_endNum=F.size();
	int NF_endNum=NF.size();
	cout<<"地面建模点的数量 "<<V.size()-V_num<<"  三角形数量 "<<F_endNum-F_startNum<<" 法向量数量 "<<NF_endNum-NF_startNum<<endl;


	// calculate the bounding box
	std::vector<Vector3d>::const_iterator v = V.begin();
	minPos = (*v);
	maxPos = (*v);
	for(++v; v!=V.end(); ++v) 
	{
		minPos = Min(minPos, (*v));
		maxPos = Max(maxPos, (*v));
	}
}
emxModel::emxModel(MESH_PTR pMesh, double SimPlane_height)
{
	V.clear();
	F.clear();
	f_materialId.clear();
	NF.clear();
	// delete all faces
	for(vector<emxFace*>::iterator pos = faceVec.begin(); pos != faceVec.end(); ++pos)
		delete *pos;
	faceVec.clear();
	// clear all vertices
	for(vector<emxVertex*>::iterator pos = vertexVec.begin(); pos != vertexVec.end(); ++pos)
		delete *pos;
	vertexVec.clear();

	//由地面生成高度为SimPlane_height的虚拟仿真面
	VERTEX2D_PTR pVtx=pMesh->pVerArr;
	int numV=pMesh->vertex_num;
	for(int i=3;i<numV+3;i++)//前三个点是bounding triangle
	{
		double x,y,z;
		x=((VERTEX2D_PTR)(pMesh->pVerArr+i))->x;
		y=((VERTEX2D_PTR)(pMesh->pVerArr+i))->y;
		z=((VERTEX2D_PTR)(pMesh->pVerArr+i))->z + SimPlane_height;
		V.push_back(Vector3d(x,y,z));
		vertexVec.push_back(new emxVertex(x,y,z));
	}

	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int tri_index = 0;
	int* pi;
	while(pTri != NULL)	
	{
		tri_index++;
		int id[3];
		pi = &(pTri->i1);
		for (int j=0; j<3; j++)	
		{	
			id[j] = *pi++;	
			id[j]=id[j]-3;
		}
		Vector3d E1,E2,N;
		E1 = V[id[1]] - V[id[0]];
		E2 = V[id[2]] - V[id[1]];
		N  = VectorCross(E1, E2);
		if(N.norm() > DOUBLE_EPSILON)
		{
			F.push_back(Vector3i(id[0],id[1],id[2]));
			emxVertex* v1 = vertexVec.at(id[0]);
			emxVertex* v2 = vertexVec.at(id[1]);
			emxVertex* v3 = vertexVec.at(id[2]);
			faceVec.push_back(new emxFace(v1,v2,v3));
			NF.push_back(N.normalize());
		}	

		pTri = pTri->pNext;
	}
//	UnInitMesh(pMesh);
	cout<<"虚拟仿真面建模点的数量 "<<V.size()<<"  三角形数量 "<<F.size()<<"  法向量数量 "<<NF.size()<<endl;

	// calculate the bounding box
	std::vector<Vector3d>::const_iterator v = V.begin();
	minPos = (*v);
	maxPos = (*v);
	for(++v; v!=V.end(); ++v) 
	{
		minPos = Min(minPos, (*v));
		maxPos = Max(maxPos, (*v));
	}
}
emxModel::~emxModel()
{
	V.clear();
	F.clear();
	f_materialId.clear();
	NF.clear();
	// delete all faces
	for(vector<emxFace*>::iterator pos = faceVec.begin(); pos != faceVec.end(); ++pos)
		delete *pos;
	faceVec.clear();

	// clear all vertices
	for(vector<emxVertex*>::iterator pos = vertexVec.begin(); pos != vertexVec.end(); ++pos)
		delete *pos;
	vertexVec.clear();
}


//get the center point position of the face
Vector3d emxModel::GetCenter(size_t i)
{
	Vector3i vertexID = F[i];
	Vector3d A, B, C;
	A = V[vertexID.x];
	B = V[vertexID.y];
	C = V[vertexID.z];
	return (A + B + C) / 3.0; 
}


int emxModel::getMtlId(int faceId)
{
	if (faceId>NumMaterialF()-1)
	{
		return -1;
	}else if(NumMaterialF()==0)
	{
		return -1;
	}
	return f_materialId[faceId];   //记录编号为faceId的面片对应材质编号
}