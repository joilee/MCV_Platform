#include "algo.h"
#include <QMessageBox>
#include <para/baseModel.h>

int algo:: sign_func(double x)  //符号函数
{
	if (x > DOUBLE_EPSILON)
	{
		return 1;
	}
	else if (x < -DOUBLE_EPSILON)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
int algo::roundInt(double x) //四舍五入取整函数
{
	if (x >= 0.0)
	{
		return int(x+0.5);
	}
	else
	{
		return int(x-0.5);
	}
}


complex<double> algo::Fresnel(double xf)
{
	complex<double> fxx[ 8 ] = { complex<double>(0.0, 0.0), complex<double>(0.5195, 0.0025), 
		complex<double>(0.3355, -0.0665),  complex<double>(0.2187, -0.0757), 
		complex<double>(0.127, -0.068),    complex<double>(0.0638, -0.0506), 
		complex<double>(0.0246, -0.0296),  complex<double>(0.0093, -0.0163) };
	complex<double> fx[ 8 ]  = { complex<double>(0.5729, 0.2677),   complex<double>(0.6768, 0.2682), 
		complex<double>(0.7439, 0.2549),   complex<double>(0.8095, 0.2322), 
		complex<double>(0.873, 0.1982),    complex<double>(0.9240, 0.1577), 
		complex<double>(0.9658, 0.1073),   complex<double>(0.9797, 0.0828) };
	double    xx[ 8 ] = { 0.3, 0.5, 0.7, 1.0, 1.5, 2.3, 4.0, 5.5 };
	double x = fabs(xf);
	complex<double> fresnel(0,0);
	if (x>5.5)
	{
		fresnel = complex<double>((1 - 3/(4*x*x)+75/(16*x*x*x*x)), (1/(2*x) - 15/(8*x*x*x)));
	}
	else if (x<0.3)
	{
		fresnel = (sqrt(M_PI*x) - 2*x*exp(complex<double>(0,M_PI/4)) - (2*x*x/3)*exp(complex<double>(0,-M_PI/4)))*exp(complex<double>(0,(M_PI/4+x)));
	}
	else
	{
		for ( int i = 0; i < 7; ++i ) 
		{
			if (x <= xx[ i+1 ] ) 
			{
				fresnel = fxx [ i + 1 ] * ( x - xx [ i + 1 ] ) + fx [ i + 1 ];
				break;
			}
		}
	}
	return (xf >= 0) ? fresnel : conj(fresnel);
}
bool algo::Domp(const vector<beamNode> &a, const vector<beamNode> &b)
{
	if(a.size() != b.size())
		return a.size() < b.size();
	else
		return false;
}



bool algo:: intersect(emxRay ray, Vector3d v0, Vector3d v1, Vector3d v2, Vector3d &intersectPoint)
{
	Vector3d E1 = v1 - v0;
	Vector3d E2 = v2 - v0;
	Vector3d S = ray.origin - v0;
	Vector3d S1 = VectorCross(ray.direction, E2);
	Vector3d S2 = VectorCross(S, E1);

	double denometor = Dot(S1, E1);
	if (fabs(denometor) > DOUBLE_EPSILON)
	{
		denometor = 1.0 / denometor;
		double t = Dot(S2, E2) * denometor;
		//if(t <= 0.0 || t >= ray.maxt || t <= ray.mint)		// avoid intersect with the edge of two triangles
		//	return false;
		if (t < DOUBLE_EPSILON || t > ray.maxt - DOUBLE_EPSILON || t < ray.mint + DOUBLE_EPSILON)		// avoid intersect with the edge of two triangles
			return false;
		//if(t <= 0.0 || fabs(t - ray.maxt)<=1e-3 || fabs(t - ray.mint)<=1e-3)		// avoid intersect with the edge of two triangles
		//	return false;
		double v = Dot(S1, S) * denometor;
		double w = Dot(S2, ray.direction) * denometor;
		double u = 1 - v - w;
		if (UnitValue(u) && UnitValue(v) && UnitValue(w))
		{
			//tHit = t;
			// Coordinate
			intersectPoint = ray.origin + ray.direction * t;
			return true;
		}
	}

	return false;
}



//************确定性BeamTracing 模型仿真************//
//initial beams creation
void algo::CreateInitialBeam(vector<emxBeam*> &pRootBeams, SphereBeam* SphereTest, Vector3d AP_position)  //位于原点处的单位球，细分count次生成初始beams
{
	//多次细分后，存放球上所有生成三角面顶点信息（此时不考虑天线位置，按照位于原点处生成beam）
	for (size_t k = 0; k < BeamNumber; k++)
	{
		emxBeam *pRootBeam = new emxBeam();
		emxFace *pFace = SphereTest->m_Face[k];
		pRootBeam->cornerNum = 3;
		pRootBeam->BeamVertex[0] = pFace->vertex[0]->GetPos() + AP_position;
		pRootBeam->BeamVertex[1] = pFace->vertex[1]->GetPos() + AP_position;
		pRootBeam->BeamVertex[2] = pFace->vertex[2]->GetPos() + AP_position;
		pRootBeam->origin = AP_position;
		pRootBeam->normal = VectorCross(pRootBeam->BeamVertex[1] - pRootBeam->BeamVertex[0], pRootBeam->BeamVertex[2] - pRootBeam->BeamVertex[0]).normalize();

		pRootBeam->dir[0] = (pRootBeam->BeamVertex[0] - pRootBeam->origin).normalize();
		pRootBeam->dir[1] = (pRootBeam->BeamVertex[1] - pRootBeam->origin).normalize();
		pRootBeam->dir[2] = (pRootBeam->BeamVertex[2] - pRootBeam->origin).normalize();
		pRootBeam->beam_depth = 1;

		pRootBeams.push_back(pRootBeam);
	}
}

void algo::	BeamTracing(emxKdTree* pKdTree, const int &reflectnum, emxBeam* &beam)
{
	int reflectnum1 = reflectnum;
	vector<emxBeam*> ProcessedBeams;
	ProcessedBeams.push_back(beam);
	for (int i_r = 1; i_r <= reflectnum1 + 1; i_r++)
	{
		vector<emxBeam*> nextToBeProcessedBeams;
		// collect the reflected beams of hit beams in previous intersection
		for (size_t j = 0; j < ProcessedBeams.size(); ++j)
		{
			vector<emxBeam*> beamStack, reflectedBeams, transmittedBeams;
			beamStack.push_back(ProcessedBeams[j]);
			while (!beamStack.empty())
			{
				emxBeam* pBeam = beamStack.back();
				beamStack.pop_back();
				if (pBeam->IsSplittedBeam())
				{
					beamStack.insert(beamStack.end(), pBeam->childBeams.begin(), pBeam->childBeams.end());
				}
				else if (pBeam->IsHitBeam())
				{
					if (pBeam->BeamArea() > DOUBLE_EPSILON)
					{
						pBeam->GeneratedReflectedBeam();
						reflectedBeams.push_back(pBeam->reflectedBeam);
					}
				}
				else
				{
					if (pBeam->beam_type == -1)
					{
						reflectedBeams.push_back(pBeam);
					}
				}
			}

			// the new generation reflected beams intersection
			if (i_r <= reflectnum1)
			{
				for (size_t m = 0; m < reflectedBeams.size(); ++m)
				{
					if (pKdTree->IntersectBeam(reflectedBeams[m]) && (reflectedBeams[m]->IsSplittedBeam() || reflectedBeams[m]->IsHitBeam()))
					{
						nextToBeProcessedBeams.push_back(reflectedBeams[m]);
					}
				}
			}
		}
		ProcessedBeams = nextToBeProcessedBeams;
	}
}

//找出pRootBeam指向的路径beam树中所有可能的路径
void algo::find_beamroute(emxBeam *pRootBeam, vector<vector<beamNode>> &routes)
{
	/*if(pRootBeam == NULL)
	{return;}*/

	if (pRootBeam != NULL)
	{
		vector<emxBeam*> beamStack;
		beamStack.push_back(pRootBeam);
		while (!beamStack.empty())
		{
			emxBeam* pBeam = beamStack.back();
			beamStack.pop_back();
			if (pBeam->IsSplittedBeam())
			{
				beamStack.insert(beamStack.end(), pBeam->childBeams.begin(), pBeam->childBeams.end());
			}
			else if (pBeam->IsHitBeam() && pBeam->reflectedBeam != NULL)
			{
				vector<beamNode> record;  //record记录的是逆向路径
				vector<beamNode> mecord; //通过对record进行逆向操作，进而得到实际的路径，存储在容器mecord
				emxBeam* subNode = NULL;
				subNode = new emxBeam(*pBeam->reflectedBeam);
				while (subNode != NULL)
				{
					if (subNode->beam_type != 2) //路径中仅保存反射、透射beam，不保存childbeams
					{
						beamNode NodeNow;
						NodeNow.beam_type = subNode->beam_type;
						NodeNow.cornerNum = subNode->cornerNum;
						NodeNow.origin = subNode->origin;
						NodeNow.beamNormal = subNode->normal;
						if (subNode->parentBeam != NULL)
						{
							NodeNow.faceID = subNode->parentBeam->hitFaceID;
						}
						else
						{
							NodeNow.faceID = -1;
						}
						for (int i = 0; i<NodeNow.cornerNum; i++)
						{
							NodeNow.BeamVertex[i] = subNode->BeamVertex[i];
						}
						record.push_back(NodeNow);
					}
					subNode = subNode->parentBeam;
				}
				if (subNode)
				{
					delete subNode;
					subNode = NULL;
				}
				for (int i = record.size() - 1; i >= 0; i--)
				{
					mecord.push_back(record[i]);
				}
				//routes.push_back(mecord);
#pragma omp critical
				{
					routes.push_back(mecord);
				}

				if ((pBeam->reflectedBeam)->IsHitBeam() || (pBeam->reflectedBeam)->IsSplittedBeam())
				{
					//find_beamroute(pBeam->reflectedBeam, routes);
					beamStack.push_back(pBeam->reflectedBeam);
				}
			}
		}
	}
}

//设置接收点坐标
//设置一个site内所有接收点的坐标
void algo::SetEFieldPoint(Site_Data* m_siteData, Vector3d AP_position, double LocalScene_range)
{
	double Left_Up_X = AP_position.x - LocalScene_range/2;
	double Left_Up_Y = AP_position.y + LocalScene_range/2;
	double Right_Down_X = AP_position.x + LocalScene_range/2;
	double Right_Down_Y = AP_position.y - LocalScene_range/2;
	
	//定义测试的点数，横向m等分，纵向n等分
 
	if (cptPara->precision <= 0.0)
	{
		QMessageBox::warning(NULL, QStringLiteral("接收处预测点分布"), QStringLiteral("请输入合适的分布精度"));
		return;
	}
	int width = (int)((Left_Up_Y - Right_Down_Y)/cptPara->precision);//row
	int length = (int)((Right_Down_X - Left_Up_X) / cptPara->precision);//col
	//			length
	//      ********************
	//		*				   *
	//width *				   *	
	//		*				   *
	//		********************
	//
	cout<<"width: "<<width<<"  length: "<<length<<endl;
	int num = length * width;

	int m = length;
	int n = width;
	scenePara.scene_length = m;
	scenePara.scene_width = n;
	double Xmin = Left_Up_X;
	double Ymin = Right_Down_Y;
	//double Xstep,Ystep;
	if (m == 0)
	{
		scenePara.Xstep = 0;
	}
	else
	{
		scenePara.Xstep = (Right_Down_X - Left_Up_X)/m;
	}
	if (n == 0)
	{
		scenePara.Ystep = 0;
	}
	else
	{
		scenePara.Ystep = (Left_Up_Y - Right_Down_Y)/n;
	}

	BaseModel *tmpModel;
	bool find=false;
	for (int i = 0; i < modelParameter->SiteModels.size();i++)
	{
		if (modelParameter->SiteModels[i]->getModelID()==m_siteData->siteID)
		{
			tmpModel = modelParameter->SiteModels[i];
			find = true;
			break;
		}
	}
	if (find)
	{
		for (auto it = m_siteData->cellsMap.begin(); it != m_siteData->cellsMap.end(); it++)
		{
			for (int i = 0; i < (m + 1)*(n + 1); i++)
			{
				EField *s = new EField;
				//先x再y ，也就是从下到上，再从左到右，绘制仿真面时候需要注意
				double x = Xmin + scenePara.Xstep*(i / (n + 1));
				double y = Ymin + scenePara.Ystep*(i % (n + 1));
				double z = cptPara->altitude+tmpModel->getPointAltitude(x, y);
				s->Position = Vector3d(x, y, z);
				it->second->efildVec.push_back(s);
			}
			//补全cell的信息
			it->second->row = width + 1;//width是栅格数，row是点的行数
			it->second->col = length + 1;
			it->second->pricision =cptPara->precision;
		}
	}
}





void algo::Point_In_Out(Site_Data *m_siteData, vector< Building > &Local_buildings)
{
	auto ce = cptPara->computeEnum;
	if (ce==ComputationEnum::ReceivePoint)
	{
		auto it = m_siteData->cellsMap.begin();
		for ( ;it!=m_siteData->cellsMap.end()  ;it++ )
		{
			for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end();it2++)
			{
				EField *NField = (*it2);
				for (int j = 0; j < Local_buildings.size();j++)
				{
					vector<Vector3d> upperFacePoint = Local_buildings[j].upper_facePoint;
					double building_height = upperFacePoint[0].z;   //建筑物的高度
					if (NField->Position.z >= building_height)
						continue;
					else if (contain(upperFacePoint, NField->Position))
					{
						NField->In_or_Out = 0;
						break;
					}
				}
			}

		}
	}
	else if (ce == ComputationEnum::SimuPlane)
	{
		//获得仿真面左下角
		Cell_Data *cell= m_siteData->cellsMap.begin()->second;
		//这里选择了一个cell，因为仿真面的多个cell内部位置相同。
		EField* tmpE = *(cell->efildVec.begin());
		double Xmin = tmpE->Position.x;
		double Ymin = tmpE->Position.y;

		int start_rowId, end_rowId, start_columnId, end_columnId;

		for (int j = 0; j < Local_buildings.size(); j++)
		{
			vector<Vector3d> upperFacePoint = Local_buildings[j].upper_facePoint;
			double building_height = upperFacePoint[0].z;  
			EfieldPointInPolygon(upperFacePoint, Xmin, Ymin, start_rowId, end_rowId, start_columnId, end_columnId);
			if (start_rowId*(start_rowId - scenePara.scene_width) <= 0 && end_rowId*(end_rowId - scenePara.scene_width) <= 0 && start_columnId*(start_columnId - scenePara.scene_length) <= 0 && end_columnId*(end_columnId - scenePara.scene_length) <= 0)
			{
				//如果建筑物的包围盒四条边都在仿真面的内部
				for (int columnId = start_columnId; columnId <= end_columnId; columnId++)
				{
					for (int rowId = start_rowId; rowId <= end_rowId; rowId++)
					{
						//对site里的所有cell进行筛选操作
						EField* tmpLocationE = cell->efildVec[columnId*(scenePara.scene_width + 1) + rowId];
						if (tmpLocationE->Position.z>=building_height)
						{
							continue;
						}
						else if (contain(upperFacePoint,tmpLocationE->Position))
						{
							//对site里的所有cell进行标识，标识当前位置的接收点在建筑物内部
							auto deleteIte = m_siteData->cellsMap.begin();
							for (; deleteIte != m_siteData->cellsMap.end();deleteIte++)
							{
								deleteIte->second->efildVec[columnId*(scenePara.scene_width + 1) + rowId]->In_or_Out = 0;
							}
							//标识完毕
						}
					}
				}
			}//如果建筑物不在仿真面内部，跳过
		}//结束对所有建筑的循环
	}

}

void algo::EfieldPointInPolygon(vector<Vector3d>& polygon, double Xmin, double Ymin, int &start_rowId, int &end_rowId, int &start_columnId, int &end_columnId)
{
	double start_x = polygon[0].x, start_y = polygon[0].y, end_x = polygon[0].x, end_y = polygon[0].y;
	for (int i = 1; i < polygon.size(); i++)
	{
		start_x = min(start_x, polygon[i].x);
		start_y = min(start_y, polygon[i].y);
		end_x = max(end_x, polygon[i].x);
		end_y = max(end_y, polygon[i].y);
	}
	start_columnId = ceil((start_x - Xmin) /scenePara.Xstep);
	end_columnId = floor((end_x - Xmin) / scenePara.Xstep);
	start_rowId = ceil((start_y - Ymin) / scenePara.Ystep);
	end_rowId = floor((end_y - Ymin) / scenePara.Ystep);

	if (start_rowId < 0)
	{
		start_rowId = 0;
	}
	if (start_columnId < 0)
	{
		start_columnId = 0;
	}
	if (end_rowId > scenePara.scene_width)
	{
		end_rowId = scenePara.scene_width;
	}
	if (end_columnId > scenePara.scene_length)
	{
		end_columnId = scenePara.scene_length;
	}
}

bool algo::contain(vector<Vector3d>& polygon, Vector3d point0)
{
	int crossings = 0;
	double x0 = point0.x;
	double y0 = point0.y;
	for (int k = 0;k<polygon.size();k++)
	{
		Vector3d point1 = polygon[k];
		Vector3d point2 = polygon[(k+1)%polygon.size()];
		double slope = (point2.y-point1.y)/(point2.x-point1.x);
		if ((point1.x >x0) != (point2.x > x0) && (y0 < slope * (x0 - point1.x) + point1.y))
			crossings++;
	}
	return (crossings %2 != 0);    //若为奇数则点在多边形内部
}

//找出有效的直达路径
void algo::valid_DirPath(emxKdTree* pKdTree, Vector3d AP_position, Site_Data *m_siteData)
{
	//先判断有没有直射电磁波到达
	for (auto it = m_siteData->cellsMap.begin(); it != m_siteData->cellsMap.end();it++)
	{
		for (int i = 0; i < it->second->efildVec.size(); i++)
		{
			if (!it->second->efildVec[i]->In_or_Out)  //对于位于建筑物内部的接收点不用考察是否发生直射
				continue;
			Vector3d direction = it->second->efildVec[i]->Position - AP_position;
			double length = direction.norm();
			emxRay directRay;
			int dirFaceid = -1;
			double dirHit = 0;
			directRay.direction = direction.normalize();
			directRay.origin = AP_position;
			directRay.lastID = -1;
			if (length > 1e-6 && (!pKdTree->Intersect(directRay, dirHit, dirFaceid) || dirHit >= 0.9999*length))
			{
				Field_Path ipath;
				ipath.Path_interPoint.push_back(AP_position);
				ipath.Path_interPoint.push_back(it->second->efildVec[i]->Position);

				//ipath.all_distance = length;
				it->second->efildVec[i]->Path.push_back(ipath);
			}
		}
	}
}


//计算反射、透射、绕射路径到达接收点的信号强度
void  algo::Calc_GO_UTD(TransAntenna &AP, vector<EField*>  &EFieldArray, vector<Vedge> &Edge_list, Antenna_Para * A_Para)
{
	A_Para->frequency = AP.frequency * 1e6;
	A_Para->light_speed = 299792458;
	A_Para->epsilon0 =  1e-9/(36*M_PI);
	A_Para->lamda = A_Para->light_speed/A_Para->frequency;
	A_Para->k = 2 * M_PI * A_Para->frequency /A_Para->light_speed; 
	A_Para->w = 2 * M_PI * A_Para->frequency;

	double initPowerStrength = pow(10,(AP.trans_power + AP.enlarge_power - AP.wire_loss)/10); //发射源初始功率，由dBm转换为mW
	A_Para->EveryRayPowerStrength = initPowerStrength*1e-3; //每条源发射射线携带的功率，由mW转为W
	//获取全局坐标系中的正北方向在场景坐标系下的方位角phi,3D天线方向增益phi是相对于正北方向的夹角，因此当下坐标系统获取的phi需转换到正北方向的夹角
	A_Para->NorthAngle = cptPara->phi;


#pragma omp parallel for schedule(static,2)
	for (int i=0; i<EFieldArray.size(); i++)
	{
		EField *NField = EFieldArray[i];

		NField->HorizontalDis = sqrt((NField->Position.x - AP.position.x)*(NField->Position.x - AP.position.x) + (NField->Position.y - AP.position.y)*(NField->Position.y - AP.position.y));   //发射点和接收点之间的水平距离

		//剔除重复路径
		for (int id1= 0; id1<NField->Path.size(); id1++) 
		{
			Field_Path path1 = NField->Path[id1];
			if (path1 .Path_interPoint.size() >= 3 && path1.propagation_type[1] != 2)  //此条路径为反、透射路径
			{			
				for (int id2 = id1+1;id2<NField->Path.size();id2++)
				{
					Field_Path path2 = NField->Path[id2];
					if (path2.intersect_ID.size() == path1.intersect_ID.size() && path2.propagation_type[1] != 2)
					{
						bool same_Path = true;
						for (int id3 = 1; id3 < path2.intersect_ID.size()-1; id3++)
						{
							if ((path2.Path_interPoint[id3] - path1.Path_interPoint[id3]).norm() < DOUBLE_EPSILON)
							{
								same_Path &= true;
							}
							else
							{
								same_Path &= false;
								break;
							}
						}
						if (same_Path)
						{
							NField->Path.erase(NField->Path.begin() + id2);
							id2--;
						}
					}
				}
			}			
		}

		for (int j = 0; j<NField->Path.size(); j++)
		{
			//此条路径为直射（源点、接收点组成的路径）
			Calc_Signal(NField,AP,j,Edge_list,A_Para);
		}
	}
}

void algo::Calc_Signal(EField *NField, TransAntenna &AP, int &path_id, vector<Vedge> &Edge_list, Antenna_Para* aPara)
{
	Vector3d direction = (NField->Path[path_id].Path_interPoint[1] - NField->Path[path_id].Path_interPoint[0]).normalize();
	double length = direction.norm();
	//Tpolor是初始场强方向
	Vector3d Tpolor = AP.polor_direction.normalize() - Dot(AP.polor_direction.normalize(),direction.normalize())*(direction.normalize());  //此处的direction是不是需要normalize？？

	if(Tpolor.norm() < 1e-10)
	{
		//此条路径无效，需剔除
		NField->Path.erase(NField->Path.begin()+path_id);
		path_id--;
	}
	else
	{
		Tpolor = Tpolor.normalize();

		//原始射线方向向量，theta角不变，phi需变换为相对于正北方向的夹角
		int Hnum = int(asin(fabs(direction.y)/sqrt(direction.x*direction.x + direction.y*direction.y))*180/M_PI+0.5);  //射线的水平方向角phi
		if(direction.x >= 0 && direction.y >= 0)
		{
		
		}
		else if(direction.x < 0 && direction.y >= 0)
		{
			Hnum = 180 - Hnum;
		}
		else if(direction.x <= 0 && direction.y <= 0)
		{
			Hnum = 180 + Hnum;
		}
		else
		{
			Hnum = 360 - Hnum;
			if(Hnum == 360)
				Hnum = 0;
		}
		Hnum = aPara->NorthAngle - Hnum;
		if (Hnum < 0)
		{
			Hnum = Hnum + 360;
		}
		int Vnum = int(acos(fabs(direction.z)/(direction.norm()))*180/M_PI + 0.5); //射线的垂直方向角theta
		if(direction.z < 0)
		{
			Vnum = 180 - Vnum;
		}
		aPara->TP_gain = AP.initial_Gain - AP.direction_Gain[Hnum*181 + Vnum][2];
		double EveryRayEFieldStrength = sqrt(30 * aPara->EveryRayPowerStrength * pow(10, aPara->TP_gain / 10)) / length;  //射线携带的功率转换为射线场强值大小
		Vector3d tfield = Tpolor * EveryRayEFieldStrength;  //第一项为射线初始场强方向，第二项为射线初始场强大小
		Vector3cd ctfield = Vector3cd(tfield.x, tfield.y, tfield.z);
		double raydistance = length;

		for (int l = 1; l < NField->Path[path_id].Path_interPoint.size() - 1;l++)
		{
			double currentTOnext_distance = (NField->Path[path_id].Path_interPoint[l + 1] - NField->Path[path_id].Path_interPoint[l]).norm(); //当前相交面处点到下次场点（相交点）之间的距离
			if (NField->Path[path_id].propagation_type[l] == 0) //反射
			{
				double R_T_d_factor = raydistance / (raydistance + currentTOnext_distance);  //反射、透射扩散因子

				Vector3d Into = (NField->Path[path_id].Path_interPoint[l] - NField->Path[path_id].Path_interPoint[l - 1]).normalize();
				Vector3d next_Into = (NField->Path[path_id].Path_interPoint[l + 1] - NField->Path[path_id].Path_interPoint[l]).normalize();
				Vector3d Normal = NField->Path[path_id].intersect_faceNormal[l]; //反射、透射beam的normal
				Calc_RefEfield(ctfield, Into, Normal, next_Into,AP);

				ctfield *= R_T_d_factor;
			}
			else if (NField->Path[path_id].propagation_type[l] == 1) //透射
			{
				if (cptPara->transIndex == 0) //使用经验透射损耗值
				{
					//	使用经验透射损耗值 透射损耗要可设置，一般是有两个参数：透射损耗，表示信号穿透墙体后的衰减，比如：城市建筑物设置15dB的透射损耗；
					//	另一个参数，透射递进损耗，表示室外信号穿透到室内后，不清楚请室内布局的情况下，随着信号覆盖深入，信号强度随距离继续衰减，比如：一般设置为0.5dB/m
					double wall_loss = 10;
					double dis_loss = 0.5;
					double tf_distance = currentTOnext_distance;
					double total_loss = wall_loss + dis_loss * tf_distance;  //从透射点到达透射场点之间的损耗：透射损耗（穿透墙体损耗）+ 透射递进损耗（进入室内随距离损耗）
					ctfield = ctfield  * complex<double>((1 / pow(10, (total_loss / 2) / 10)), 0);  //功率的损耗，计算场强损耗时，应该是total_loss/2 (功率是场强的平方，转成dB就有个2倍关系)
				}
				else if (cptPara->transIndex==1)
				{
					double R_T_d_factor = raydistance / (raydistance + currentTOnext_distance);  //反射、透射扩散因子

					Vector3d Into = (NField->Path[path_id].Path_interPoint[l] - NField->Path[path_id].Path_interPoint[l - 1]).normalize();
					Vector3d Normal = NField->Path[path_id].intersect_faceNormal[l]; //反射、透射beam的normal
					Calc_TransEfield(ctfield, Into, Normal,AP);

					ctfield *= R_T_d_factor;
				}
			}
			else if (NField->Path[path_id].propagation_type[l] == 2) // 绕射
			{
				double Diff_d_factor = sqrt(raydistance / (currentTOnext_distance*(raydistance + currentTOnext_distance)));

				Vector3d source_pos = NField->Path[path_id].Path_interPoint[l - 1];
				Vector3d Diffract_Pos = NField->Path[path_id].Path_interPoint[l];
				Vector3d field_pos = NField->Path[path_id].Path_interPoint[l + 1];
				Vedge the_edge = Edge_list[NField->Path[path_id].intersect_ID[l]];
				Calc_diffEfield(ctfield, source_pos, Diffract_Pos, field_pos, the_edge,AP,aPara);

				ctfield *= Diff_d_factor;
			}
			raydistance += currentTOnext_distance;
		}
		ctfield = ctfield *  exp(complex<double>(0, -aPara->k*raydistance));  //考虑整条路径的相位影响
		NField->EFieldAll += ctfield;     //电场叠加
	
		NField->Path[path_id].all_distance = raydistance;
		double path_RecPower = aPara->lamda *aPara-> lamda * (ctfield.x.real()*ctfield.x.real() + ctfield.x.imag()*ctfield.x.imag() + ctfield.y.real()*ctfield.y.real() + ctfield.y.imag()*ctfield.y.imag() + ctfield.z.real()*ctfield.z.real() + ctfield.z.imag()*ctfield.z.imag()) / (4 * M_PI * 120 * M_PI);
		NField->Path[path_id].power_Loss = 10 * log10(aPara->EveryRayPowerStrength / path_RecPower);
	}	
}

void algo::Calc_RefEfield(Vector3cd &ctfield, Vector3d Into, Vector3d Normal, Vector3d next_Into, TransAntenna &AP)
{
	//int materialId = pMesh->getMtlId(rayRoute[l].faceId); //获取面片对应的材质编号
	int materialId = 11; //获取面片对应的材质编号
	complex<double> Epsilon = getEpsilon(AP.frequency, materialId); //折射处介质的相对介电常数，此处为复相对介电常数
	//complex<double> Epsilon = complex<double>(6.0,  -1.0/(w * 1e-9 / (36 * M_PI)));  //复相对介电常数
	double costheta = fabs(Dot(Into, Normal));  //入射角的cos值
	double square_sintheta = 1 - costheta*costheta;

	complex<double> EV, EP;//反射垂直极化系数、平行极化系数
	complex<double> VD1, VD2, PD1, PD2;//垂直极化方向反射前后的分量，平行极化反射前后的分量
	Vector3d RV1, RP1, RV2, RP2; //反射前后的垂直极化方向，平行极化方向

	EV = (costheta - sqrt(Epsilon - square_sintheta)) / (costheta + sqrt(Epsilon - square_sintheta));
	EP = (Epsilon*costheta - sqrt(Epsilon - square_sintheta)) / (Epsilon*costheta + sqrt(Epsilon - square_sintheta));

	//求反射后的垂直极化分量，平行极化分量
	if (costheta > 0.9999999999999)
	{
		//垂直入射时，反射波场强
		ctfield = ctfield*((complex<double>(1, 0) - sqrt(Epsilon)) / (complex<double>(1, 0) + sqrt(Epsilon)));
	}
	else
	{
		RV1 = VectorCross(Into, Normal).normalize();
		RP1 = VectorCross(RV1, Into).normalize();
		RV2 = RV1;
		RP2 = VectorCross(RV2, next_Into).normalize();


		//反射后垂直极化分量VD2数值大小改变体现在系数里面
		VD1 = Dot(ctfield, RV1);
		VD2 = VD1*EV;
		//反射后平行极化分量PD2数值大小改变体现在系数里面
		PD1 = Dot(ctfield, RP1);
		PD2 = PD1*EP;

		//反射后电场方向（一般为非单位向量，经过系数衰减）
		ctfield = RV2*VD2 + RP2*PD2;

	}
}

void algo::Calc_TransEfield(Vector3cd &ctfield, Vector3d Into, Vector3d Normal, TransAntenna &AP)
{
	//int materialId = pMesh->getMtlId(rayRoute[l].faceId); //获取面片对应的材质编号
	int materialId = 11; //获取面片对应的材质编号
	complex<double> Epsilon = getEpsilon(AP.frequency, materialId); //折射处介质的相对介电常数，此处为复相对介电常数
	//complex<double> Epsilon = complex<double>(6.0,  -1.0/(w * 1e-9 / (36 * M_PI)));  //复相对介电常数
	double costheta = fabs(Dot(Into, Normal));  //入射角的cos值
	double square_sintheta = 1 - costheta*costheta;

	complex<double> EV, EP;//垂直极化衰减系数、平行极化衰减系数
	complex<double> VD1, VD2, PD1, PD2;//垂直极化方向折射前后的分量，平行极化折射前后的分量
	Vector3d TV1, TP1, TV2, TP2; //透射前后的垂直极化方向，平行极化方向

	EV = (costheta * 2) / (costheta + sqrt(Epsilon - square_sintheta));   //垂直极化透射系数
	EP = (2 * costheta*sqrt(Epsilon)) / (Epsilon*costheta + sqrt(Epsilon - square_sintheta));   //水平极化波透射系数

	//	求透射后的垂直极化分量，平行极化分量
	if (costheta > 0.9999999999999)
	{
		//	垂直入射时，透射波场强
		ctfield = ctfield*(complex<double>(2, 0) / (complex<double>(1, 0) + sqrt(Epsilon)));
	}
	else
	{
		TV1 = VectorCross(Into, Normal).normalize();
		TP1 = VectorCross(TV1, Into).normalize();
		//透射后垂直、水平极化方向与透射前一致
		TV2 = TV1;
		TP2 = TP1;

		//折射后垂直极化分量VD2大小改变
		VD1 = Dot(ctfield, TV1); //透射前垂直极化分量数值
		VD2 = VD1*EV;

		//折射前后平行极化分量PD2大小改变
		PD1 = Dot(ctfield, TP1); //透射前平行极化分量数值
		PD2 = PD1*EP;

		//折射后电场方向（一般为非单位向量，经过系数衰减）
		ctfield = TV2*VD2 + TP2*PD2;
	}
}

void algo::Calc_diffEfield(Vector3cd &ctfield, Vector3d source_pos, Vector3d Diffract_Pos, Vector3d field_pos, Vedge the_edge, TransAntenna &AP, Antenna_Para *aPara)
{
	Vector3d inVec = Diffract_Pos - source_pos;
	Vector3d diffVec = field_pos - Diffract_Pos;
	double lengthSD = inVec.norm();
	double lengthDF = diffVec.norm();
	inVec = inVec.normalize();
	diffVec = diffVec.normalize();
	getDiffEdgeINfor(the_edge, source_pos, Diffract_Pos);
	Vector3d edgeDir = (the_edge.end - the_edge.start).normalize();
	Vector3d normal = the_edge.normal_front;

	Vector3d xDir;
	xDir = VectorCross(normal, edgeDir);

	Vector3d projInVec = -inVec - edgeDir * Dot(-inVec, edgeDir);
	projInVec = projInVec.normalize();
	double phiS = acos(max(-1.0, min(1.0, Dot(projInVec, xDir))));  //入射平面和楔形正面（前面）的夹角

	Vector3d projdiffVec = (diffVec - edgeDir * Dot(diffVec, edgeDir));
	projdiffVec = projdiffVec.normalize();
	double phi = acos(max(-1.0, min(1.0, Dot(projdiffVec, xDir))));  //绕射平面和楔形正面（前面）的夹角

	if (Dot(diffVec, normal) < -DOUBLE_EPSILON)
		phi = 2.0 * M_PI - phi;

	double FaceInteriorAngle = M_PI - acos(max(-1.0, min(1.0, Dot(the_edge.normal_front, the_edge.normal_back))));
	double n = 2 - FaceInteriorAngle / M_PI;//绕射尖劈，最接近入射面（即定义的正面或前面）的劈面为0面，另一面为n面，劈内角为 FaceInteriorAngle = (2-n)*PI
	double L = ((lengthSD * lengthDF) / (lengthSD + lengthDF));

	complex<double> epsilon = getEpsilon(AP.frequency, the_edge.materialId);

	complex<double> prevD = -exp(complex<double>(0, -M_PI / 4.0)) / (2 * n * sqrt(2 * M_PI*aPara->k));

	double gama[4] = {
		(M_PI - (phi - phiS)) / (2 * n), (M_PI + (phi - phiS)) / (2 * n),
		(M_PI - (phi + phiS)) / (2 * n), (M_PI + (phi + phiS)) / (2 * n)
	};
	int N[4] = {
		round((-M_PI + (phi - phiS)) / (2 * n*M_PI)), round((M_PI + (phi - phiS)) / (2 * n*M_PI)),
		round((-M_PI + (phi + phiS)) / (2 * n*M_PI)), round((M_PI + (phi + phiS)) / (2 * n*M_PI))
	};
	double a[4] = {
		2 * cos((2 * n*M_PI*N[0] - (phi - phiS)) / 2) *cos((2 * n*M_PI*N[0] - (phi - phiS)) / 2),
		2 * cos((2 * n*M_PI*N[1] - (phi - phiS)) / 2) *cos((2 * n*M_PI*N[1] - (phi - phiS)) / 2),
		2 * cos((2 * n*M_PI*N[2] - (phi + phiS)) / 2) *cos((2 * n*M_PI*N[2] - (phi + phiS)) / 2),
		2 * cos((2 * n*M_PI*N[3] - (phi + phiS)) / 2) *cos((2 * n*M_PI*N[3] - (phi + phiS)) / 2),
	};

	double epsilonp[4] = {
		M_PI - ((phi - phiS) - 2 * n*M_PI*N[0]) + 1e-7, M_PI + ((phi - phiS) - 2 * n*M_PI*N[1]),
		M_PI - ((phi + phiS) - 2 * n*M_PI*N[2]) + 1e-7, M_PI + ((phi + phiS) - 2 * n*M_PI*N[3])
	};
	complex<double> D[4];

	for (int j = 0; j < 4; ++j)
	{
		if (fabs(gama[j]) < DOUBLE_EPSILON) //当gama[j]趋于0时，cot(gama[j])趋于无穷，但与此同时，其相应的过渡函数F趋于零，从而消除了奇异性。阴影边界（phi-phiS = M_PI）和反射边界处(phi+phiS = M_PI)
		{
			complex<double> fresnel = n *(sqrt(2 * M_PI*aPara->k*L)*sign_func(epsilonp[j]) - 2 * aPara->k*L*epsilonp[j] * exp(complex<double>(0, M_PI / 4))) * exp(complex<double>(0, M_PI / 4));

			D[j] = prevD * fresnel;
		}
		else
			D[j] = prevD * cot(gama[j]) * Fresnel(aPara->k*L*a[j]);
	}

	complex<double> R0v(-1), R0p(1), Rnv(-1), Rnp(1);   //if perfectly conducting  R0和Rn分别是入射面和绕射面的反射因子

	//////////////////////////////////////////////////////////////////////////
	double theta0 = phiS;
	double thetan = n*M_PI - phi;
	//if not perfectly conducting  有耗介质
	if (1)
	{
		R0v = (sin(theta0) - sqrt(epsilon - cos(theta0) * cos(theta0)))
			/ (sin(theta0) + sqrt(epsilon - cos(theta0) * cos(theta0)));
		R0p = (epsilon * sin(theta0) - sqrt(epsilon - cos(theta0) * cos(theta0)))
			/ (epsilon * sin(theta0) + sqrt(epsilon - cos(theta0) * cos(theta0)));
		Rnv = (sin(thetan) - sqrt(epsilon - cos(thetan) * cos(thetan)))
			/ (sin(thetan) + sqrt(epsilon - cos(thetan) * cos(thetan)));
		Rnp = (epsilon * sin(thetan) - sqrt(epsilon - cos(thetan) * cos(thetan)))
			/ (epsilon * sin(thetan) + sqrt(epsilon - cos(thetan) * cos(thetan)));
	}
	// TE/TM 
	Vector3d phipv;  //入射垂直极化波方向，TE
	Vector3d betapv; //入射平行极化波方向，TM 
	Vector3d phiv; //绕射垂直极化波方向，TE
	Vector3d betav; //绕射平行极化波方向，TM 
	phipv = (VectorCross(inVec, edgeDir)).normalize();
	betapv = (VectorCross(phipv, inVec)).normalize();
	phiv = (VectorCross(edgeDir, diffVec)).normalize();
	betav = (VectorCross(phiv, diffVec)).normalize();

	Vector3d e_v0 = VectorCross(inVec, the_edge.normal_front).normalize();
	Vector3d e_p0 = VectorCross(e_v0, inVec).normalize();
	Vector3d e_vn = VectorCross(diffVec, the_edge.normal_back).normalize();
	Vector3d e_pn = VectorCross(e_vn, diffVec).normalize();

	double sin_alpha1 = max(-1.0, min(1.0, Dot(e_p0, phipv)));
	double sin_alpha2 = max(-1.0, min(1.0, Dot(e_pn, phipv)));;   //????????
	double cos_alpha1 = sqrt(1 - sin_alpha1*sin_alpha1);   //????????
	double cos_alpha2 = sqrt(1 - sin_alpha2*sin_alpha2);   //????????
	complex<double> Dss = D[0] + D[1] - (R0p*cos_alpha1*cos_alpha1 - R0v*sin_alpha1*sin_alpha1)*D[2] - (Rnp*cos_alpha2*cos_alpha2 - Rnv*sin_alpha2*sin_alpha2)*D[3];
	complex<double> Dhh = D[0] + D[1] + (R0p*sin_alpha1*sin_alpha1 - R0v*cos_alpha1*cos_alpha1)*D[2] + (Rnp*sin_alpha2*sin_alpha2 - Rnv*cos_alpha2*cos_alpha2)*D[3];
	complex<double> Dsh = -(R0p + R0v)*cos_alpha1*sin_alpha1*D[2] - (Rnp + Rnv)*cos_alpha2*sin_alpha2*D[3];
	complex<double> Dhs = (R0p + R0v)*cos_alpha1*sin_alpha1*D[2] + (Rnp + Rnv)*cos_alpha2*sin_alpha2*D[3];

	complex<double> Ei_betap = Dot(ctfield, betapv); //入射场的平行极化分量
	complex<double> Ei_phip = Dot(ctfield, phipv); //入射场的垂直极化分量
	complex<double> Ed_beta = (Dss * Ei_betap + Dsh * Ei_phip); //绕射场的平行极化分量
	complex<double> Ed_phi = (Dhs * Ei_betap + Dhh * Ei_phip); //绕射场的垂直极化分量

	ctfield = Ed_beta*betav + Ed_phi*phiv;
}



bool algo::getDiffEdgeINfor(Vedge& info, const Vector3d& source_pos, const Vector3d& diffract_pos)
{
	//分别求入射光线与夹棱边两面法向量的夹角，来判断相对于入射光线而言，哪个是前面，哪个是背面
	if(Dot(source_pos - diffract_pos, info.normal_front) < DOUBLE_EPSILON  && Dot(source_pos - diffract_pos, info.normal_back) > DOUBLE_EPSILON)
	{
		std::swap(info.normal_front, info.normal_back);
		std::swap(info.start, info.end);
	}
	return true;
}

complex<double>algo::getEpsilon(double freq, int materialId)   //相对介电常数        /*单位:HZ*/
{
	double w = 2 * M_PI * freq ;
	int index = getMaterial(freq,materialId);
	double dielectric = modelParameter->materialdatabase[index].dielectric;
	double conductivity = modelParameter->materialdatabase[index].conductivity;
	return std::complex<double>(dielectric, 	-conductivity / (w * 1e-9 / (36 * M_PI)));  //复介电常数

}

int algo::getMaterial(double freq /*单位:HZ*/, int materialId)
{
	double dif = DBL_MAX;
	int MatVector_index;
	for (int i=0;i<modelParameter->materialdatabase.size();i++)
	{
		if (materialId == modelParameter->materialdatabase[i].Id )
		{
			if (fabs(freq - (modelParameter->materialdatabase[i].frequency/*单位:MHZ*/) * 1e6) < dif)
			{
				dif = fabs(freq - (modelParameter->materialdatabase[i].frequency) * 1e6);
				MatVector_index = i;
			}
		}
	}
	return MatVector_index;
}

//找出有效的反透射路径，直接由beam与虚拟仿真面的覆盖区域获取接收点信息，未考虑海拔时
void algo::valid_RefTransPath(BaseModel* model, emxKdTree* pKdTree, Vector3d AP_position, Site_Data* m_siteData, const vector<vector<beamNode>> &beamRoutes)
{
#pragma omp parallel for schedule(static,2)
	for (int beam_id = 0; beam_id < beamRoutes.size(); beam_id++)  //容器beamRoutes存放的所有可能路径
	{
		vector<beamNode> beamroute = beamRoutes[beam_id];
		int FinalBeamNum = beamroute.size() - 1;
		beamNode FinalBeam = beamroute[FinalBeamNum];

		int start_rowId, end_rowId, start_columnId, end_columnId;    //beam与仿真面区域相交覆盖的接收点下标
		bool illuminate=false;

		for (int beam_type = 0; beam_type <= 1; beam_type++)  //beam_type=0对反射beam的操作，beam_type=1对透射beam操作
		{
			//初始化
			illuminate = false;
			start_rowId = -1;
			end_rowId = -1;
			start_columnId = -1;
			end_columnId = -1;

			//计算beam与仿真面区域的相交
			Vector3d origin;
			if (beam_type == 0)
			{
				//反射beam源点
				origin = FinalBeam.origin;
			}
			else
			{
				//透射beam源点
				Vector3d TransBeamOrigin = FinalBeam.origin - FinalBeam.beamNormal * (2 * Dot(FinalBeam.beamNormal, FinalBeam.origin - FinalBeam.BeamVertex[0]));
				origin = TransBeamOrigin;
			}

			/*added by ligen
			如果是接收点模式，那EFieldArray存所有接收点
			如果是仿真面模式，那EFieldArray存一个单独的仿真面上的接受点，在计算路径之后，将该仿真面上的接收点数据复制到其他仿真面
			*/
			vector<EField*> EfildArray;
			if (cptPara->computeEnum==ComputationEnum::ReceivePoint)
			{
				for (auto it = m_siteData->cellsMap.begin(); it != m_siteData->cellsMap.end(); it++)
				{
					for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
					{
						EfildArray.push_back(*(it2));
					}
				}
			}
			else if (cptPara->computeEnum==ComputationEnum::SimuPlane)
			{
				//只放入第一个cell的仿真面的点，其余的点复制这个面的路径信息
				auto it = m_siteData->cellsMap.begin(); 			
				for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
				{
					EfildArray.push_back(*(it2));
				}	
			}

			if (cptPara->computeEnum == ComputationEnum::ReceivePoint)
			{
				start_rowId = 0;
				end_rowId = EfildArray.size() - 1;
				start_columnId = 0;
				end_columnId = 0;
				illuminate = true;
			}
			else if (cptPara->computeEnum == ComputationEnum::SimuPlane)
			{
				double Xmin = m_siteData->cellsMap.begin()->second->efildVec[0]->Position.x;
				double Ymin = m_siteData->cellsMap.begin()->second->efildVec[0]->Position.y;
				double Zheight = m_siteData->cellsMap.begin()->second->efildVec[0]->Position.z;
			//	assert(Zheight ==m_siteData->cellsMap.begin()->second->efildVec[0]->Position.z);

				Vector3d Point_IN_SimPlane(Xmin, Ymin, Zheight);
				Vector3d SimPlane_Normal(0, 0, 1);

				//获取beam源点位置处仿真面的高度
			
				if (fabs(origin.z - Zheight) < DOUBLE_EPSILON)  //beam的源点在仿真面上，此时不可能存在覆盖区域
					continue;

				bool ALLbackward = true;   //判断是否beam的所有棱边都背离仿真面传播
			
				for (int i = 0; i < FinalBeam.cornerNum; i++)
				{
					Vector3d direction = (FinalBeam.BeamVertex[i] - origin).normalize();  //???????????		
					if (origin.z>Zheight)
					{
						ALLbackward &= (Dot(direction, SimPlane_Normal) >= 0);
					}
					else if (origin.z < Zheight)
					{
						ALLbackward &= (Dot(direction, SimPlane_Normal) <= 0);
					}
				}
				if (ALLbackward)
					continue;

				//beam相交情况
				double tmax[] = { 0.0, 0.0, 0.0, 0.0 };	// distance from beam origin to SimPlane
				bool behind = true;
				bool Paral = false;
				bool anyone_backward = false;   //判断beam是否有部分（1<= ？？  <棱边数目）棱边背离仿真面传播
				for (int i = 0; i < FinalBeam.cornerNum; i++)
				{
					Vector3d direction = (FinalBeam.BeamVertex[i] - origin).normalize();  //???????????
					double invDDotN;
					if (fabs(Dot(direction, SimPlane_Normal)) < DOUBLE_EPSILON)  //射线方向与面片方向平行时
					{
						Paral = true;
						break;
					}
					//判断beam是否有部分棱边背向仿真面传播
					else if ((origin.z>Zheight &&Dot(direction, SimPlane_Normal) > 0) || (origin.z < Zheight &&Dot(direction, SimPlane_Normal) < 0))
					{
						anyone_backward = true;
						break;
					}
					else
					{
						invDDotN = 1.0 / Dot(direction, SimPlane_Normal);
						tmax[i] = -Dot(FinalBeam.BeamVertex[i] - Point_IN_SimPlane, SimPlane_Normal) * invDDotN;
						behind &= (tmax[i] < DOUBLE_EPSILON);
					}
				}

				// 获取beam覆盖的仿真面区域对应的接收点
				if (Paral || anyone_backward)  //beam的的某条边与仿真面平行或者beam的部分棱边背离仿真面，无法求得相交区域，则需对所有接收点验证有效性
				{
					start_rowId = 0;
					end_rowId = scenePara.scene_width;
					start_columnId = 0;
					end_columnId = scenePara.scene_length;
					illuminate = true;
				}
				else if (behind)	//若存在相交情况，但是仿真面均在beam平面之前，即不存在有效相交区域
				{
					illuminate = false;
				}
				else
				{
					//反射beam或者透射beam与仿真面存在相交区域
					Vector3d illuminateArea[5];
					int illunimateNum_p = 0;
					for (int i = 0; i < FinalBeam.cornerNum; i++)
					{
						Vector3d direction = (FinalBeam.BeamVertex[i] - origin).normalize();
						Vector3d next_direction = (FinalBeam.BeamVertex[(i + 1) % FinalBeam.cornerNum] - origin).normalize();

						if (tmax[i] >= -DOUBLE_EPSILON)
							illuminateArea[illunimateNum_p++] = FinalBeam.BeamVertex[i] + direction * tmax[i];
						//穿透时的交点
						if (fabs(tmax[i]) > DOUBLE_EPSILON && fabs(tmax[(i + 1) % FinalBeam.cornerNum]) > DOUBLE_EPSILON && tmax[i] * tmax[(i + 1) % FinalBeam.cornerNum] < 0.0)
						{
							Vector3d A = FinalBeam.BeamVertex[i];
							Vector3d C = FinalBeam.BeamVertex[(i + 1) % FinalBeam.cornerNum];
							Vector3d direction = (C - A).normalize();
							double t = Dot((Point_IN_SimPlane - A), SimPlane_Normal) / Dot(direction, SimPlane_Normal);
							illuminateArea[illunimateNum_p++] = A + direction*t;
						}
					}
					vector<Vector3d> illunimatre_Vertexs;
					for (int illuminate_id = 0; illuminate_id < illunimateNum_p; illuminate_id++)
					{
						illunimatre_Vertexs.push_back(illuminateArea[illuminate_id]);
					}
					EfieldPointInPolygon(illunimatre_Vertexs, Xmin, Ymin, start_rowId, end_rowId, start_columnId, end_columnId);
					illuminate = true;
				}

			}

			//对beam覆盖的接收点计算信号强度
			if (illuminate && (cptPara->computeEnum == ComputationEnum::ReceivePoint || start_rowId*(start_rowId - scenePara.scene_width) <= 0 && end_rowId*(end_rowId - scenePara.scene_width) <= 0 && start_columnId*(start_columnId - scenePara.scene_length) <= 0 && end_columnId*(end_columnId - scenePara.scene_length) <= 0))
			{
				for (int columnId = start_columnId; columnId <= end_columnId; columnId++)
				{
					for (int rowId = start_rowId; rowId <= end_rowId; rowId++)
					{
						if (cptPara->computeEnum == ComputationEnum::ReceivePoint)  //只有在仿真面设置的接收点位置时候，才有scene_width，而对于非仿真面设置的接收点，不存在scene_width
						{
							scenePara.scene_width = 0;
						}
						EField *NField = EfildArray[columnId*(scenePara.scene_width + 1) + rowId];
						Vector3d receiver = NField->Position;

						if (NField->In_or_Out)   //透射只对建筑物内的接收点考察
						{
							if (beam_type == 1)
								continue;
						}
						else  //反射只对建筑物外的接收点考察
						{
							if (beam_type == 0)
								continue;
						}

						emxRay Finalray;
						Finalray.origin = origin;
						Finalray.direction = (receiver - Finalray.origin).normalize();
						Finalray.mint = 0;
						Finalray.maxt = (receiver - Finalray.origin).norm();
						Finalray.lastID = -1;
						Vector3d intersectPoint;
						bool valid = false;
						if (FinalBeam.cornerNum == 3)
						{
							valid = intersect(Finalray, FinalBeam.BeamVertex[0], FinalBeam.BeamVertex[1], FinalBeam.BeamVertex[2], intersectPoint);
						}
						else if (FinalBeam.cornerNum == 4)
						{
							valid = intersect(Finalray, FinalBeam.BeamVertex[0], FinalBeam.BeamVertex[1], FinalBeam.BeamVertex[2], intersectPoint) ||
								intersect(Finalray, FinalBeam.BeamVertex[2], FinalBeam.BeamVertex[3], FinalBeam.BeamVertex[0], intersectPoint);
						}
						if (valid)
						{
							Finalray.origin = intersectPoint;
							Finalray.direction = (receiver - intersectPoint).normalize();
							Finalray.mint = 0;
							Finalray.maxt = (receiver - Finalray.origin).norm();
							Finalray.lastID = -1;
							double finalHit = 0;
							int finalFaceid = -1;
							vector<Vector3d>path_point, inverse_pathpoint; //存放传播过程路径点信息,invese_pathpoint中存储的是逆向路径信息
							if (((receiver - intersectPoint).norm() > 1e-6) && (!pKdTree->Intersect(Finalray, finalHit, finalFaceid) || (finalHit >= 0.9999 *(receiver - intersectPoint).norm())))
							{
								if (beam_type == 1) //此条路径有效，且最后一次为透射
								{
									beamroute[FinalBeamNum].beam_type = 1;
								}

								inverse_pathpoint.push_back(intersectPoint);
								for (int j = beamroute.size() - 2; j >= 1; j--)
								{
									Vector3d direction = (intersectPoint - beamroute[j].origin).normalize();
									double dist = Dot(beamroute[j].BeamVertex[0] - beamroute[j].origin, beamroute[j].beamNormal) / Dot(direction, beamroute[j].beamNormal);
									intersectPoint = beamroute[j].origin + direction * dist;
									inverse_pathpoint.push_back(intersectPoint);
								}
								path_point.push_back(AP_position);
								for (int i = inverse_pathpoint.size() - 1; i >= 0; i--)
								{
									path_point.push_back(inverse_pathpoint[i]);
								}
								path_point.push_back(receiver);

								Field_Path ipath;
								ipath.Path_interPoint = path_point;
								for (int j = 0; j < beamroute.size(); j++)
								{
									ipath.propagation_type.push_back(beamroute[j].beam_type);
									ipath.intersect_faceNormal.push_back(beamroute[j].beamNormal);
									ipath.intersect_ID.push_back(beamroute[j].faceID);
								}
								#pragma omp critical
								{
									NField->Path.push_back(ipath);
									if (cptPara->computeEnum==ComputationEnum::SimuPlane)
									{
										//在该site的其他cell中加入同样的信息
										auto itMap = m_siteData->cellsMap.begin();
										itMap++;//跳过自己
										while (itMap != m_siteData->cellsMap.end())
										{
											itMap->second->efildVec[columnId*(scenePara.scene_width + 1) + rowId]->Path.push_back(ipath);
											itMap++;
										}
									}

								}
							}
							vector<Vector3d>().swap(path_point);
							vector<Vector3d>().swap(inverse_pathpoint);
						}
					}
				}
			}
		}
		vector<beamNode>().swap(beamroute);
	}
}

void algo::valid_OnceDiffPath(vector<Vedge> &Edge_list, emxKdTree* pKdTree, Vector3d AP_position, Site_Data*m_siteData)
{
	Vector3d source_pos = AP_position;
#pragma omp parallel for  schedule(static,2)
	for (int i = 0; i < Edge_list.size(); i++)
	{
		Vedge the_edge = Edge_list[i];
		Vector3d vInit = the_edge.start;
		Vector3d vEnd = the_edge.end;
		//发射源点必须位于绕射棱边两侧面之外
		if (Dot(AP_position - vInit, the_edge.normal_front) < DOUBLE_EPSILON && Dot(AP_position - vInit, the_edge.normal_back) < DOUBLE_EPSILON/*|| !LOS_edge[i]*/)
		{
			//	count ++;
			continue;
		}

		//通过设定源点与棱边的距离阈值来限制绕射发生
		Vector3d edge_center = 0.5*(the_edge.start + the_edge.end);
		if ((source_pos - edge_center).norm()>Diff_dis)
			continue;

		//粗略判断棱边是否对源可见
		emxRay ray;
		double ray_tHit = 0;
		int ray_faceID = -1;
		ray.lastID = ray_faceID;
		ray.origin = source_pos;
		ray.direction = (edge_center - source_pos).normalize();

		if (pKdTree->Intersect(ray, ray_tHit, ray_faceID) && (ray_tHit <= 0.999 * (edge_center - source_pos).norm()))  //判断从发射点到绕射棱边中点是否有遮挡,若有遮挡则认为此条棱边对源不可见
			continue;

		Vector3d lineDirection = vEnd - vInit;
		Vector3d line_unitDirection = lineDirection.normalize();

		//遍历所有站点信息，放入到EfildArray中
		/*added by ligen
		如果是接收点模式，那EFieldArray存所有接收点
		如果是仿真面模式，那EFieldArray存一个单独的仿真面上的接受点，在计算路径之后，将该仿真面上的接收点数据复制到其他仿真面
		*/
		vector<EField*> EFieldArray;
		if (cptPara->computeEnum == ComputationEnum::ReceivePoint)
		{
			for (auto it = m_siteData->cellsMap.begin(); it != m_siteData->cellsMap.end(); it++)
			{
				for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
				{
					EFieldArray.push_back(*(it2));
				}
			}
		}
		else if (cptPara->computeEnum == ComputationEnum::SimuPlane)
		{
			//只放入第一个cell的仿真面的点，其余的点复制这个面的路径信息
			auto it = m_siteData->cellsMap.begin();
			for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
			{
				EFieldArray.push_back(*(it2));
			}
		}
		//接收点设置完毕

		for (int j = 0; j < EFieldArray.size(); j++)
		{
			if (!EFieldArray[j]->In_or_Out)  //只对位于建筑物外的接收点考察是否发生绕射
				continue;

			EField *NField = EFieldArray[j];
			Vector3d field_pos = NField->Position;

			//接收点必须位于绕射棱边两侧面之外
			if (Dot(field_pos - vInit, the_edge.normal_front) < DOUBLE_EPSILON && Dot(field_pos - vInit, the_edge.normal_back) < DOUBLE_EPSILON)
				continue;

			//通过设定接收点与棱边的距离阈值来限制绕射发生
			if ((field_pos - edge_center).norm()>Diff_dis)
				continue;

			//double time3 = clock();

			Vector3d line1_Direction = source_pos - vInit;
			Vector3d line2_Direction = field_pos - vInit;

			double line_Length1 = line1_Direction.norm();
			double line_Length2 = line2_Direction.norm();
			double shadowLength1 = Dot(line1_Direction, line_unitDirection);
			double shadowLength2 = Dot(line2_Direction, line_unitDirection);
			Vector3d Intesect_Tranfer = vInit + shadowLength1 * line_unitDirection;
			Vector3d Intesect_Receive = vInit + shadowLength2 * line_unitDirection;
			double dis_Transfer = sqrt(line_Length1 * line_Length1 - shadowLength1 * shadowLength1);
			double dis_Receiver = sqrt(line_Length2 * line_Length2 - shadowLength2 * shadowLength2);
			Vector3d Diffract_Pos;
			Diffract_Pos = (dis_Receiver * Intesect_Tranfer + dis_Transfer*Intesect_Receive) / (dis_Transfer + dis_Receiver);

			//	double time4= clock();
			//	time_Valid += (time4-time3)/1000;

			if (Dot(Diffract_Pos - vInit, Diffract_Pos - vEnd) < 0)//几何绕射点位于棱边上
			{
				bool isDiff = 0;  //用变量isDiff判断是否存在绕射
				//判断两边是否可见
				emxRay Rayfirst;
				emxRay Raysecond;
				Vector3d RayDirect1(Diffract_Pos - source_pos);
				Vector3d RayDirect2(field_pos - Diffract_Pos);
				double tHit(0);
				int faceID = -1;
				double sll = 0;
				Rayfirst.lastID = faceID;
				Rayfirst.origin = source_pos/* point->position*/;
				Rayfirst.direction = RayDirect1.normalize();

				if (!pKdTree->Intersect(Rayfirst, tHit, faceID) || (tHit > 0.999 * (Diffract_Pos - source_pos).norm()))  //判断从发射点到绕射点是否有遮挡
				{

					sll = (Diffract_Pos - field_pos).norm();
					Raysecond.lastID = -1;

					Raysecond.origin = Diffract_Pos;
					Raysecond.direction = RayDirect2.normalize();
					tHit = 0;

					if (!pKdTree->Intersect(Raysecond, tHit, Raysecond.lastID) || tHit > 0.999*sll)  //判断从绕射点到接收点是否有遮挡
					{
						isDiff = 1;
					}
					if ((Diffract_Pos - source_pos).norm() < 1e-10 || (field_pos - Diffract_Pos).norm() < 1e-10)
					{
						isDiff = 0;
					}
				}

				//double time5 = clock();
				//time_Valid += (time5-time4)/1000;

				if (isDiff)//存在此绕射
				{
					Field_Path ipath;
					ipath.Path_interPoint.push_back(source_pos);
					ipath.propagation_type.push_back(-1);  //源点处设置为-1
					ipath.intersect_ID.push_back(-1);

					ipath.Path_interPoint.push_back(Diffract_Pos);
					ipath.propagation_type.push_back(2);  //2表示传播为绕射
					ipath.intersect_ID.push_back(i);

					ipath.Path_interPoint.push_back(field_pos);

#pragma omp critical
					{
						NField->Path.push_back(ipath);
						if (cptPara->computeEnum == ComputationEnum::SimuPlane)
						{
							//在该site的其他cell中加入同样的信息
							auto itMap = m_siteData->cellsMap.begin();
							itMap++;//跳过自己
							while (itMap != m_siteData->cellsMap.end())
							{
								itMap->second->efildVec[j]->Path.push_back(ipath);
								itMap++;
							}
						}
					}
				}
			}
		}
	}
}

void algo::valid_RefDiffPath(emxKdTree* pKdTree, vector<Vedge> &Edge_list, Vector3d AP_position,Site_Data* m_siteData, const vector<vector<beamNode>> &beamRoutes)
{
#pragma omp parallel for schedule(static,4)
	for (int beam_id = 0; beam_id < beamRoutes.size(); beam_id++)  //容器beamRoutes存放的所有可能路径
	{
		vector<beamNode> beamroute = beamRoutes[beam_id];
		if (beamroute.size() <= 2)
		{
			continue;
		}
		int FinalBeamNum = beamroute.size() - 1;
		beamNode FinalBeam = beamroute[FinalBeamNum];

		int edge_id = -1;
		int beamedge_id = -1;
		if (is_diffEdge(FinalBeam, Edge_list, beamedge_id, edge_id))
		{
			Vedge the_edge = Edge_list[edge_id];
			Vector3d vInit = the_edge.start;
			Vector3d vEnd = the_edge.end;
			Vector3d edge_center = 0.5*(vInit + vEnd);

			Vector3d source_pos = beamroute[beamroute.size() - 2].origin;
			//发射源点必须位于绕射棱边两侧面之外
			if (Dot(source_pos - vInit, the_edge.normal_front) < DOUBLE_EPSILON && Dot(source_pos - vInit, the_edge.normal_back) < DOUBLE_EPSILON)
			{
				continue;
			}

			Vector3d lineDirection = vEnd - vInit;
			Vector3d line_unitDirection = lineDirection.normalize();

			//遍历所有站点信息，放入到EfildArray中
			/*added by ligen
			如果是接收点模式，那EFieldArray存所有接收点
			如果是仿真面模式，那EFieldArray存一个单独的仿真面上的接受点，在计算路径之后，将该仿真面上的接收点数据复制到其他仿真面
			*/
			vector<EField*> EfildArray;
			if (cptPara->computeEnum == ComputationEnum::ReceivePoint)
			{
				for (auto it = m_siteData->cellsMap.begin(); it != m_siteData->cellsMap.end(); it++)
				{
					for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
					{
						EfildArray.push_back(*(it2));
					}
				}
			}
			else if (cptPara->computeEnum == ComputationEnum::SimuPlane)
			{
				//只放入第一个cell的仿真面的点，其余的点复制这个面的路径信息
				auto it = m_siteData->cellsMap.begin();
				for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
				{
					EfildArray.push_back(*(it2));
				}
			}

			for (int efIndex = 0; efIndex <EfildArray.size(); efIndex++)
			{
				if (!EfildArray[efIndex]->In_or_Out)  //只对位于建筑物外的接收点考察是否发生绕射
					continue;

				EField *NField = EfildArray[efIndex];
				Vector3d field_pos = NField->Position;

				//接收点必须位于绕射棱边两侧面之外
				if (Dot(field_pos - vInit, the_edge.normal_front) < DOUBLE_EPSILON && Dot(field_pos - vInit, the_edge.normal_back) < DOUBLE_EPSILON)
					continue;

				//通过设定接收点与棱边的距离阈值来限制绕射发生
				if ((field_pos - edge_center).norm()>Diff_dis)
					continue;

				Vector3d line1_Direction = source_pos - vInit;
				Vector3d line2_Direction = field_pos - vInit;

				double line_Length1 = line1_Direction.norm();
				double line_Length2 = line2_Direction.norm();
				double shadowLength1 = Dot(line1_Direction, line_unitDirection);
				double shadowLength2 = Dot(line2_Direction, line_unitDirection);
				Vector3d Intesect_Tranfer = vInit + shadowLength1 * line_unitDirection;
				Vector3d Intesect_Receive = vInit + shadowLength2 * line_unitDirection;
				double dis_Transfer = sqrt(line_Length1 * line_Length1 - shadowLength1 * shadowLength1);
				double dis_Receiver = sqrt(line_Length2 * line_Length2 - shadowLength2 * shadowLength2);
				// 求出准确的绕射点位置：Diffract_Pos
				Vector3d Diffract_Pos;
				Diffract_Pos = (dis_Receiver * Intesect_Tranfer + dis_Transfer*Intesect_Receive) / (dis_Transfer + dis_Receiver);

				//判断绕射点是否在beam的边上					
				if (Dot(Diffract_Pos - FinalBeam.BeamVertex[(beamedge_id - 1 + FinalBeam.cornerNum) % FinalBeam.cornerNum], Diffract_Pos - FinalBeam.BeamVertex[beamedge_id]) < 0)
				{
					bool isDiff = 0;
					emxRay Ray;
					double tHit = 0;
					int faceID = -1;
					double sll = 0;
					Ray.lastID = faceID;
					Ray.origin = Diffract_Pos;
					Ray.direction = (field_pos - Diffract_Pos).normalize();
					sll = (Diffract_Pos - field_pos).norm();

					if (!pKdTree->Intersect(Ray, tHit, Ray.lastID) || tHit > 0.999*sll)  //判断从绕射点到接收点是否有遮挡
					{
						isDiff = 1;
					}
					//绕射的实际发射源点
					int beam_id = beamroute.size() - 2;
					Vector3d direction1 = (Diffract_Pos - beamroute[beam_id].origin).normalize();
					double dist1 = Dot(beamroute[beam_id].BeamVertex[0] - beamroute[beam_id].origin, beamroute[beam_id].beamNormal) / Dot(direction1, beamroute[beam_id].beamNormal);
					Vector3d intersectPoint = beamroute[beam_id].origin + direction1 * dist1;
					if ((field_pos - Diffract_Pos).norm() < DOUBLE_EPSILON || (Diffract_Pos - intersectPoint).norm() < DOUBLE_EPSILON)
					{
						isDiff = 0;
					}

					if (isDiff)//存在此绕射
					{
						vector<Vector3d>path_point, inverse_pathpoint; //存放传播过程路径点信息,invese_pathpoint中存储的是逆向路径信息
						inverse_pathpoint.push_back(Diffract_Pos);
						inverse_pathpoint.push_back(intersectPoint);
						for (int j = beamroute.size() - 3; j >= 1; j--)
						{
							Vector3d direction = (intersectPoint - beamroute[j].origin).normalize();
							double dist = Dot(beamroute[j].BeamVertex[0] - beamroute[j].origin, beamroute[j].beamNormal) / Dot(direction, beamroute[j].beamNormal);
							intersectPoint = beamroute[j].origin + direction * dist;
							inverse_pathpoint.push_back(intersectPoint);
						}
						path_point.push_back(AP_position);
						for (int i = inverse_pathpoint.size() - 1; i >= 0; i--)
						{
							path_point.push_back(inverse_pathpoint[i]);
						}
						path_point.push_back(field_pos);

						Field_Path ipath;
						ipath.Path_interPoint = path_point;
						for (int j = 0; j <= beamroute.size() - 2; j++)
						{
							ipath.propagation_type.push_back(beamroute[j].beam_type);
							ipath.intersect_faceNormal.push_back(beamroute[j].beamNormal);
							ipath.intersect_ID.push_back(beamroute[j].faceID);
						}
						//最后一次为绕射
						ipath.propagation_type.push_back(2);  //2表示绕射
						ipath.intersect_ID.push_back(edge_id);

#pragma omp critical
						{
							NField->Path.push_back(ipath);

						  //复制给站点的其他cell的仿真面 
							//added by ligen
							if (cptPara->computeEnum == ComputationEnum::SimuPlane)
							{
								//在该site的其他cell中加入同样的信息
								auto itMap = m_siteData->cellsMap.begin();
								itMap++;//跳过自己
								while (itMap != m_siteData->cellsMap.end())
								{
									itMap->second->efildVec[efIndex]->Path.push_back(ipath);
									itMap++;
								}
							}

						}
						vector<Vector3d>().swap(path_point);
						vector<Vector3d>().swap(inverse_pathpoint);
					}
				}
			}
		}
	}
}



bool algo::is_diffEdge(beamNode &the_beam, vector<Vedge> &Edge_list, int &beamedge_id, int &edge_id)
{
	for (int i = 0; i < Edge_list.size(); i++)
	{
		Vedge the_edge = Edge_list[i];
		Vector3d start = the_edge.start;
		Vector3d end = the_edge.end;
		double edge_length = (end - start).norm();

		int count = 0;
		for (int j = 0; j < the_beam.cornerNum; j++)
		{
			Vector3d A = the_beam.BeamVertex[j];
			if (fabs((A - start).norm() + (A - end).norm() - edge_length) < DOUBLE_EPSILON)  //判断点A是否在棱边the_edge上
			{
				count++;
				beamedge_id = j;
			}
		}
		if (count >= 2)
		{
			edge_id = i;
			return true;
		}
	}
	return false;
}

void algo::Get_diffVEdge(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<vector<int>>&diff_RouteEdgeID, vector<vector<int>> & NextDifEdgesID, int diff_Num, int first_edgeid)
{
	vector<int>first_route;
	first_route.push_back(first_edgeid);
	vector<vector<int>> tobeProcessedEdge; //记录待处理的棱边
	tobeProcessedEdge.push_back(first_route);

	for (int id = 0; id<diff_Num; id++)
	{
		vector<vector<int>> nextTobeProcessedEdge;
		for (int id1 = 0; id1<tobeProcessedEdge.size(); id1++)
		{
			int prev_edgeid = tobeProcessedEdge[id1][tobeProcessedEdge[id1].size() - 1];    //获取该条路径最后一条棱边id号(在Vertical_Edge_ID中的索引号)
			for (int i = 0; i<NextDifEdgesID[prev_edgeid].size(); i++)
			{
				vector<int> diff_route(tobeProcessedEdge[id1]);
				int current_edgeid = NextDifEdgesID[prev_edgeid][i];
				diff_route.push_back(current_edgeid);
				/*	if (id == diff_Num-1)
				{
				diff_RouteEdgeID.push_back(diff_route);
				}*/
				diff_RouteEdgeID.push_back(diff_route);

				nextTobeProcessedEdge.push_back(diff_route);
			}
		}
		tobeProcessedEdge = nextTobeProcessedEdge;
	}
}

void algo::Get_SourceLosEdgesInfo(Vector3d source_pos, vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, map<int, Source_LosEdgeInfo> &Source_LosEdges)
{
	for (int i = 0; i < Vertical_Edge_ID.size(); i++)
	{
		int the_edge1_ID = Vertical_Edge_ID[i];
		Vedge the_edge1 = Edge_list[the_edge1_ID];
		Vector3d vInit1 = the_edge1.start;
		Vector3d vEnd1 = the_edge1.end;

		//发射源点必须位于绕射棱边两侧面之外
		if (Dot(source_pos - vInit1, the_edge1.normal_front) < DOUBLE_EPSILON && Dot(source_pos - vInit1, the_edge1.normal_back) < DOUBLE_EPSILON)
		{
			continue;
		}
		//通过设定源点与棱边的距离阈值来限制绕射发生
		Vector3d edge_center1 = 0.5*(the_edge1.start + the_edge1.end);
		if ((source_pos - edge_center1).norm()>Diff_dis)
			continue;

		//粗略判断棱边是否对源可见
		emxRay ray;
		double ray_tHit = 0;
		int ray_faceID = -1;
		ray.lastID = ray_faceID;
		ray.origin = source_pos;
		ray.direction = (edge_center1 - source_pos).normalize();
		if (pKdTree->Intersect(ray, ray_tHit, ray_faceID) && (ray_tHit <= 0.999 * (edge_center1 - source_pos).norm()))  //判断从发射点到绕射棱边1的中点是否有遮挡,若有遮挡则认为此条棱边1对源不可见
			continue;

		//对垂直棱边可用以下方式计算
		Vector3d Intesect_Source = Vector3d(vInit1.x, vInit1.y, source_pos.z);   //源点在棱边上的垂直映射
		double dis_source = (Intesect_Source - source_pos).norm();   //源点和棱边的垂直距离

		Source_LosEdgeInfo LosEdge;
		LosEdge.dis_sourceToEdge = dis_source;
		LosEdge.source_onEdge = Intesect_Source;

		Source_LosEdges.insert(map<int, Source_LosEdgeInfo>::value_type(i, LosEdge));	//将对源可见的棱边信息保存下来
	}
}

void algo::Get_nextDifEdge(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<vector<int>> & NextDifEdgesID)
{
#pragma omp parallel for schedule(static,2)
	for (int j = 0; j < Vertical_Edge_ID.size(); j++)
	{
		int prev_edgeid = Vertical_Edge_ID[j];
		Vedge prev_edge = Edge_list[prev_edgeid];
		int current_edgeid = -1;
		Vector3d prev_edge_center = 0.5*(prev_edge.start + prev_edge.end);
		vector<int> nextDifEdgeID;
		for (int i = 0; i<Vertical_Edge_ID.size(); i++)
		{
			if (i != j)
			{
				current_edgeid = Vertical_Edge_ID[i];
				Vedge the_current_edge = Edge_list[current_edgeid];

				//通过设定相邻两棱边的距离阈值来限制绕射发生
				Vector3d current_edge_center = 0.5*(the_current_edge.start + the_current_edge.end);
				if ((prev_edge_center - current_edge_center).norm()>Diff_dis)
					continue;

				//棱边1必须位于绕射棱边2两侧面之外,棱边2也必须位于绕射棱边1两侧面之外
				if ((Dot(prev_edge_center - the_current_edge.start, the_current_edge.normal_front) < DOUBLE_EPSILON && Dot(prev_edge_center - the_current_edge.start, the_current_edge.normal_back) < DOUBLE_EPSILON) ||
					(Dot(current_edge_center - prev_edge.start, prev_edge.normal_front) < DOUBLE_EPSILON && Dot(current_edge_center - prev_edge.start, prev_edge.normal_back) < DOUBLE_EPSILON))
				{
					continue;
				}

				//粗略判断两棱边是否直接可见
				emxRay ray;
				double ray_tHit = 0;
				int ray_faceID = -1;
				ray.lastID = ray_faceID;
				ray.origin = prev_edge_center;
				ray.direction = (current_edge_center - prev_edge_center).normalize();
				if (pKdTree->Intersect(ray, ray_tHit, ray_faceID) && (ray_tHit <= 0.999 *  (current_edge_center - prev_edge_center).norm()))  //判断从edge1中点到edge2棱边中点是否有遮挡,若有遮挡则认为两棱边不可见
					continue;

				nextDifEdgeID.push_back(i);
			}
		}
		NextDifEdgesID[j] = nextDifEdgeID;
	}
}

void algo::Get_EdgeLosFieldPoint(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<EField*> &EFieldArray, vector<vector<Edge_LosPointInfo>> & edges_LosFieldPoints)
{
#pragma omp parallel for schedule(static,2)
	for (int j = 0; j < Vertical_Edge_ID.size(); j++)
	{
		int edgeid = Vertical_Edge_ID[j];
		Vedge edge = Edge_list[edgeid];
		Vector3d edge_center = 0.5*(edge.start + edge.end);
		vector<Edge_LosPointInfo> edge_FieldPoints;

		for (int i = 0; i<EFieldArray.size(); i++)
		{
			if (!EFieldArray[i]->In_or_Out)  //只对位于建筑物外的接收点考察是否发生绕射
				continue;

			Vector3d field_pos = EFieldArray[i]->Position;

			//通过设定棱边与接收点距离阈值来限制绕射发生
			if ((field_pos - edge_center).norm()>Diff_dis)
				continue;

			//接收点必须位于绕射棱边两侧面之外
			if (Dot(field_pos - edge.start, edge.normal_front) < DOUBLE_EPSILON && Dot(field_pos - edge.start, edge.normal_back) < DOUBLE_EPSILON)
			{
				continue;
			}

			//粗略判断棱边是否对接收点直接可见
			emxRay ray;
			double ray_tHit = 0;
			int ray_faceID = -1;
			ray.lastID = ray_faceID;
			ray.origin = edge_center;
			ray.direction = (field_pos - edge_center).normalize();
			if (pKdTree->Intersect(ray, ray_tHit, ray_faceID) && (ray_tHit <= 0.999 *  (field_pos - edge_center).norm()))  //判断从棱边中点到接收点是否有遮挡,若有遮挡则认为不可见
				continue;

			//对垂直棱边可用以下方式计算
			Vector3d Intesect_Receive = Vector3d(edge.start.x, edge.start.y, field_pos.z);
			double dis_Receiver = (Intesect_Receive - field_pos).norm();

			Edge_LosPointInfo LosPoint;
			LosPoint.ReceiverID = i;
			LosPoint.dis_edgeToReceiver = dis_Receiver;
			LosPoint.Receiver_onEdge = Intesect_Receive;
			edge_FieldPoints.push_back(LosPoint);	//将每条棱边通过简单判断能够到达的接收点索引号保存下来
		}
		edges_LosFieldPoints[j] = edge_FieldPoints;
	}
}

void algo::valid_NVDiffPath(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, Vector3d AP_position, Site_Data* m_siteData, int diff_Num)
{
	Vector3d source_pos = AP_position;
	vector<vector<int>> total_diff_RouteEdgeID; //所有 <= N次绕射的路径序列

	/*
	和之前的所有绕射计算的函数一样，这里会生成一个临时的EFieldArray
	如果是接收点模式，那EFieldArray存所有接收点
	如果是仿真面模式，那EFieldArray存一个单独的仿真面上的接受点，在计算路径之后，将该仿真面上的接收点数据复制到其他仿真面
	*/
	vector<EField*> EfildArray;
	if (cptPara->computeEnum == ComputationEnum::ReceivePoint)
	{
		for (auto it = m_siteData->cellsMap.begin(); it != m_siteData->cellsMap.end(); it++)
		{
			for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
			{
				EfildArray.push_back(*(it2));
			}
		}
	}
	else if (cptPara->computeEnum == ComputationEnum::SimuPlane)
	{
		//只放入第一个cell的仿真面的点，其余的点复制这个面的路径信息
		auto it = m_siteData->cellsMap.begin();
		for (auto it2 = it->second->efildVec.begin(); it2 != it->second->efildVec.end(); it2++)
		{
			EfildArray.push_back(*(it2));
		}
	}



	double time1 = clock();
	//**********************预处理提前计算一些可重用的信息**********************//
	//获取对源可见的棱边集合
	map<int, Source_LosEdgeInfo>Source_LosEdges;
	Get_SourceLosEdgesInfo(source_pos, Vertical_Edge_ID, Edge_list, pKdTree, Source_LosEdges);
	double time5 = clock();
	cout << "the source los edge: " << (time5 - time1) / 1000 << endl;

	//获取每条棱边下一可能发射绕射的棱边集合
	vector<vector<int>> NextDifEdgesID(Vertical_Edge_ID.size());
	Get_nextDifEdge(Vertical_Edge_ID, Edge_list, pKdTree, NextDifEdgesID);
	double time6 = clock();
	cout << "the next diff edge: " << (time6 - time5) / 1000 << endl;

	//获取每条棱边简单判断的可到达接收点索引号
	vector<vector<Edge_LosPointInfo>> edges_LosFieldPoints(Vertical_Edge_ID.size());
	Get_EdgeLosFieldPoint(Vertical_Edge_ID, Edge_list, pKdTree, EfildArray, edges_LosFieldPoints);
	double time7 = clock();
	cout << "the edge los field point: " << (time7 - time6) / 1000 << endl;

	//******************************************************************************//
	double time2 = clock();
	cout << "the preprocessing time is: " << (time2 - time1) / 1000 << endl;

	map<int, Source_LosEdgeInfo>::iterator m_iter;
	vector<int> Los_EdgeID;
	for (m_iter = Source_LosEdges.begin(); m_iter != Source_LosEdges.end(); m_iter++)
	{
		Los_EdgeID.push_back(m_iter->first);
	}
	cout << "the Los EDGE: " << Los_EdgeID.size() << endl;

	//获取所有可能的绕射棱边传播序列
#pragma omp parallel for schedule(static,2)
	for (int i = 0; i < Los_EdgeID.size(); i++)
	{
		int the_edge_ID = Los_EdgeID[i];

		//获取绕射传播棱边序列
		vector<vector<int>> diff_RouteEdgeID;
		Get_diffVEdge(Vertical_Edge_ID, Edge_list, pKdTree, diff_RouteEdgeID, NextDifEdgesID, diff_Num - 1, the_edge_ID);

#pragma omp critical
		{
			total_diff_RouteEdgeID.insert(total_diff_RouteEdgeID.end(), diff_RouteEdgeID.begin(), diff_RouteEdgeID.end());
		}

	}
	double time3 = clock();
	cout << "get the possible edge path: " << (time3 - time2) / 1000 << endl;

	//验证路径有效性并计算出绕射点
#pragma omp parallel for schedule(static,2)
	for (int j = 0; j < total_diff_RouteEdgeID.size(); j++)
	{
		//判断当前路径是否有能够到达的接收点
		int ID = total_diff_RouteEdgeID[j][total_diff_RouteEdgeID[j].size() - 1];  //edges_LosFieldPoints是和Vertical_Edge_ID中相对应的，所以要取和Vertical_Edge_ID中相对应的ID
		if (edges_LosFieldPoints[ID].size() == 0)  //没有能够到达的接收点
			continue;

		vector<int> diff_route;
		for (int m = 0; m < total_diff_RouteEdgeID[j].size(); m++)
		{
			diff_route.push_back(Vertical_Edge_ID[total_diff_RouteEdgeID[j][m]]);  //total_diff_RouteEdgeID中的每条路径序列中的棱边id号是和Vertical_Edge_ID中相对应的，所以需要做一个转换，取该id号在Vertical_Edge_ID中的对应值，即为实际棱边id号（对应Edge_list中）
		}
		double total_horizontal_dis = 0;
		vector<double> Distance;   //当前棱边到起始棱边之间，相邻两棱边的水平距离之和
		Distance.push_back(0);
		for (int route_id = 1; route_id < diff_route.size(); route_id++)
		{
			total_horizontal_dis += (Vector3d(Edge_list[diff_route[route_id]].start.x, Edge_list[diff_route[route_id]].start.y, 0) - Vector3d(Edge_list[diff_route[route_id - 1]].start.x, Edge_list[diff_route[route_id - 1]].start.y, 0)).norm();
			Distance.push_back(total_horizontal_dis);
		}
		int the_firstedgeID = diff_route[0];
		Vedge the_firstedge = Edge_list[the_firstedgeID];
		map<int, Source_LosEdgeInfo>::iterator m_iter1 = Source_LosEdges.find(total_diff_RouteEdgeID[j][0]);
		Source_LosEdgeInfo the_firstedgeInfo = m_iter1->second;
		Vector3d Intesect_Tranfer = the_firstedgeInfo.source_onEdge;
		double dis_Transfer = the_firstedgeInfo.dis_sourceToEdge;

		for (int l = 0; l < edges_LosFieldPoints[ID].size(); l++)
		{
			EField *NField = EfildArray[edges_LosFieldPoints[ID][l].ReceiverID];
			Vector3d field_pos = NField->Position;

			Vector3d Intesect_Receive = edges_LosFieldPoints[ID][l].Receiver_onEdge;
			double dis_Receiver = edges_LosFieldPoints[ID][l].dis_edgeToReceiver;
			Vector3d Intesect_Receive_onEdge1(the_firstedge.start.x, the_firstedge.start.y, Intesect_Receive.z);  //接收点水平投影到第一条绕射棱边，对垂直棱边可用此方式计算

			vector<Vector3d> path_point;   //绕射路径中间绕射点信息
			double current_horizontalDis = 0;
			Vector3d prevDifPos = source_pos, currentDifPos_onEdge1, currentDifPos;
			bool is_diff = true;  //用变量isDiff判断是否存在绕射
			//判断对该接收点而言此条绕射路径是否有效，并计算出中间传播棱边上的绕射点
			for (int id = 0; id < diff_route.size(); id++)
			{
				Vedge current_edge = Edge_list[diff_route[id]];
				current_horizontalDis = Distance[id];
				currentDifPos_onEdge1 = ((dis_Receiver + total_horizontal_dis - current_horizontalDis) * Intesect_Tranfer + (dis_Transfer + current_horizontalDis)*Intesect_Receive_onEdge1) / (dis_Transfer + dis_Receiver + total_horizontal_dis);  //计算得到的是当前绕射点在起始棱边上的垂直映射
				currentDifPos = Vector3d(current_edge.start.x, current_edge.start.y, currentDifPos_onEdge1.z);   //将在起始棱边上的投影绕射点再映射到该发生绕射的棱边得到真正的绕射点坐标
				path_point.push_back(currentDifPos);
				if (Dot(currentDifPos - current_edge.start, currentDifPos - current_edge.end) < 0 && (currentDifPos - prevDifPos).norm() >= DOUBLE_EPSILON)  //几何绕射点位于棱边上
				{
					//判断从上一绕射点到当前绕射点是否无遮挡
					emxRay Rayfirst;
					Vector3d RayDirect1(currentDifPos - prevDifPos);
					double tHit = 0;
					int faceID = -1;
					Rayfirst.lastID = faceID;
					Rayfirst.origin = prevDifPos;
					Rayfirst.direction = RayDirect1.normalize();
					if (!pKdTree->Intersect(Rayfirst, tHit, faceID) || (tHit > 0.999 * RayDirect1.norm())) //判断从上一绕射点到当前绕射点是否无遮挡
					{
						is_diff &= true;
						prevDifPos = currentDifPos;
					}
					else
					{
						is_diff &= false;
						break;
					}
				}
				else
				{
					is_diff &= false;
					break;
				}
			}
			if (is_diff && path_point.size() > 0)    //从发射点到最后一个绕射点之间均有效
			{
				//判断从最后一个绕射点到接收点之间是否无遮挡
				emxRay Raysecond;
				Vector3d RayDirect2(field_pos - prevDifPos);
				double tHit = 0;
				int faceID = -1;
				Raysecond.lastID = faceID;
				Raysecond.origin = prevDifPos;
				Raysecond.direction = RayDirect2.normalize();
				if (RayDirect2.norm() >= DOUBLE_EPSILON && (!pKdTree->Intersect(Raysecond, tHit, faceID) || tHit > 0.999*RayDirect2.norm()))
				{
					Field_Path ipath;
					ipath.Path_interPoint.push_back(source_pos);
					ipath.propagation_type.push_back(-1);  //源点处设置为-1
					ipath.intersect_ID.push_back(-1);
					for (int path_point_id = 0; path_point_id < path_point.size(); path_point_id++)
					{
						ipath.Path_interPoint.push_back(path_point[path_point_id]);
						ipath.propagation_type.push_back(2);  //2表示传播为绕射
						ipath.intersect_ID.push_back(diff_route[path_point_id]);
					}
					ipath.Path_interPoint.push_back(field_pos);
#pragma omp critical
					{
						NField->Path.push_back(ipath);

						/************************************************************************/
						/* 如果是仿真面，就传送到其他点                                               */
						/************************************************************************/
						if (cptPara->computeEnum == ComputationEnum::SimuPlane)
						{
							//在该site的其他cell中加入同样的信息
							auto itMap = m_siteData->cellsMap.begin();
							itMap++;//跳过自己
							while (itMap != m_siteData->cellsMap.end())
							{
								itMap->second->efildVec[j]->Path.push_back(ipath);
								itMap++;
							}
						}
					}
				}
			}
		}
	}
	double time4 = clock();
	cout << "the path validation and geting path info : " << (time4 - time3) / 1000 << endl;

	//清除内存
	vector<vector<int>>().swap(NextDifEdgesID);
	vector<vector<int>>().swap(total_diff_RouteEdgeID);
	map<int, Source_LosEdgeInfo>().swap(Source_LosEdges);
	vector<vector<Edge_LosPointInfo>>().swap(edges_LosFieldPoints);
	vector<int>().swap(Los_EdgeID);
}

algo::algo()
{
	Diff_dis = 800;
	m_logText=new LogText;
	modelParameter = nullptr;
	cptPara = nullptr;
	vPara = nullptr;
}

algo::~algo()
{
}

void algo::pluginAlgo(ModelPara *mmmm, ComputePara *cccc, visPara *vvvv)
{
	modelParameter = mmmm;
	cptPara = cccc;
	vPara = vvvv;
	bool ok;
	int totalcpu = omp_get_num_procs();

	int core_nums = QInputDialog::getInt(NULL, QStringLiteral("cpu core"), QString("There are ") + QString::number(totalcpu) + " cores in this PC.\n use ", 0, 0, 100, 1, &ok);
	omp_set_num_threads(core_nums);   //指定用于并行计算的线程数目

	if (modelParameter->SiteModels.empty())
	{
		QMessageBox::warning(NULL, QStringLiteral("模型文件"), QStringLiteral("请加载模型文件"));
		return;
	}
	if (cptPara->Sites.size() == 0)
	{
		QMessageBox::warning(NULL, QStringLiteral("发射天线"), QStringLiteral("请添加发射天线文件"));
		return;
	}
	if (modelParameter->materialdatabase.size() == 0)
	{
		QMessageBox::warning(NULL, QStringLiteral("材质文件"), QStringLiteral("请加载材质文件"));
		return;
	}
	map<int, Site_Data *> &AP_EFieldMap = vPara->simuResult;
	AP_EFieldMap.clear();
	auto siteIterator = cptPara->Sites.begin();
	int siteCount = 0;
	m_logText->show();
	m_logText->addText(QStringLiteral("--------------开始计算-----------------"));

	double  beginTime = clock();
	QString fileName= QCoreApplication::applicationDirPath();
	fileName.append("/calculation_log.dat");
	ofstream fout(fileName.toStdString());

	//对每一个站点循环处理
	while (siteIterator != cptPara->Sites.end())
	{
		SphereBeam* SphereTest = new SphereBeam;
		SphereTest = SphereTest->creat(cptPara->RT_sample, cptPara->RT_radius);  //界面交互设置的采样密度即对正20面体细分的次数
		BeamNumber = SphereTest->m_Face.size();; //发射源采样Beam数目


		//保存结果的站点文件
		Site_Data* m_siteData = new Site_Data(siteIterator->first);
		double beginTime = clock();
		m_logText->addText("--------------Site: "+QString::number(siteIterator->first)+"---------------");
		if (modelParameter->mType == ModelType::CITY_LOCAL)
		{
			BaseModel* currentModel = nullptr;
			int currentModelID = siteIterator->first;
			for (size_t i = 0; i < modelParameter->SiteModels.size(); i++)
			{
				if (currentModelID==modelParameter->SiteModels[i]->getModelID())
				{
					currentModel = modelParameter->SiteModels[i];
				}
			}

			if (currentModel==nullptr)
			{
				cout << "no model" << endl;
				return;
			}

			emxModel *trianglePara = new emxModel(currentModel);
			Vector3d AP_position = siteIterator->second->Site_Antennas[0].position;
			double  time1 = clock();
			emxKdTree *AP_KdTree = new emxKdTree(trianglePara, 80, 1, 0.5, 1, -1);
			double  time2 = clock();
			double time_kd_tree = (time2 - time1) / 1000;
			fout << "time_kd_tree:  " << time_kd_tree << endl;
			fout << "begin to find all possible propagation path" << endl;
			m_logText->addText("time_kd_tree :"+QString::number(time_kd_tree)+"s");


			vector<emxBeam*> pRootBeams;
			double time6 = clock();
			CreateInitialBeam(pRootBeams, SphereTest, AP_position);
			fout << "pRootBeams Number: " << pRootBeams.size() << endl;
			m_logText->addText("pRootBeams Number: " + QString::number(pRootBeams.size()));
			double time7 = clock();
			double time_initialBeam = (time7 - time6) / 1000;
			fout << "time_initialBeam:  " << time_initialBeam << endl;
			m_logText->addText("time_initialBeam:  " + QString::number(time_initialBeam));
			double time8 = clock();
			vector< vector<beamNode> >  AP_route;  //route
			int totalpathnum = 0;  //the number of available path

#pragma omp parallel for
			for (int sourceID = 0; sourceID < pRootBeams.size(); sourceID++)
			{
				BeamTracing(AP_KdTree, cptPara->reflectNumPara, pRootBeams[sourceID]);  //跟踪生成编号为sourceID指向的源beam对应的路径树
				find_beamroute(pRootBeams[sourceID], AP_route); //找出toBeProcessedBeams[sourceId]指向的路径树中所有路径，存放到Allroute中

			}
			double time9 = clock();
			double time_Beamtracing = (time9 - time8) / 1000;
			fout << "time_Beamtracing:  " << time_Beamtracing << endl;
			m_logText->addText("time_Beamtracing:" + QString::number(time_Beamtracing));



			totalpathnum = AP_route.size();
			fout << "the total possible path number is " << totalpathnum * 2 << endl;
			m_logText->addText("the total possible path number is:" + QString::number(totalpathnum * 2));

			//找出有效直射、反射、透射、绕射传播路径
			fout << "Begin to find the valid path of receivers in  site  " << siteIterator->second->Site_Name << endl;
			m_logText->addText(QStringLiteral("开始寻找有效直射、反射、透射、绕射传播路径"));

			//接收点坐标设置
			if (cptPara->computeEnum == ComputationEnum::ReceivePoint)  //非仿真面的接收点设置
			{
				bool siteNeedCompute = false;//判断该站点是否要计算

				//根据接收点的pci信息，生成要计算的站点结果，
				//目前只包含位置，信号强度需要下面算法继续计算，并保存到 m_siteData
				for (int j = 0; j < cptPara->No_SimPlanePoint.size(); j++)
				{
					int pci = cptPara->No_SimPlanePoint[j].PCI;

					auto transVec = siteIterator->second->Site_Antennas;
					//检测导入的站点文件是否包含此PCI
					bool isPciInSite = false;
					string cellName = "";
					//遍历计算参数中该站点的天线（cell）
					for (vector<TransAntenna>::iterator it = transVec.begin(); it != transVec.end(); it++)
					{
						if ((*it).PCI == pci)
						{
							isPciInSite = true;
							cellName = (*it).Cell_Name;
							break;
						}
					}

					//如果该站点包含要接收点的所对应的pci
					if (isPciInSite)
					{
						//加入本站点要计算的接收点
						//检测结果文件是否包含此pci
						if (m_siteData->cellsMap.find(pci) == m_siteData->cellsMap.end())
						{
							//如果结果参数中，不包含此pci，插入新的数据
							m_siteData->cellsMap.insert(make_pair(pci, new Cell_Data(pci, cellName)));
						}

						EField *s = new EField;
						s->Position = cptPara->No_SimPlanePoint[j].position;
						m_siteData->cellsMap[pci]->efildVec.push_back(s);
						siteNeedCompute = true;
					}
					else
					{
						//该站点没有要计算的pci
						continue;
					}
				}
				if (siteNeedCompute==false)
				{
					continue;
				}
			}
			else if (cptPara->computeEnum == ComputationEnum::SimuPlane) //按照仿真面设置
			{
				//将该站点所有的pci进行
				auto transVec = siteIterator->second->Site_Antennas;
				for (vector<TransAntenna>::iterator it2 = transVec.begin(); it2 != transVec.end(); it2++)
				{
					Cell_Data * tmpCell = new Cell_Data((*it2).PCI, (*it2).Cell_Name);
					m_siteData->cellsMap.insert(make_pair((*it2).PCI, tmpCell));
				}
				//设置仿真面的点
				SetEFieldPoint(m_siteData, AP_position, currentModel->getSceneRange());
			}
			m_logText->addText(QStringLiteral("接收点设置完毕"));
			//接收点的内外特征判断
			Point_In_Out( m_siteData, currentModel->getLocalScene()->getTotal_Building());
			//接收点设置完毕

		//	emxModel * LocalVirtualSimPlane = new emxModel(currentModel->getGround_Mesh(), cptPara->altitude);
		//	emxKdTree *LocalVirtualSimPlane_KdTree = new emxKdTree(LocalVirtualSimPlane, 80, 1, 0.5, 1, -1);
			double time10 = clock();
			//直射
			valid_DirPath(AP_KdTree, AP_position,m_siteData );
			double time11 = clock();
			double time_DirPath = (time11 - time10) / 1000;
			fout << "time_DirPath:  " << time_DirPath << endl;
			m_logText->addText(QStringLiteral("直射计算完毕"));
			m_logText->addText("time_DirPath:  "+QString::number(time_DirPath));

			for (int i = 0; i < pRootBeams.size(); i++)
			{
				delete pRootBeams[i];
				pRootBeams[i] = NULL;
			}


			//反透射
			valid_RefTransPath(currentModel, AP_KdTree, AP_position, m_siteData, AP_route);
			double time12 = clock();
			double time_RefTransPath = (time12 - time11) / 1000;
			fout << "time_RefTransPath:  " << time_RefTransPath << endl;
			m_logText->addText(QStringLiteral("反射透射计算完毕"));
			m_logText->addText("time_RefTransPath::  " + QString::number(time_RefTransPath));

			//绕射
			/*
			if (cptPara->diffractionNumPara >= 1)
			{
				vector<Vedge> currentEdge = currentModel->getAP_Edge_List();
				//一次绕射
				valid_OnceDiffPath(currentEdge, AP_KdTree, AP_position, m_siteData);
				double time13 = clock();
				double time_once_diffPath = (time13 - time12) / 1000;
				fout << "time_once_diffPath:  " << time_once_diffPath << endl;
			
				m_logText->addText(QStringLiteral("一次绕射计算完毕"));
				m_logText->addText("time_once_diffPath:  " + QString::number(time_once_diffPath));

				//反射加绕射
				valid_RefDiffPath(AP_KdTree, currentEdge, AP_position, m_siteData, AP_route);
				double time16 = clock();
				double time_Ref_LastdiffPath = (time16 - time13) / 1000;
				fout << "time_Ref_LastdiffPath:  " << time_Ref_LastdiffPath << endl;
			
				m_logText->addText("time_Ref_LastdiffPath:  " + QString::number(time_Ref_LastdiffPath));
				if (cptPara->diffractionNumPara>=2)
				{
					vector<int> edgeID = currentModel->getAP_Edge_ID();
					//多次绕射
					valid_NVDiffPath(edgeID, currentEdge, AP_KdTree, AP_position, m_siteData, cptPara->diffractionNumPara);
					double time17 = clock();
					double time_multiple_diffPath = (time17 - time16) / 1000;
					fout << "time_multiple_diffPath:  " << time_multiple_diffPath << endl;
					m_logText->addText(QStringLiteral("多次绕射计算完毕"));
					m_logText->addText("time_multiple_diffPath :  " + QString::number(time_multiple_diffPath));

				}
			}
			*/
			double time18 = clock();
			double time_totalValidPath = (time18 - time10) / 1000;
			fout << "the end of finding valid paths" << endl;
			fout << "time_totalValidPath:  " << time_totalValidPath << endl;
			m_logText->addText(QStringLiteral("路径搜索完成"));
			m_logText->addText("time_totalValidPath :  " + QString::number(time_totalValidPath));

			fout << "Begin to calculate the signal strength of receivers in  site  " << siteIterator->first<< endl;
			m_logText->addText(QStringLiteral("准备计算信号点接收强度"));

			//考虑相位，矢量叠加的信号强度计算
			auto cellIterator = m_siteData->cellsMap.begin();
			while (cellIterator!=m_siteData->cellsMap.end())
			{
				m_logText->addText(QString("cell is ") + QString::fromStdString(cellIterator->second->cell_name));

				//找到当前cptPara中的AP信息
				vector<TransAntenna>& Transss = siteIterator->second->Site_Antennas;
				TransAntenna AP;
				bool findApFlag = false;
				for (int i = 0; i < Transss.size();i++)
				{
					if (Transss[i].PCI==cellIterator->first)
					{
						AP = Transss[i];
						findApFlag = true;
						break;
					}
				}
				if (!findApFlag)
				{
					continue;
				}
				vector<EField*>& EfieldArray_tmpCell = cellIterator->second->efildVec;
				for (int k = 0; k < EfieldArray_tmpCell.size();k++)
				{
					if( (AP.position-EfieldArray_tmpCell[k]->Position).norm()<1e-10)//发射点、接收点重合情况
					{
						double Power1 = AP.trans_power + AP.enlarge_power - AP.wire_loss;
						EfieldArray_tmpCell[k]->MolStrength += pow(10, Power1 / 10);
					}
				}
				double time14 = clock();
				vector<Vedge> currentEdge = currentModel->getAP_Edge_List();
				Antenna_Para *apara = new Antenna_Para;
				Calc_GO_UTD(AP, EfieldArray_tmpCell, currentEdge,apara);
				double time15 = clock();
				double time_totalSignalCalculation = (time15 - time14) / 1000;
				fout << "the end of  signal calculation" << endl;
				fout << "time_totalSignalCalculation:  " << time_totalSignalCalculation << endl;

				m_logText->addText(QStringLiteral("当前站点信号强度计算完成"));
				m_logText->addText("time_totalSignalCalculation: " + QString::number(time_totalSignalCalculation));

				int total_validpathnum = 0;
				for (int id = 0; id < EfieldArray_tmpCell.size(); id++)
				{
					total_validpathnum += EfieldArray_tmpCell[id]->Path.size();
					EfieldArray_tmpCell[id]->pathnum = EfieldArray_tmpCell[id]->Path.size();

					if (EfieldArray_tmpCell[id]->Path.size()!=0)//有路径到达的点
					{
						Vector3cd tmpEfildAll = EfieldArray_tmpCell[id]->EFieldAll;
						EfieldArray_tmpCell[id]->MolStrength = (apara->lamda * apara->lamda / (4 * M_PI * 120 * M_PI))*((tmpEfildAll).x.real()*(tmpEfildAll).x.real() + (tmpEfildAll).x.imag()*(tmpEfildAll).x.imag() + (tmpEfildAll).y.real()*(tmpEfildAll).y.real() + (tmpEfildAll).y.imag()*(tmpEfildAll).y.imag() + (tmpEfildAll).z.real()*(tmpEfildAll).z.real() + (tmpEfildAll).z.imag()*(tmpEfildAll).z.imag());  //场强转成功率
						EfieldArray_tmpCell[id]->MolStrength = 10 * log10(EfieldArray_tmpCell[id]->MolStrength) + 30;  //W转为dBmw
					}
				}
				cellIterator++;
			}
		}
		else if (modelParameter->mType == ModelType::OBJ_LOCAL)
		{
			//暂时不处理obj
		}

		AP_EFieldMap.insert(make_pair(siteIterator->first, m_siteData));

		siteCount++;
		siteIterator++;
	}//

	double endTime = clock();
	double runtime = (endTime - beginTime) / 1000;
	vPara->runTime = runtime;
	fout << "the total  runtime is  " << runtime << endl;
	m_logText->addText(QStringLiteral("所有计算结束！"));
	m_logText->addText("the total  runtime is " + QString::number(runtime));
	fout.close();
}