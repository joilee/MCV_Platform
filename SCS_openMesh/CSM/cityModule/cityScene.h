#pragma  once
#include "../util/vector.h"
#include "../util/emxUtilityInc.h"
#include <vector>
#include <string>
#include "cityGroundVector.h"
#include "cityBuilding.h"
using namespace std;

/*
建筑物类
包含：建筑物数组、地面模型
*/
class cityScene
{
	public:
		/*
		@brief 从本地读文件
		*/
		cityScene(vector<string> _v, vector<string> _h, string _p);

		/*
		@brief 从整体生成局部场景
		*/
		cityScene(Vector3d center, double range, cityScene* cityAll);

		~cityScene();

		cityGroundVector *getGround();
		double getAltitude(double x, double y);
		vector<Building>& getTotal_Building(){ return total_Buildings; }
		inline size_t getBuildingSize(){ return total_Buildings.size(); }
		inline int getConcaveNum(){ return concave_num; }
		const Building& getBuildingByReference(int id){ return total_Buildings[id]; }
		Building getBuildingByValue(int id){ return total_Buildings[id]; }
		inline Vector3d getMaxPoint(){ return MaxPoint; }
		inline Vector3d getMinPoint(){ return MinPoint; }
		inline vector<Vedge>& getAPEdge(){ return AP_Edge_list; }
	private:
		void readBuilding(const char*filename_2D, const char*filename_Height);
		void readGround(string p);

	private:
		vector<Building> total_Buildings;
		int concave_num;
		Vector3d MaxPoint, MinPoint;
	
		//地面模型数组
		cityGroundVector * ground;

		vector<Vedge> AP_Edge_list;
		//找出局部场景中所有的棱边，以便于考虑绕射时所需，实际上仅考虑建筑物棱边
		void GenerateEdge();
};