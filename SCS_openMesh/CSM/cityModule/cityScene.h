#pragma  once
#include "../util/vector.h"
#include "../util/emxUtilityInc.h"
#include <vector>
#include <string>
#include "cityGroundVector.h"
#include "cityBuilding.h"
using namespace std;

/*
��������
���������������顢����ģ��
*/
class cityScene
{
	public:
		/*
		@brief �ӱ��ض��ļ�
		*/
		cityScene(vector<string> _v, vector<string> _h, string _p);

		/*
		@brief ���������ɾֲ�����
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
	
		//����ģ������
		cityGroundVector * ground;

		vector<Vedge> AP_Edge_list;
		//�ҳ��ֲ����������е���ߣ��Ա��ڿ�������ʱ���裬ʵ���Ͻ����ǽ��������
		void GenerateEdge();
};