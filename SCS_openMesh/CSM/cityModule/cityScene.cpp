#include "cityScene.h"
//#include "../../util/emxUtilityInc.h"
#include <limits>
#include <fstream>


void cityScene::readGround(string p)
{
	//导入海拔信息
	ground = new cityGroundVector(p);
	MaxPoint = Max(MaxPoint, Vector3d(ground->getXmax(), ground->getYmax(), numeric_limits<double>::min()));
	MinPoint = Min(MinPoint, Vector3d(ground->getXmin(), ground->getYmin(), numeric_limits<double>::max()));
}

cityScene::cityScene(vector<string> _v, vector<string> _h, string _p)
{
	concave_num = 0;
	MinPoint = Vector3d(numeric_limits<double>::max(), numeric_limits<double>::max(), numeric_limits<double>::max());
	MaxPoint = Vector3d(numeric_limits<double>::min(), numeric_limits<double>::min(), numeric_limits<double>::min());
	readGround(_p);
	cout << "Info: 地面模型读入成功。" << endl;
	string Scene2DInfoFile_path, SceneHeightInfoFile_path;

	for (size_t i = 0; i < _v.size();i++)
	{
		Scene2DInfoFile_path =_v.at(i);
		SceneHeightInfoFile_path = _h.at(i);
		readBuilding(Scene2DInfoFile_path.c_str(), SceneHeightInfoFile_path.c_str());
	}
}
cityGroundVector *cityScene::getGround()
{
	return ground;
}

void cityScene::readBuilding(const char*filename_2D, const char*filename_Height)
{
	//场景vector格式文件读取
	ifstream infile1(filename_2D, ios::in | ios::_Nocreate);
	if (!infile1)
	{
		cout << "can not open file1!" << endl;
		return;
	}
	ifstream infile2(filename_Height, ios::in | ios::_Nocreate);
	if (!infile2)
	{
		cout << "can not open file2!" << endl;
		return;
	}
	string str1, str2;
	infile1.seekg(4, ios::beg);
	infile2.seekg(3, ios::beg);
	while (getline(infile1, str1))
	{
		int building_id1 = 0;
		string sign1 = " ";
		int point_num = 0;
		istringstream linestream1(str1);
		linestream1 >> building_id1 >> sign1 >> point_num;

		int building_id2 = 0;
		string sign2 = " ";
		double height = 0;
		getline(infile2, str2);
		istringstream linestream2(str2);
		linestream2 >> building_id2 >> sign2 >> height;

		if (point_num > 0)
		{
			Building building_info;
			building_info.height = height;

			for (int i = 0; i < point_num; i++)
			{
				double x, y;
				getline(infile1, str1);
				istringstream linestream4(str1);
				linestream4 >> x >> y;

				double building_altitude = getAltitude(x, y); //建筑物各个点按照实际获取对应海拔值
				double upper_height = height + building_altitude;
				double under_height = building_altitude;
				building_info.upper_facePoint.push_back(Vector3d(x, y, upper_height));
				MaxPoint = Max(MaxPoint, Vector3d(x, y, upper_height) );
				MinPoint = Min(MinPoint, Vector3d(x, y, under_height) );
			}
			total_Buildings.push_back(building_info);
		}
	}
	infile1.close();
	infile2.close();
}

void cityScene::GenerateEdge()
{
	AP_Edge_list.clear();
	Vedge edge_now1, edge_now2;
	for (size_t buildings_id = 0; buildings_id < total_Buildings.size(); buildings_id++)
	{
		int count = total_Buildings[buildings_id].upper_facePoint.size() - 1;
		double building_height = total_Buildings[buildings_id].height;
		for (int id = 0; id < count; id++) //记录building顶面点坐标时，首末点重合，记录两次，所以   .size（）-1
		{
			Vector3d point1 = total_Buildings[buildings_id].upper_facePoint[id];
			Vector3d point2 = total_Buildings[buildings_id].upper_facePoint[(id + 1) % count];
			Vector3d point3 = total_Buildings[buildings_id].upper_facePoint[(id - 1 + count) % count];
			edge_now1.start = point1;   //上顶面以buildings[buildings_id][id]为起点的棱边
			edge_now1.end = point2;
			edge_now2.start = point1;  //以buildings[buildings_id][id]为起点的侧棱边
			double under_height = point1.z - building_height;
			edge_now2.end = Vector3d(point1.x, point1.y, under_height);


			Vector3d a = VectorCross((edge_now1.end - edge_now1.start), (edge_now2.end - edge_now2.start));
			Vector3d b = VectorCross((edge_now1.end - edge_now1.start), (point1 - point3));
			Vector3d c = VectorCross((point1 - point3), (edge_now2.end - edge_now2.start));

			if (a != 0 && b != 0 && c != 0)
			{
				edge_now1.normal_front = a.normalize();
				edge_now1.normal_back = b.normalize();
				edge_now2.normal_front = c.normalize();
				edge_now2.normal_back = edge_now1.normal_front;
				edge_now1.materialId = 11;
				edge_now2.materialId = 11;

				Vector3d Edge_unitDirection1 = (edge_now1.end - edge_now1.start).normalize();
				double FaceInteriorAngle1 = 0;
				if (Dot(VectorCross(edge_now1.normal_front, edge_now1.normal_back), Edge_unitDirection1) > 0)
				{
					FaceInteriorAngle1 = M_PI - acos(max(-1.0, min(1.0, Dot(edge_now1.normal_front, edge_now1.normal_back))));
					if (FaceInteriorAngle1 >= M_PI / 6 && FaceInteriorAngle1 <= M_PI * 2 / 3)   //劈内角满足30度~120度之间才可以发生绕射
					{
						AP_Edge_list.push_back(edge_now1);
					}
				}

				//判断棱边edge_now2劈内角是否符合 30~120范围
				Vector3d Edge_unitDirection2 = (edge_now2.end - edge_now2.start).normalize();
				double FaceInteriorAngle2 = 0;
				if (Dot(VectorCross(edge_now2.normal_front, edge_now2.normal_back), Edge_unitDirection2) > 0)
				{
					FaceInteriorAngle2 = M_PI - acos(max(-1.0, min(1.0, Dot(edge_now2.normal_front, edge_now2.normal_back))));
					if (FaceInteriorAngle2 >= M_PI / 6 && FaceInteriorAngle2 <= M_PI * 2 / 3)   //劈内角满足30度~120度之间才可以发生绕射
					{
						AP_Edge_list.push_back(edge_now2);

						Vertical_Edge_ID.push_back(AP_Edge_list.size() - 1);    //存储垂直棱边在Edge_list 中对应的索引编号
					}
				}
				
			}
			else if (a!=0&&c!=0)//特殊处理
			{
				edge_now1.normal_front = a.normalize();
				edge_now1.normal_back = Vector3d(0, 0,1);
				edge_now1.materialId = 11;
				AP_Edge_list.push_back(edge_now1);
			}
			else
			{
				cout << "something wrong!" << endl;
			}
		}
	}
}

double cityScene:: getAltitude(double x, double y)
{
	if (ground->existed())
	{
		return ground->getAltitudeFrom0(x, y);
	}
	else
	{
		cout << "error: ground is not existed!" << endl;
		return 0;
	 }
}

cityScene::cityScene(Vector3d  AP_position, double  LocalRange, cityScene* cityAll)
{
	MinPoint = Vector3d(DBL_MAX, DBL_MAX, DBL_MAX);
	MaxPoint = Vector3d(DBL_MIN, DBL_MIN, DBL_MIN);
	MinPoint = AP_position - Vector3d(LocalRange / 2, LocalRange / 2, 0);
	MaxPoint = AP_position + Vector3d(LocalRange / 2, LocalRange / 2, 0);

	//step1 get all building in the bouding box
	int buildingSize = cityAll->getBuildingSize();
	for (size_t buildings_id = 0; buildings_id < buildingSize; buildings_id++)
	{
		bool in_range = true;
		Building tmpBuilding = cityAll->getBuildingByReference(buildings_id);
		for (int id = 0; id < tmpBuilding.upper_facePoint.size() - 1; id++) //记录building顶面点坐标时，首末点重合，记录两次，所以   .size（）-1
		{
			double x1 = tmpBuilding.upper_facePoint[id].x - MinPoint.x;
			double x2 = tmpBuilding.upper_facePoint[id].x - MaxPoint.x;
			double y1 = tmpBuilding.upper_facePoint[id].y - MinPoint.y;
			double y2 = tmpBuilding.upper_facePoint[id].y - MaxPoint.y;
			if (!(x1*x2 < 0 && y1*y2 < 0)) //判断建筑物是否在设定范围内,即使有一个点不在范围内，也判定为不在内部
			{
				in_range = false;
				break;
			}
		}
		if (in_range)
		{
			total_Buildings.push_back(cityAll->getBuildingByValue(buildings_id));
			cout << buildings_id << " "<<tmpBuilding.height<<endl;
		}
	}

	//step2 生成局部的cityGround模型数据
	//从第一个地面中实现
	
	cityGround * ground_1 = new cityGround(* ((cityAll->getGround()->getGroundVector())[0]) , AP_position,LocalRange);

	ground = new cityGroundVector;
	ground->addItem(ground_1);
	MinPoint = Min(MinPoint, Vector3d(ground->getXmin(), ground->getYmin(), DBL_MAX));
	MaxPoint = Max(MaxPoint, Vector3d(ground->getXmax(), ground->getYmax(), DBL_MIN));
	//step3 生成建筑物棱边
	GenerateEdge();
}

cityScene::~cityScene()
{
	if (ground)
	{
		delete ground;
	}
	total_Buildings.clear();
}