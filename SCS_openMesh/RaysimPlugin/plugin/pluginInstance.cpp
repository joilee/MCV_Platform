#include "pluginInstance.h"
#include <QInputDialog>
#include <omp.h>  //OpenMp并行编程需要包含的头文件
#include <QMessageBox>
#include "geometry/SphereBeam.h"
#include "geometry/emxKdTreeD.h"
#include "geometry/emxFace.h"
#include "geometry/emxModel.h"
#include "util/EField.h"
#include "algo.h"
//#include "raysim.h"


PluginInstance::PluginInstance()
{
}



/************************************************************************/
/* 算法函数  动态加载                                                                                  */
/************************************************************************/
void PluginInstance::runAlgorithm(ModelPara *modelParameter,ComputePara *cptPara,visPara *vPara)
{
	bool ok;
	int totalcpu= omp_get_num_procs();
	cout<<"----------开始计算--------------"<<endl;
	int core_nums = QInputDialog::getInt(NULL,QStringLiteral("cpu core"),QString("There are ")+QString::number(totalcpu)+" cores in this PC.\n use ", 0, 0, 100, 1, &ok);
	omp_set_num_threads(core_nums);   //指定用于并行计算的线程数目

	if (modelParameter->SiteModels.empty())
	{
		QMessageBox::warning(NULL, QStringLiteral("模型文件"), QStringLiteral("请加载模型文件"));
		return;
	}
	if(cptPara->Sites.size() == 0)
	{
		QMessageBox::warning(NULL, QStringLiteral("发射天线"), QStringLiteral("请添加发射天线文件"));
		return;
	}
	if(modelParameter->materialdatabase.size() == 0)
	{
		QMessageBox::warning(NULL, QStringLiteral("材质文件"), QStringLiteral("请加载材质文件"));
		return;
	}
	map<int, Site_Data *> &AP_EFieldMap = vPara->simuResult;
	AP_EFieldMap.clear();
	auto siteIterator = cptPara->Sites.begin();
	int siteCount = 0;

	//对每一个站点循环处理
	while (siteIterator!=cptPara->Sites.end())
	{
		SphereBeam* SphereTest = new SphereBeam;
		SphereTest = SphereTest->creat(cptPara->RT_sample, cptPara->RT_radius);  //界面交互设置的采样密度即对正20面体细分的次数
		int BeamNumber = SphereTest->m_Face.size();; //发射源采样Beam数目
		const char *fileName = "D:\\calculation_log.dat";
		ofstream fout(fileName);

		Site_Data* m_siteData = new Site_Data(siteIterator->first);
		double beginTime = clock();

		if (modelParameter->mType==ModelType::CITY_LOCAL)
		{
			emxModel *trianglePara = new emxModel(modelParameter->SiteModels[siteCount]);
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


			if (cptPara->computeEnum==ComputationEnum::ReceivePoint)  //非仿真面的接收点设置
			{
				//根据接收点的pci信息，生成要计算的站点结果，
				//当然目前只包含位置，信号强度需要下面算法继续计算，并保存到 m_siteData
				for (int j = 0; j < cptPara->No_SimPlanePoint.size(); j++)
				{
					int pci = cptPara->No_SimPlanePoint[j].PCI;
					
					auto transVec= siteIterator->second->Site_Antennas;
					//检测导入的站点文件是否包含此PCI
					bool isPciInSite = false;
					string cellName = "";
					for (vector<TransAntenna>::iterator it = transVec.begin(); it != transVec.end();it++)
					{
						if ((*it).PCI==pci)
						{
							isPciInSite = true;
							cellName = (*it).Cell_Name;
							break;
						}
					}
					//检测结果文件是否包含此pci
					if (m_siteData->cellsMap.find(pci) == m_siteData->cellsMap.end())
					{
						m_siteData->cellsMap.insert(make_pair(pci, new Cell_Data(pci,cellName)));
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
			//接收点设置完毕



			//接收点的内外特征判断
			Point_In_Out(m_siteData, modelParameter->SiteModels[siteCount]->getLocalScene()->getTotal_Building(), cptPara->altitude, s_para);

		}
		else if (modelParameter->mType == ModelType::OBJ_LOCAL)
		{
			//暂时不处理
		}
		


		siteCount++;
		siteIterator++;
	}




	

	

	


	
	
	for ( int site_id = 0; site_id< cptPara->Sites.size();site_id++)
	{
		

	
		





		//设置虚拟仿真面，并构建仿真面kd-tree
		double t1 = clock();
		double SimPlaneHeight=cptPara->altitude;
		emxModel * LocalVirtualSimPlane = new emxModel(modelParameter->getGround_Mesh(), SimPlaneHeight);
		emxKdTree *LocalVirtualSimPlane_KdTree = new emxKdTree(LocalVirtualSimPlane, 80, 1, 0.5, 1, -1);
		double t2 = clock();
		double time_SimPlaneKdTree = (t2 - t1)/1000;
		fout <<"time_SimPlaneKdTree:  "<<time_SimPlaneKdTree<<endl;

		double time10 = clock();
		valid_DirPath(AP_KdTree,AP_position,Cell_EFieldArray);
		double time11 = clock();
		double time_DirPath = (time11-time10)/1000;
		fout <<"time_DirPath:  "<<time_DirPath<<endl;

		double time13 = clock();
		double time_totalValidPath = (time13-time10)/1000;
		fout<<"the end of finding valid paths"<<endl;
		fout <<"time_totalValidPath:  "<<time_totalValidPath<<endl;

		fout<<"Begin to calculate the signal strength of receivers in  site  "<<cptPara->Sites[site_id].Site_Name<<endl;

		for (int Cell_id = 0; Cell_id < cptPara->Sites[site_id].Site_Antennas.size();Cell_id++)
		{
			fout<<endl;
			fout<<"Cell:  "<<cptPara->Sites[site_id].Site_Antennas[Cell_id].Cell_Name<<endl;
			vector<EField> EFieldArray(Cell_EFieldArray);
			TransAntenna AP = cptPara->Sites[site_id].Site_Antennas[Cell_id];

			for (int j= 0; j < EFieldArray.size(); j++)
			{
				if((AP.position - EFieldArray[j].Position).norm() <1e-10) //发射点、接收点重合情况
				{
					double Power1 = AP.trans_power + AP.enlarge_power - AP.wire_loss; 
					EFieldArray[j].MolStrength += pow(10,Power1/10);   //单位为：mW
				}
			}

			double time14 = clock();
			Antenna_Para aPara;
			Calc_GO_UTD(AP,EFieldArray,modelParameter->getAP_Edge_List(), aPara,cptPara,modelParameter);
			double time15= clock();
			double time_totalSignalCalculation  = (time15-time14)/1000;
			fout<<"the end of  signal calculation"<<endl;
			fout<<"time_totalSignalCalculation:  "<<time_totalSignalCalculation<<endl;



			double k1 = 0;
			double k2 = 0;
			double weight = 1;

			int total_validpathnum = 0;
			for(int id = 0; id < EFieldArray.size(); id++)
			{
				total_validpathnum+=EFieldArray[id].Path.size();

				if(EFieldArray[id].Path.size() != 0) //有路径到达的点
				{
					EFieldArray[id].MolStrength =  (aPara.lamda *aPara. lamda / (4 * M_PI * 120 * M_PI))*( (EFieldArray[id].EFieldAll).x.real()*(EFieldArray[id].EFieldAll).x.real() + (EFieldArray[id].EFieldAll).x.imag()*(EFieldArray[id].EFieldAll).x.imag()  + (EFieldArray[id].EFieldAll).y.real()*(EFieldArray[id].EFieldAll).y.real() + (EFieldArray[id].EFieldAll).y.imag()*(EFieldArray[id].EFieldAll).y.imag() + (EFieldArray[id].EFieldAll).z.real()*(EFieldArray[id].EFieldAll).z.real() + (EFieldArray[id].EFieldAll).z.imag()*(EFieldArray[id].EFieldAll).z.imag() );  //场强转成功率
					EFieldArray[id].MolStrength = 10*log10(EFieldArray[id].MolStrength) + 30;  //W转为dBmw
					//EFieldArray[id].MolStrength =  k1*EFieldArray[id].MolStrength + k2;  //模型校正参数  k1*molstrength + k2
					EFieldArray[id].MolStrength =  weight*EFieldArray[id].MolStrength + k1*log10(EFieldArray[id].HorizontalDis) + k2;  //模型校正参数  k1*molstrength + k2
				}
			}

			vPara->horizonNum.push_back(s_para.scene_length);
			vPara->veticalNum.push_back(s_para.scene_width);
			AP_EFieldArrays.push_back(EFieldArray);
			fout<<"the total valid path num is :"<<total_validpathnum<<endl;
		}
		fout<<"The end computation of  site  "<<cptPara->Sites[site_id].Site_Name<<endl;	
		fout<<endl;
		
	}
	double endTime= clock();
	double runtime = (endTime - beginTime)/1000;
	fout<<"the total  runtime is  "<<runtime<<endl;
	cout<<"The total runtime is  "<<runtime<<endl;
	fout.close();
	


}