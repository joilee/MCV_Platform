#include "algo.h"
#include <QMessageBox>


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
void algo::CreateInitialBeam(vector<emxBeam*> &pRootBeams, SphereBeam* SphereTest, Vector3d AP_position, int BeamNumber)  //位于原点处的单位球，细分count次生成初始beams
{
	//多次细分后，存放球上所有生成三角面顶点信息（此时不考虑天线位置，按照位于原点处生成beam）
	for(size_t k = 0; k < BeamNumber; k++)
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

void algo::BeamTracing(emxKdTree* pKdTree, const int &reflectnum, emxBeam* &beam)
{
	int reflectnum1 = reflectnum;

	vector<emxBeam*> ProcessedBeams;
	ProcessedBeams.push_back(beam);
	for(int i_r = 1; i_r <= reflectnum1+1; i_r++) 
	{
		vector<emxBeam*> nextToBeProcessedBeams;
		// collect the reflected beams of hit beams in previous intersection
		for(size_t j = 0; j < ProcessedBeams.size(); ++j) 
		{
			vector<emxBeam*> beamStack, reflectedBeams, transmittedBeams;
			beamStack.push_back(ProcessedBeams[j]);
			while(!beamStack.empty()) 
			{
				emxBeam* pBeam = beamStack.back();
				beamStack.pop_back();
				if(pBeam->IsSplittedBeam()) 
				{
					beamStack.insert(beamStack.end(), pBeam->childBeams.begin(), pBeam->childBeams.end());
				} 
				else if(pBeam->IsHitBeam())
				{
					if(pBeam->BeamArea() > DOUBLE_EPSILON)
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
				for(size_t m = 0; m < reflectedBeams.size(); ++m)
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

	if(pRootBeam != NULL)
	{
		vector<emxBeam*> beamStack;
		beamStack.push_back(pRootBeam);
		while(!beamStack.empty())
		{
			emxBeam* pBeam = beamStack.back();
			beamStack.pop_back();
			if(pBeam->IsSplittedBeam()) 
			{
				beamStack.insert(beamStack.end(), pBeam->childBeams.begin(), pBeam->childBeams.end());
			}
			else if(pBeam->IsHitBeam() && pBeam->reflectedBeam != NULL)
			{
				vector<beamNode> record;  //record记录的是逆向路径
				vector<beamNode> mecord; //通过对record进行逆向操作，进而得到实际的路径，存储在容器mecord
				emxBeam* subNode = NULL ;
				subNode = new emxBeam(*pBeam->reflectedBeam);
				while(subNode !=NULL)
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
						for (int i=0;i<NodeNow.cornerNum;i++)
						{
							NodeNow.BeamVertex[i] = subNode->BeamVertex[i];
						}
						record.push_back(NodeNow);
					}			
					subNode = subNode->parentBeam;	
				}
				if(subNode)
				{
					delete subNode;
					subNode = NULL;
				}
				for(int i = record.size()-1; i >= 0; i--)
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
void algo::SetEFieldPoint(Site_Data* m_siteData, Vector3d AP_position, double LocalScene_range, double Zheight, double Precetion, Scene_para &sp, ModelPara *mP)
{
	m_siteData->cellsMap.clear();

	double Left_Up_X = AP_position.x - LocalScene_range/2;
	double Left_Up_Y = AP_position.y + LocalScene_range/2;
	double Right_Down_X = AP_position.x + LocalScene_range/2;
	double Right_Down_Y = AP_position.y - LocalScene_range/2;
	
	//定义测试的点数，横向m等分，纵向n等分
	
	if(Precetion <= 0.0)
	{
		QMessageBox::warning(NULL, QStringLiteral("接收处预测点分布"), QStringLiteral("请输入合适的分布精度"));
		return;
	}
	int width = (int)((Left_Up_Y - Right_Down_Y)/Precetion);//row
	int length = (int)((Right_Down_X - Left_Up_X)/Precetion);//col
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
	sp.scene_length = m;
	sp.scene_width = n;
	double Xmin = Left_Up_X;
	double Ymin = Right_Down_Y;
	//double Xstep,Ystep;
	if (m == 0)
	{
		sp.Xstep = 0;
	}
	else
	{
		sp.Xstep = (Right_Down_X - Left_Up_X)/m;
	}
	if (n == 0)
	{
		sp.Ystep = 0;
	}
	else
	{
		sp.Ystep = (Left_Up_Y - Right_Down_Y)/n;
	}
	BaseModel *tmpModel;
	bool find=false;
	for (int i = 0; i < mP->SiteModels.size();i++)
	{
		if (mP->SiteModels[i]->getModelID()==m_siteData->siteID)
		{
			tmpModel = mP->SiteModels[i];
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
				double x = Xmin + sp.Xstep*(i / (n + 1));
				double y = Ymin + sp.Ystep*(i % (n + 1));
				double z = Zheight + tmpModel->getPointAltitude(x, y);
				s->Position = Vector3d(x, y, z);
				it->second->efildVec.push_back(s);
			}
			//补全cell的信息
			it->second->row = width + 1;//width是栅格数，row是点的行数
			it->second->col = length + 1;
			it->second->pricision = Precetion;
		}
	}


}





void algo::Point_In_Out(ComputationEnum ce, Site_Data *m_siteData, vector< Building > &Local_buildings, double Zheight, Scene_para &sp)
{
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
		EField* tmpE = *(cell->efildVec.begin());
		double Xmin = tmpE->Position.x;
		double Ymin = tmpE->Position.y;

		int start_rowId, end_rowId, start_columnId, end_columnId;

		for (int j = 0; j < Local_buildings.size(); j++)
		{
			vector<Vector3d> upperFacePoint = Local_buildings[j].upper_facePoint;
			double building_height = upperFacePoint[0].z;  
			EfieldPointInPolygon(upperFacePoint, Xmin, Ymin, start_rowId, end_rowId, start_columnId, end_columnId,sp);
			if (start_rowId*(start_rowId - sp.scene_width) <= 0 && end_rowId*(end_rowId - sp.scene_width) <= 0 && start_columnId*(start_columnId - sp.scene_length) <= 0 && end_columnId*(end_columnId - sp.scene_length) <= 0)
			{
				//如果建筑物的包围盒四条边都在仿真面的内部
				for (int columnId = start_columnId; columnId <= end_columnId; columnId++)
				{
					for (int rowId = start_rowId; rowId <= end_rowId; rowId++)
					{
						//对site里的所有cell进行筛选操作
						EField* tmpLocationE = cell->efildVec[columnId*(sp.scene_width + 1) + rowId];
						if (tmpLocationE->Position.z>building_height)
						{
							continue;
						}
						else if (contain(upperFacePoint,tmpLocationE->Position))
						{
							//对site里的所有cell进行标识，标识当前位置的接收点在建筑物内部
							auto deleteIte = m_siteData->cellsMap.begin();
							for (; deleteIte != m_siteData->cellsMap.end();deleteIte++)
							{
								deleteIte->second->efildVec[columnId*(sp.scene_width + 1) + rowId]->In_or_Out = 0;
							}
							//标识完毕
						}

					}
				}
			}//如果建筑物不在仿真面内部，跳过
		}//结束对所有建筑的循环
	}

}

void algo::EfieldPointInPolygon(vector<Vector3d>& polygon, double Xmin, double Ymin, int &start_rowId, int &end_rowId, int &start_columnId, int &end_columnId, Scene_para sp)
{
	double start_x = polygon[0].x,start_y = polygon[0].y, end_x = polygon[0].x,end_y = polygon[0].y;
	for (int i = 1; i < polygon.size(); i++)
	{
		start_x = min(start_x,polygon[i].x);
		start_y = min(start_y,polygon[i].y);
		end_x = max(end_x,polygon[i].x);
		end_y = max(end_y,polygon[i].y);
	}
	start_columnId = ceil((start_x - Xmin)/sp.Xstep);//找到建筑物相对于仿真面的行列值。
	end_columnId = floor((end_x-Xmin)/sp.Xstep);
	start_rowId = ceil((start_y-Ymin)/sp.Ystep);
	end_rowId = floor((end_y-Ymin)/sp.Ystep);

	if (start_rowId < 0)
	{
		start_rowId = 0;
	}
	if (start_columnId < 0)
	{
		start_columnId = 0;
	}
	if (end_rowId > sp.scene_width)
	{
		end_rowId = sp.scene_width;
	}
	if (end_columnId > sp.scene_length)
	{
		end_columnId = sp.scene_length;
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
void algo::valid_DirPath(emxKdTree* pKdTree, Vector3d AP_position, Cell_Data *m_cellData)
{
	//先判断有没有直射电磁波到达
	for (int i=0; i<m_cellData->efildVec.size(); i ++)
	{
		if (!m_cellData->efildVec[i]->In_or_Out)  //对于位于建筑物内部的接收点不用考察是否发生直射
			continue;
		Vector3d direction = m_cellData->efildVec[i]->Position - AP_position;
		double length = direction.norm();
		emxRay directRay;
		int dirFaceid = -1;
		double dirHit = 0;
		directRay.direction = direction.normalize();
		directRay.origin = AP_position;
		directRay.lastID = -1;
		if(length > 1e-6&&(!pKdTree->Intersect(directRay, dirHit, dirFaceid) || dirHit>=0.9999*length))
		{
			Field_Path ipath;
			ipath.Path_interPoint.push_back(AP_position);
			ipath.Path_interPoint.push_back(m_cellData->efildVec[i]->Position);

			ipath.all_distance = length;
			m_cellData->efildVec[i]->Path.push_back(ipath);
		}
	}
}


//计算反射、透射、绕射路径到达接收点的信号强度
void  algo::Calc_GO_UTD(TransAntenna &AP, vector<EField>  &EFieldArray, vector<Vedge> &Edge_list, Antenna_Para & A_Para, ComputePara * c_para, ModelPara *mp)
{
	A_Para.frequency = AP.frequency * 1e6;
	A_Para.light_speed = 299792458;
	A_Para.epsilon0 =  1e-9/(36*M_PI);
	A_Para.lamda = A_Para.light_speed/A_Para.frequency;
	A_Para.k = 2 * M_PI * A_Para.frequency /A_Para.light_speed; 
	A_Para.w = 2 * M_PI * A_Para.frequency;

	double initPowerStrength = pow(10,(AP.trans_power + AP.enlarge_power - AP.wire_loss)/10); //发射源初始功率，由dBm转换为mW
	A_Para.EveryRayPowerStrength = initPowerStrength*1e-3; //每条源发射射线携带的功率，由mW转为W
	//获取全局坐标系中的正北方向在场景坐标系下的方位角phi,3D天线方向增益phi是相对于正北方向的夹角，因此当下坐标系统获取的phi需转换到正北方向的夹角
	A_Para.NorthAngle = c_para->phi;


#pragma omp parallel for
	for (int i=0; i<EFieldArray.size(); i++)
	{
		EField &NField = EFieldArray[i];

		NField.HorizontalDis = sqrt((NField.Position.x - AP.position.x)*(NField.Position.x - AP.position.x) + (NField.Position.y - AP.position.y)*(NField.Position.y - AP.position.y));   //发射点和接收点之间的水平距离

		//NField.LosDis = (NField.Position - AP.position).norm();

		//剔除重复路径
		for (int id1= 0; id1<NField.Path.size(); id1++) 
		{
			Field_Path path1 = NField.Path[id1];
			if (path1 .Path_interPoint.size() >= 3 && path1.propagation_type[1] != 2)  //此条路径为反、透射路径
			{			
				for (int id2 = id1+1;id2<NField.Path.size();id2++)
				{
					Field_Path path2 = NField.Path[id2];
					if (path2.intersect_ID.size() == path1.intersect_ID.size() && path2.propagation_type[1] != 2)
					{
						bool same_Path = true;
						for (int id3 = 0; id3 < path2.intersect_ID.size(); id3++)
						{
							if (path1.intersect_ID[id3] == path2.intersect_ID[id3] && path1.propagation_type[id3] == path2.propagation_type[id3])
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
							NField.Path.erase(NField.Path.begin() + id2);
							id2--;
						}
					}
				}
			}			
		}
		for (int j = 0; j<NField.Path.size(); j++)
		{
			if (NField.Path[j].Path_interPoint.size() == 2)  //此条路径为直射（源点、接收点组成的路径）
			{
				Calc_DirPathSignal(NField,AP,j,A_Para);
			}
			//或者考虑绕射
			else  if (c_para->isDiffractionPara ==true&& NField.Path[j].Path_interPoint.size() == 3 && NField.Path[j].propagation_type[1] == 2)    //绕射路径
			{
				Calc_diffSignal1(Edge_list,NField,AP,j,A_Para,mp);
				//Calc_difftest(Edge_list,NField,AP,j);
			}
			else   //反、透射路径
			{
				Calc_RefTransSignal(NField,AP,j,A_Para,mp,c_para);
			}		
		}
	}
}

void algo::Calc_DirPathSignal(EField &NField, TransAntenna &AP, int &path_id, Antenna_Para aPara)
{
	Vector3d direction = NField.Position - AP.position;
	double length = direction.norm();
	//Tpolor是初始场强方向
	Vector3d Tpolor = AP.polor_direction.normalize() - Dot(AP.polor_direction.normalize(),direction.normalize())*(direction.normalize());  //此处的direction是不是需要normalize？？

	if(Tpolor.norm() < 1e-10)
	{
		//此条路径无效，需剔除
		NField.Path.erase(NField.Path.begin()+path_id);
		path_id--;
	}
	else
	{
		Tpolor = Tpolor.normalize();

		//原始射线方向向量，theta角不变，phi需变换为相对于正北方向的夹角
		int Hnum = int(asin(fabs(direction.y)/sqrt(direction.x*direction.x + direction.y*direction.y))*180/M_PI+0.5);  //射线的水平方向角phi
		if(direction.x >= 0 && direction.y >= 0)
		{
			Hnum = aPara.NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
		}
		else if(direction.x < 0 && direction.y >= 0)
		{
			Hnum = 180 - Hnum;
			Hnum = aPara.NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
		}
		else if(direction.x <= 0 && direction.y <= 0)
		{
			Hnum = 180 + Hnum;
			Hnum = aPara.NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
		}
		else
		{
			Hnum = 360 - Hnum;
			Hnum = aPara.NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
			if(Hnum == 360)
				Hnum = 0;
		}
		int Vnum = int(acos(fabs(direction.z)/(direction.norm()))*180/M_PI + 0.5); //射线的垂直方向角theta
		if(direction.z < 0)
		{
			Vnum = 180 - Vnum;
		}
		aPara.TP_gain = AP.initial_Gain - AP.direction_Gain[Hnum*181 + Vnum][2];

		double EveryRayEFieldStrength = sqrt(30 *aPara. EveryRayPowerStrength * pow(10,aPara.TP_gain/10))/length ;  //射线携带的功率转换为射线场强值大小
		Vector3d tfield =  Tpolor * EveryRayEFieldStrength ;  //第一项为射线初始场强方向，第二项为射线初始场强大小
		Vector3cd ctfield = Vector3cd(tfield.x,tfield.y,tfield.z);

		ctfield = ctfield *  exp(complex<double>(0,-aPara.k*length));  //考虑相位影响
		NField.EFieldAll += ctfield;     //电场叠加
		//NField.MolStrength +=  (lamda * lamda / (4 * M_PI* 120 * M_PI))*( ctfield.x.real()*ctfield.x.real() + ctfield.x.imag()*ctfield.x.imag()  + ctfield.y.real()*ctfield.y.real() + ctfield.y.imag()*ctfield.y.imag() + ctfield.z.real()*ctfield.z.real() +ctfield.z.imag()*ctfield.z.imag() );  //场强转成功率

		double path_RecPower = aPara.lamda * aPara.lamda * (ctfield.x.real()*ctfield.x.real() + ctfield.x.imag()*ctfield.x.imag() + ctfield.y.real()*ctfield.y.real() + ctfield.y.imag()*ctfield.y.imag() + ctfield.z.real()*ctfield.z.real() + ctfield.z.imag()*ctfield.z.imag()) / (4*M_PI * 120 * M_PI);
		NField.Path[path_id].power_Loss = 10*log10( aPara.EveryRayPowerStrength  / path_RecPower);
	}	
}

//计算绕射路径信号强度
void algo::Calc_diffSignal1(vector<Vedge> &Edge_list, EField &NField, TransAntenna &AP, int &path_id, Antenna_Para aP, ModelPara *mp)
{
	Vedge the_edge = Edge_list[NField.Path[path_id].edge_id];

	Vector3d vInit = the_edge.start;
	Vector3d vEnd = the_edge.end;
	Vector3d lineDirection = vEnd - vInit;
	Vector3d line_unitDirection = lineDirection.normalize();

	Vector3d source_pos = AP.position;
	Vector3d Diffract_Pos = NField.Path[path_id].Path_interPoint[1];
	Vector3d field_pos = NField.Position;

	Vector3d direction = (Diffract_Pos-source_pos).normalize();
	Vector3d polor_direction = AP.polor_direction.normalize() - Dot(AP.polor_direction.normalize(),direction.normalize())*(direction.normalize()); 	

	if(polor_direction.norm() < 1e-10)
	{
		//此条路径无效，需剔除
		NField.Path.erase(NField.Path.begin()+path_id);
		path_id--;
	}
	else
	{
		polor_direction = polor_direction.normalize();
		int Hnum = int(asin(fabs(direction.y)/sqrt(direction.x*direction.x + direction.y*direction.y))*180/M_PI+0.5);
		if(direction.x >= 0 && direction.y >= 0)
		{}
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
		int Vnum = int(acos((direction.z)/(direction.norm()))*180/M_PI + 0.5);
		if(direction.z < 0)
		{
			Vnum = 180 - Vnum;
		}

		Hnum = aP.NorthAngle - Hnum;
		if (Hnum<0)
		{
			Hnum+=360;
		}				
		double AP_gain = AP.initial_Gain - AP.direction_Gain[Hnum*181 + Vnum][2];

		Vector3d inVec = Diffract_Pos-source_pos;
		Vector3d diffVec = field_pos-Diffract_Pos;
		double lengthSD = inVec.norm();
		double lengthDF = diffVec.norm();
		inVec = inVec.normalize();
		diffVec = diffVec.normalize();
		getDiffEdgeINfor(the_edge, source_pos ,Diffract_Pos);
		Vector3d edgeDir = (the_edge.end - the_edge.start).normalize();
		Vector3d normal = the_edge.normal_front;

		Vector3d xDir;
		xDir = VectorCross(normal, edgeDir);

		Vector3d projInVec =-inVec - edgeDir * Dot(-inVec,edgeDir); 
		projInVec = projInVec.normalize();
		double phiS = acos(max(-1.0,min(1.0,Dot(projInVec,xDir))));  //入射平面和楔形正面（前面）的夹角

		Vector3d projdiffVec = (diffVec - edgeDir * Dot(diffVec, edgeDir));
		projdiffVec = projdiffVec.normalize();
		double phi = acos(max(-1.0, min(1.0, Dot(projdiffVec, xDir))));  //绕射平面和楔形正面（前面）的夹角

		if(Dot(diffVec, normal) < 0)
			phi = 2.0 * M_PI - phi;

		double EveryRayEFieldStrength = sqrt(30*aP.EveryRayPowerStrength* pow(10,AP_gain/10))/lengthSD;  //射线携带的功率转换为射线场强值大小
		polor_direction =  polor_direction * EveryRayEFieldStrength;  //发射天线的方向增益,第二项为射线初始场强方向，第三项为射线初始场强大小

		Vector3cd in_field = Vector3cd(polor_direction.x,polor_direction.y,polor_direction.z);

		double FaceInteriorAngle = M_PI - acos(max(-1.0,min(1.0,Dot(the_edge.normal_front,the_edge.normal_back))));
		double n = 2 - FaceInteriorAngle/M_PI;//绕射尖劈，最接近入射面（即定义的正面或前面）的劈面为0面，另一面为n面，劈内角为 FaceInteriorAngle = (2-n)*PI
		double L =((lengthSD * lengthDF) / (lengthSD + lengthDF));

		//扩散因子
		double factor = sqrt(lengthSD/(lengthDF*(lengthSD + lengthDF)));

		complex<double> epsilon = getEpsilon(aP.frequency, the_edge.materialId,mp);

		complex<double> prevD = -exp(complex<double>(0,-M_PI/4.0)) /(2 * n * sqrt(2 * M_PI* aP.k));

		double gama[4] = {
			(M_PI - (phi - phiS)) / (2 * n), (M_PI + (phi - phiS)) / (2 * n), 
			(M_PI - (phi + phiS)) / (2 * n), (M_PI + (phi + phiS)) / (2 * n)
		};
		int N[4] = {
			roundInt((-M_PI +(phi - phiS))/(2*n*M_PI)) , roundInt((M_PI +(phi - phiS))/(2*n*M_PI)),
			roundInt((-M_PI +(phi + phiS))/(2*n*M_PI)) ,roundInt((M_PI +(phi + phiS))/(2*n*M_PI)) 
		};
		double a[4]  = {
			2*cos((2*n*M_PI*N[0] - (phi - phiS))/2) *cos((2*n*M_PI*N[0] - (phi - phiS))/2), 
			2*cos((2*n*M_PI*N[1] - (phi - phiS))/2) *cos((2*n*M_PI*N[1] - (phi - phiS))/2), 
			2*cos((2*n*M_PI*N[2] - (phi + phiS))/2) *cos((2*n*M_PI*N[2] - (phi + phiS))/2), 
			2*cos((2*n*M_PI*N[3] - (phi + phiS))/2) *cos((2*n*M_PI*N[3] - (phi + phiS))/2), 
		};

		double epsilonp[4] = {
			M_PI - ((phi - phiS) - 2*n*M_PI*N[0]) + 1e-7 , M_PI + ((phi - phiS) - 2*n*M_PI*N[1]) ,
			M_PI - ((phi + phiS) - 2*n*M_PI*N[2]) +1e-7 , M_PI + ((phi + phiS) - 2*n*M_PI*N[3]) 
		};
		complex<double> D[4];

		for(int j = 0; j < 4; ++j) 
		{
			if(fabs(gama[j]) < DOUBLE_EPSILON) //当gama[j]趋于0时，cot(gama[j])趋于无穷，但与此同时，其相应的过渡函数F趋于零，从而消除了奇异性。阴影边界（phi-phiS = M_PI）和反射边界处(phi+phiS = M_PI)
			{
				complex<double> fresnel = n *(sqrt(2*M_PI*aP.k*L)*sign_func(epsilonp[j]) - 2*aP.k*L*epsilonp[j]*exp(complex<double>(0,M_PI/4))) * exp(complex<double>(0,M_PI/4));

				D[j] = prevD * fresnel;
			}
			else 
				D[j] = prevD * cot(gama[j]) * Fresnel(aP.k*L*a[j]);
		}

		complex<double> R0v(-1),R0p(1),Rnv(-1),Rnp(1);   //if perfectly conducting  R0和Rn分别是入射面和绕射面的反射因子

		//////////////////////////////////////////////////////////////////////////
		double theta0 = phiS;
		double thetan = n*M_PI - phi;
		//if not perfectly conducting  有耗介质
		if(1)
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
		Vector3d phipv ;  //入射垂直极化波方向，TE
		Vector3d betapv ; //入射平行极化波方向，TM 
		Vector3d phiv   ; //绕射垂直极化波方向，TE
		Vector3d betav  ; //绕射平行极化波方向，TM 
		phipv = (VectorCross(inVec, edgeDir)).normalize();
		betapv= (VectorCross( phipv, inVec)).normalize();
		phiv = (VectorCross( edgeDir, diffVec)).normalize();
		betav= (VectorCross(phiv, diffVec)).normalize();

		Vector3d e_v0 = VectorCross(inVec,the_edge.normal_front).normalize();
		Vector3d e_p0  = VectorCross(e_v0,inVec).normalize();
		Vector3d e_vn = VectorCross(diffVec,the_edge.normal_back).normalize();
		Vector3d e_pn  = VectorCross(e_vn,diffVec).normalize();

		double sin_alpha1 = max(-1.0,min(1.0,Dot(e_p0,phipv)));
		double sin_alpha2 = max(-1.0,min(1.0,Dot(e_pn,phipv)));;   //????????
		double cos_alpha1 = sqrt(1 - sin_alpha1*sin_alpha1);   //????????
		double cos_alpha2 = sqrt(1- sin_alpha2*sin_alpha2);   //????????
		complex<double> Dss = D[0] + D[1] - (R0p*cos_alpha1*cos_alpha1 - R0v*sin_alpha1*sin_alpha1)*D[2] - (Rnp*cos_alpha2*cos_alpha2 - Rnv*sin_alpha2*sin_alpha2)*D[3];
		complex<double> Dhh = D[0] + D[1] +(R0p*sin_alpha1*sin_alpha1 - R0v*cos_alpha1*cos_alpha1)*D[2] + (Rnp*sin_alpha2*sin_alpha2 - Rnv*cos_alpha2*cos_alpha2)*D[3];
		complex<double> Dsh = -(R0p+R0v)*cos_alpha1*sin_alpha1*D[2] - (Rnp + Rnv)*cos_alpha2*sin_alpha2*D[3];
		complex<double> Dhs = (R0p+R0v)*cos_alpha1*sin_alpha1*D[2] + (Rnp + Rnv)*cos_alpha2*sin_alpha2*D[3];

		complex<double> Ei_betap = Dot(in_field, betapv); //入射场的平行极化分量
		complex<double> Ei_phip  = Dot(in_field,  phipv); //入射场的垂直极化分量
		complex<double> Ed_beta  = (Dss * Ei_betap + Dsh * Ei_phip) * factor; //绕射场的平行极化分量
		complex<double> Ed_phi   = (Dhs * Ei_betap + Dhh * Ei_phip) * factor; //绕射场的垂直极化分量

		Vector3cd diffField = Ed_beta*betav + Ed_phi*phiv;

		diffField = diffField * exp(complex<double>(0,-(aP.k*(lengthSD + lengthDF))));
		NField.EFieldAll += diffField;			
		//NField.MolStrength +=  (lamda * lamda / (4 *M_PI* 120 * M_PI))*( diffField.x.real()*diffField.x.real() + diffField.x.imag()*diffField.x.imag()  + diffField.y.real()*diffField.y.real() + diffField.y.imag()*diffField.y.imag() + diffField.z.real()*diffField.z.real() +diffField.z.imag()*diffField.z.imag() );  //场强转成功率

		NField.Path[path_id].all_distance = lengthSD + lengthDF;
		double path_RecPower =aP.lamda * aP.lamda * (diffField.x.real()*diffField.x.real() + diffField.x.imag()*diffField.x.imag() + diffField.y.real()*diffField.y.real() + diffField.y.imag()*diffField.y.imag() + diffField.z.real()*diffField.z.real() + diffField.z.imag()*diffField.z.imag()) / (4*M_PI * 120 * M_PI);
		NField.Path[path_id].power_Loss = 10*log10( aP.EveryRayPowerStrength/ path_RecPower);
	}
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

complex<double>algo::getEpsilon(double freq, int materialId, ModelPara * mp)   //相对介电常数        /*单位:HZ*/
{
	double w = 2 * M_PI * freq ;
	int index = getMaterial(freq,materialId,mp);
	double dielectric = mp->materialdatabase[index].dielectric;
	double conductivity = mp->materialdatabase[index].conductivity;
	return std::complex<double>(dielectric, 
		-conductivity / (w * 1e-9 / (36 * M_PI)));  //复介电常数

}

int algo::getMaterial(double freq /*单位:HZ*/, int materialId, ModelPara * mp)
{
	double dif = DBL_MAX;
	int MatVector_index;
	for (int i=0;i<mp->materialdatabase.size();i++)
	{
		if (materialId == mp->materialdatabase[i].Id )
		{
			if (fabs(freq - (mp->materialdatabase[i].frequency/*单位:MHZ*/) * 1e6) < dif)
			{
				dif = fabs(freq - (mp->materialdatabase[i].frequency) * 1e6);
				MatVector_index = i;
			}
		}
	}
	return MatVector_index;
}




//计算反透射路径信号强度
void algo::Calc_RefTransSignal(EField &NField, TransAntenna &AP, int &path_id, Antenna_Para ap, ModelPara *mp, ComputePara * cPara)
{
	//路径判断有效后，追踪此条路径计算场强
	vector<Vector3d> &path_point = NField.Path[path_id].Path_interPoint;
	Vector3d Raydirection = (path_point[1]-path_point[0]).normalize();
	double raydistance = (path_point[1]-path_point[0]).norm();  //raydistance 记录整条传播路径的长度
	//Tpolor是发射天线的极化方向
	Vector3d Tpolor = AP.polor_direction.normalize() - Dot(AP.polor_direction.normalize(),Raydirection)*(Raydirection); 

	if(Tpolor.norm() < 1e-10)
	{
		//此条路径无效，需剔除
		NField.Path.erase(NField.Path.begin()+path_id);
		path_id--;
	}
	else
	{
		Tpolor = Tpolor.normalize();

		int Hnum = int(asin(fabs(Raydirection.y)/sqrt(Raydirection.x*Raydirection.x + Raydirection.y*Raydirection.y))*180/M_PI+0.5);  //射线的水平方向角phi
		if(Raydirection.x >= 0 && Raydirection.y >= 0)
		{
			Hnum =ap. NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
		}
		else if(Raydirection.x < 0 && Raydirection.y >= 0)
		{
			Hnum = 180 - Hnum;
			Hnum = ap.NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
		}
		else if(Raydirection.x <= 0 && Raydirection.y <= 0)
		{
			Hnum = 180 + Hnum;
			Hnum = ap.NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
		}
		else
		{
			Hnum = 360 - Hnum;
			Hnum = ap.NorthAngle  - Hnum;
			if (Hnum<0)
			{
				Hnum = Hnum + 360;
			}
			if(Hnum == 360)
				Hnum = 0;
		}
		int Vnum = int(acos(fabs(Raydirection.z)/Raydirection.norm())*180/M_PI + 0.5); //射线的垂直方向角theta，与球坐标系中theta不一致？？？？？？  改了
		if(Raydirection.z < 0)
		{
			Vnum = 180 - Vnum;
		}

		ap.TP_gain = AP.initial_Gain - AP.direction_Gain[Hnum*181 + Vnum][2];

		double EveryRayEFieldStrength = sqrt(30 * ap.EveryRayPowerStrength * pow(10,ap.TP_gain/10))/raydistance;  //射线携带的功率转换为射线场强值大小
		Vector3d tfield = Tpolor *  EveryRayEFieldStrength;
		Vector3cd ctfield(tfield.x,tfield.y,tfield.z); //入射射线在第一个相交面相交点处的入射末场强					

		Vector3d Into, next_Into, Normal; //Into入射方向向量，Normal相交面处法向量
		double currentTOnext_distance; //此次相交点到下次场点（相交点）之间的距离
		//传播过程中电场经过多次反射、折射，最终到达接收点处方向
		for(int l = 1; l < path_point.size()-1; l++)
		{
			double d_factor; //扩散因子，与距离有关
			//int materialId = pMesh->getMtlId(rayRoute[l].faceId); //获取面片对应的材质编号
			int materialId = 11; //获取面片对应的材质编号
			complex<double> Epsilon = getEpsilon(ap.frequency, materialId,mp); //折射处介质的相对介电常数，此处为复相对介电常数
			//complex<double> Epsilon = complex<double>(6.0,  -1.0/(w * 1e-9 / (36 * M_PI)));  //复相对介电常数
			Into = (path_point[l]-path_point[l-1]).normalize();
			next_Into = (path_point[l+1]-path_point[l]).normalize();
			Normal = NField.Path[path_id].intersect_faceNormal[l]; //反射、透射beam的normal
			double costheta = fabs(Dot(Into,Normal));  //入射角的cos值
			double square_sintheta = 1 - costheta*costheta;

			currentTOnext_distance = (path_point[l+1] - path_point[l]).norm(); //当前相交面处点到下次场点（相交点）之间的距离
			//当扩散因子中的s'理解为从源点到当前透射点整个传播路径的长度时，扩散因子的计算表达式
			d_factor = raydistance/(raydistance + currentTOnext_distance);

			//透射时
			if(NField.Path[path_id].propagation_type[l] == 1) 
			{

				//	cout<<"transmision"<<endl;
				if (cPara->transIndex == 0) //使用经验透射损耗值
				{
					//	使用经验透射损耗值 透射损耗要可设置，一般是有两个参数：透射损耗，表示信号穿透墙体后的衰减，比如：城市建筑物设置15dB的透射损耗；
					//	另一个参数，透射递进损耗，表示室外信号穿透到室内后，不清楚请室内布局的情况下，随着信号覆盖深入，信号强度随距离继续衰减，比如：一般设置为0.5dB/m
					double wall_loss = 10;
					double dis_loss = 0.5;
					double tf_distance = currentTOnext_distance;
					raydistance += tf_distance;
					double total_loss = wall_loss + dis_loss * tf_distance;  //从透射点到达透射场点之间的损耗：透射损耗（穿透墙体损耗）+ 透射递进损耗（进入室内随距离损耗）
					ctfield = ctfield  * complex<double>((1 / pow(10,  total_loss/10)), 0);
				}
				else if (cPara->transIndex  == 1)  //使用理论公式计算
				{
					complex<double> EV,EP;//垂直极化衰减系数、平行极化衰减系数
					complex<double> VD1,VD2,PD1,PD2;//垂直极化方向折射前后的分量，平行极化折射前后的分量
					Vector3d TV1,TP1,TV2,TP2; //透射前后的垂直极化方向，平行极化方向

					EV = (costheta * 2)/(costheta + sqrt(Epsilon-square_sintheta));   //垂直极化透射系数
					EP = (2*costheta*sqrt(Epsilon))/(Epsilon*costheta + sqrt(Epsilon-square_sintheta));   //水平极化波透射系数

					//	求透射后的垂直极化分量，平行极化分量
					if(costheta > 0.9999999999999)
					{
						//	垂直入射时，透射波场强
						ctfield = ctfield*(complex<double>(2,0)/(complex<double>(1,0)+sqrt(Epsilon)));
						raydistance += currentTOnext_distance;
					}
					else
					{
						TV1 = VectorCross(Into,Normal).normalize();
						TP1 = VectorCross(TV1,Into).normalize();
						//透射后垂直、水平极化方向与透射前一致
						TV2 = TV1;
						TP2 = TP1;

						//折射后垂直极化分量VD2大小改变
						VD1 = Dot(ctfield,TV1); //透射前垂直极化分量数值
						VD2 = VD1*EV*d_factor;

						//折射前后平行极化分量PD2大小改变
						PD1 = Dot(ctfield,TP1); //透射前平行极化分量数值
						PD2 = PD1*EP*d_factor;

						//折射后电场方向（一般为非单位向量，经过系数衰减）
						ctfield = TV2*VD2 + TP2*PD2;

						raydistance += currentTOnext_distance;
					}
				}
			}

			//反射时
			else if (NField.Path[path_id].propagation_type[l] == 0)
			{
				complex<double> EV,EP;//反射垂直极化系数、平行极化系数
				complex<double> VD1,VD2,PD1,PD2;//垂直极化方向反射前后的分量，平行极化反射前后的分量
				Vector3d RV1,RP1,RV2,RP2; //反射前后的垂直极化方向，平行极化方向

				EV = (costheta - sqrt(Epsilon-square_sintheta))/(costheta + sqrt(Epsilon-square_sintheta));
				EP = (Epsilon*costheta - sqrt(Epsilon-square_sintheta))/(Epsilon*costheta + sqrt(Epsilon-square_sintheta));

				//求反射后的垂直极化分量，平行极化分量
				if(costheta > 0.9999999999999)
				{
					//垂直入射时，反射波场强
					ctfield = ctfield*((complex<double>(1,0)-sqrt(Epsilon))/(complex<double>(1,0)+sqrt(Epsilon)));
					raydistance += currentTOnext_distance;
				}
				else
				{
					RV1 = VectorCross(Into,Normal).normalize();
					RP1 = VectorCross(RV1,Into).normalize();
					RV2 = RV1;
					RP2 = VectorCross(RV2,next_Into).normalize();


					//反射后垂直极化分量VD2数值大小改变体现在系数里面
					VD1 = Dot(ctfield,RV1);
					VD2 = VD1*EV*d_factor;
					//反射后平行极化分量PD2数值大小改变体现在系数里面
					PD1 = Dot(ctfield,RP1);
					PD2 = PD1*EP*d_factor;

					//反射后电场方向（一般为非单位向量，经过系数衰减）
					ctfield = RV2*VD2 + RP2*PD2; 

					raydistance += currentTOnext_distance;
				}
			}
		}
		double phase =ap.k*raydistance;  //记录整条传播路径的累计相位

		ctfield = ctfield*exp(complex<double>(0,-phase));   //考虑整条传播路径累计相位影响
		NField.EFieldAll+= ctfield;
		//NField.MolStrength +=  (lamda * lamda / (4 * M_PI * 120 * M_PI))*( ctfield.x.real()*ctfield.x.real() + ctfield.x.imag()*ctfield.x.imag()  + ctfield.y.real()*ctfield.y.real() + ctfield.y.imag()*ctfield.y.imag() + ctfield.z.real()*ctfield.z.real() +ctfield.z.imag()*ctfield.z.imag() );  //场强转成功率

		NField.Path[path_id].all_distance = raydistance;
		double path_RecPower =ap.lamda * ap.lamda * (ctfield.x.real()*ctfield.x.real() + ctfield.x.imag()*ctfield.x.imag() + ctfield.y.real()*ctfield.y.real() + ctfield.y.imag()*ctfield.y.imag() + ctfield.z.real()*ctfield.z.real() + ctfield.z.imag()*ctfield.z.imag()) / (4*M_PI * 120 * M_PI);
		NField.Path[path_id].power_Loss = 10*log10( ap.EveryRayPowerStrength  / path_RecPower);
	}
}

//找出有效的反透射路径，直接由beam与虚拟仿真面的覆盖区域获取接收点信息，未考虑海拔时
void algo::valid_RefTransPath(ComputationEnum ce, Scene_para &sp, emxKdTree* pKdTree, Vector3d AP_position, Cell_Data* m_cellData, const vector<vector<beamNode>> &beamRoutes)
{

		double Xmin =m_cellData->efildVec[0]->Position.x;
		double Ymin = m_cellData->efildVec[0]->Position.y;
		double Zheight = m_cellData->efildVec[0]->Position.z;
		Vector3d Point_IN_SimPlane(Xmin, Ymin, Zheight);
		Vector3d SimPlane_Normal(0, 0, 1);
		int num_RefTransPath = 0;

#pragma omp parallel for schedule(static,4)
		for (int beam_id = 0; beam_id < beamRoutes.size(); beam_id++)  //容器beamRoutes存放的所有可能路径
		{
			vector<beamNode> beamroute = beamRoutes[beam_id];
			int FinalBeamNum = beamroute.size() - 1;
			beamNode FinalBeam = beamroute[FinalBeamNum];

			int start_rowId, end_rowId, start_columnId, end_columnId;    //beam与仿真面区域相交覆盖的接收点下标
			bool illuminate = false;
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
					origin = FinalBeam.origin;
				}
				else
				{
					//透射beam源点
					Vector3d TransBeamOrigin = FinalBeam.origin - FinalBeam.beamNormal * (2 * Dot(FinalBeam.beamNormal, FinalBeam.origin - FinalBeam.BeamVertex[0]));
					origin = TransBeamOrigin;
					//continue;
				}

				if (fabs(origin.z - Zheight) < DOUBLE_EPSILON)  //beam的源点在仿真面上，此时不可能存在覆盖区域
					continue;

				//若没有仿真面时，只是在一条直线上取接收点，此时无法求得仿真区域，需要对所有接收点考察有效性或者对于实测位置处的接收点预测信号强度时，由于不位于仿真面上，所以也得对所有接收点考察有效性
				if (ce==ComputationEnum::ReceivePoint)
				{
					start_rowId = 0;
					end_rowId = m_cellData->efildVec.size() - 1;
					start_columnId = 0;
					end_columnId = 0;
					illuminate = true;
				}
				else
				{
					bool ALLbackward = true;   //判断是否beam的所有棱边都背离仿真面传播
					//判断beam是否背向仿真面传播
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
						end_rowId = sp.scene_width;
						start_columnId = 0;
						end_columnId = sp.scene_length;
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
						EfieldPointInPolygon(illunimatre_Vertexs, Xmin, Ymin, start_rowId, end_rowId, start_columnId, end_columnId,sp);
						illuminate = true;
					}
				}


				//对beam覆盖的接收点计算信号强度

				if (illuminate && (ce==ComputationEnum::ReceivePoint || start_rowId*(start_rowId - sp.scene_width) <= 0 && end_rowId*(end_rowId - sp.scene_width) <= 0 && start_columnId*(start_columnId - sp.scene_length) <= 0 && end_columnId*(end_columnId - sp.scene_length) <= 0))
				{
					for (int columnId = start_columnId; columnId <= end_columnId; columnId++)
					{
						for (int rowId = start_rowId; rowId <= end_rowId; rowId++)
						{
							if (ce == ComputationEnum::ReceivePoint)  //只有在仿真面设置的接收点位置时候，才有scene_width，而对于非仿真面设置的接收点，不存在scene_width
							{
								sp.scene_width = 0;
							}
							EField *NField = m_cellData->efildVec[columnId*(sp.scene_width + 1) + rowId];
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
									}
									num_RefTransPath++;
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
		cout << "---------- num_RefTransPath = " << num_RefTransPath << endl;
}


void algo::valid_RefDiffPath(ComputePara *cptPara, emxKdTree* pKdTree, vector<Vedge> &Edge_list, Vector3d AP_position, Cell_Data* m_cellData, const vector<vector<beamNode>> &beamRoutes)
{
	int num_RefDiff_diff = 0;
	double Xmin = EFieldArray[0].Position.x;
	double Ymin = EFieldArray[0].Position.y;
	double Zheight = EFieldArray[0].Position.z;
	Vector3d Point_IN_SimPlane(Xmin, Ymin, Zheight);
	Vector3d SimPlane_Normal(0, 0, 1);

}



algo::algo()
{
}

algo::~algo()
{
}

void algo::pluginAlgo(ModelPara *modelParameter, ComputePara *cptPara, visPara *vPara)
{
	bool ok;
	int totalcpu = omp_get_num_procs();
	cout << "----------开始计算--------------" << endl;
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

	//对每一个站点循环处理
	while (siteIterator != cptPara->Sites.end())
	{
		SphereBeam* SphereTest = new SphereBeam;
		SphereTest = SphereTest->creat(cptPara->RT_sample, cptPara->RT_radius);  //界面交互设置的采样密度即对正20面体细分的次数
		int BeamNumber = SphereTest->m_Face.size();; //发射源采样Beam数目
		const char *fileName = "D:\\calculation_log.dat";
		ofstream fout(fileName);

		//保存结果的站点文件
		Site_Data* m_siteData = new Site_Data(siteIterator->first);
		double beginTime = clock();

		if (modelParameter->mType == ModelType::CITY_LOCAL)
		{
			//
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
			vector<emxBeam*> pRootBeams;
			double time6 = clock();
			CreateInitialBeam(pRootBeams, SphereTest, AP_position, BeamNumber);
			fout << "pRootBeams Number: " << pRootBeams.size() << endl;
			double time7 = clock();
			double time_initialBeam = (time7 - time6) / 1000;
			fout << "time_initialBeam:  " << time_initialBeam << endl;
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

			totalpathnum = AP_route.size();
			fout << "the total possible path number is " << totalpathnum * 2 << endl;
			for (int i = 0; i < pRootBeams.size(); i++)
			{
				delete pRootBeams[i];
				pRootBeams[i] = NULL;
			}

			//找出有效直射、反射、透射、绕射传播路径
			fout << "Begin to find the valid path of receivers in  site  " << siteIterator->second->Site_Name << endl;

			//接收点坐标设置
			Site_Data * m_siteData;
			Scene_para s_para;


			if (cptPara->computeEnum == ComputationEnum::ReceivePoint)  //非仿真面的接收点设置
			{
				//根据接收点的pci信息，生成要计算的站点结果，
				//当然目前只包含位置，信号强度需要下面算法继续计算，并保存到 m_siteData
				for (int j = 0; j < cptPara->No_SimPlanePoint.size(); j++)
				{
					int pci = cptPara->No_SimPlanePoint[j].PCI;

					auto transVec = siteIterator->second->Site_Antennas;
					//检测导入的站点文件是否包含此PCI
					bool isPciInSite = false;
					string cellName = "";
					for (vector<TransAntenna>::iterator it = transVec.begin(); it != transVec.end(); it++)
					{
						if ((*it).PCI == pci)
						{
							isPciInSite = true;
							cellName = (*it).Cell_Name;
							break;
						}
					}
					//检测结果文件是否包含此pci
					if (m_siteData->cellsMap.find(pci) == m_siteData->cellsMap.end())
					{
						m_siteData->cellsMap.insert(make_pair(pci, new Cell_Data(pci, cellName)));
					}
					//
					if (isPciInSite)
					{
						//加入本站点要计算的接受点
						EField *s = new EField;
						s->Position = cptPara->No_SimPlanePoint[j].position;
						m_siteData->cellsMap[pci]->efildVec.push_back(s);
					}
					else
					{
						continue;
					}

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
				SetEFieldPoint(m_siteData, AP_position, modelParameter->SiteModels[siteIterator->first]->getSceneRange(), cptPara->altitude, cptPara->precision, s_para, modelParameter);
			}

			//接收点的内外特征判断
			Point_In_Out(cptPara->computeEnum, m_siteData, modelParameter->SiteModels[siteCount]->getLocalScene()->getTotal_Building(), cptPara->altitude, s_para);
			//接收点设置完毕

			//开始以cell为单位循环计算
			auto cellIterator = m_siteData->cellsMap.begin();
			while (cellIterator != m_siteData->cellsMap.end())
			{
				//直射
				double time10 = clock();
				valid_DirPath(AP_KdTree, AP_position, cellIterator->second);
				double time11 = clock();
				double time_DirPath = (time11 - time10) / 1000;
				fout << "time_DirPath:  " << time_DirPath << endl;

				valid_RefTransPath(cptPara->computeEnum, s_para, AP_KdTree, AP_position, cellIterator->second, AP_route);
				double time12 = clock();
				double time_RefTransPath = (time12 - time11) / 1000;
				fout << "time_RefTransPath:  " << time_RefTransPath << endl;
				cout << "time_RefTransPath:  " << time_RefTransPath << endl;

				if (cptPara->diffractionNumPara>=1)
				{
					vector<Vedge> currentEdge = currentModel->getAP_Edge_List();
					valid_OnceDiffPath(currentEdge, AP_KdTree, AP_position, cellIterator->second);
					double time13 = clock();
					double time_once_diffPath = (time13 - time12) / 1000;
					fout << "time_once_diffPath:  " << time_once_diffPath << endl;
					cout << "time_once_diffPath:  " << time_once_diffPath << endl;

					valid_RefDiffPath(TODO, AP_KdTree, AP_Edge_list, AP_position, EFieldArray, AP_route);
					double time16 = clock();
					double time_Ref_LastdiffPath = (time16 - time13) / 1000;
					fout << "time_Ref_LastdiffPath:  " << time_Ref_LastdiffPath << endl;
					cout << "time_Ref_LastdiffPath:  " << time_Ref_LastdiffPath << endl;
				}

				double time13 = clock();
				double time_totalValidPath = (time13 - time10) / 1000;
				fout << "the end of finding valid paths" << endl;
				fout << "time_totalValidPath:  " << time_totalValidPath << endl;
				fout << "Begin to calculate the signal strength of receivers in  site  " << siteIterator->second->Site_Name << endl;
				cout << "Begin to calculate the signal strength of receivers in  site  " << siteIterator->second->Site_Name << endl;

				cellIterator++;
			}


		}
		else if (modelParameter->mType == ModelType::OBJ_LOCAL)
		{
			//暂时不处理
		}



		siteCount++;
		siteIterator++;
	}

}