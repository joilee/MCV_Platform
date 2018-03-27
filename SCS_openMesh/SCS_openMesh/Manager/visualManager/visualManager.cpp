#include "visualManager.h"
#include <QJsonDocument>  
#include <QJsonArray>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QJsonObject>  
#include "Context/context.h"
#include <QDebug>
#include <QProgressDialog>
#include <util/stringUtil.h>

//**************模型校正**************//
//均值
double mean(vector<double> &X, vector<double>&Y)
{
	double t1 = 0;
	for (int i = 0; i < X.size(); i++)
	{
		t1 += (X[i] - Y[i]);
	}
	t1 /= X.size();
	return t1;
}

//标准差
double Standard_Deviation(vector<double> &X, vector<double>&Y)
{
	double Mean_value = mean(X, Y);
	double t1 = 0;
	for (int i = 0; i < X.size(); i++)
	{
		t1 += ((X[i] - Y[i] - Mean_value) * (X[i] - Y[i] - Mean_value));
	}
	t1 = sqrt(t1 / X.size());
	return t1;
}

//相关系数
double Correlation_Coefficient(vector<double> &X, vector<double>&Y)
{
	double t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;

	int N = X.size();

	for (int i = 0; i < X.size(); i++)
	{
		t1 += X[i] * Y[i];
		t2 += X[i];
		t3 += Y[i];
		t4 += X[i] * X[i];
		t5 += Y[i] * Y[i];
	}
	return (N*t1 - t2*t3) / (sqrt(N*t4 - t2*t2) * sqrt(N * t5 - t3*t3));
}

VisualManager::VisualManager()
{
	m_SiteCellSubject = new SiteCellSubject;
	visContainer = new EFieldContainer;
	para = new visPara;
}


VisualManager::~VisualManager()
{

}

void VisualManager::setContainerData()
{
	if (para==nullptr||para->simuResult.size()==0)
	{
		qDebug() << "para为null"<< para->simuResult.size();
		return;
	}

	map<int, Site_Data *> &  visRes = para->simuResult;

	//做一次深拷贝，保证visPara和Container分开
	auto it = visRes.begin();
	//清除之前的数据
	visContainer->deleteAllSimuData();

	//遍历站点
	for (; it != visRes.end();it++)
	{
		int siteID = it->second->siteID;
		//遍历cell
		auto cell_Iterator = it->second->cellsMap.begin();
		for (; cell_Iterator != it->second->cellsMap.end();cell_Iterator++)
		{
			Cell_Data * cell = cell_Iterator->second;//旧cell
			Cell_Data * newCell = new Cell_Data(cell->pci);//新cell
			//拷贝简单变量
			newCell->cell_name = cell->cell_name;
			newCell->col = cell->col;
			newCell->row = cell->row;
			newCell->pricision = cell->pricision;

			//遍历每一个仿真点，深拷贝
			auto eField_Iterator = cell->efildVec.begin();
			for (; eField_Iterator != cell->efildVec.end();eField_Iterator++)
			{
				EField *source = *eField_Iterator;
				EField * tmp = new EField(*source);//解引用
				newCell->efildVec.push_back(tmp);
			}
			visContainer->addCellData(newCell, siteID);
		}
	}

	sendSiteCellName();
}

visPara* VisualManager::getVisualPara()
{
	return para;
}

void VisualManager::sendSiteCellName()
{
	if (visContainer->isDataExist())
	{
		m_SiteCellSubject->clearItem();
		vector<int> siteIDS = visContainer->getAllSiteID();
		for (int i = 0; i < siteIDS.size();i++)
		{
			Site_Data * tmpSite = visContainer->getSiteDataByID(siteIDS[i]);
			auto ite = tmpSite->cellsMap.begin();
			while (ite != tmpSite->cellsMap.end())
			{
				m_SiteCellSubject->siteAndCellData->addCell(siteIDS[i],ite->second->pci,ite->second->cell_name);
				m_SiteCellSubject->siteAndCellData->setFlag(true);
				ite++;
			}
		}
	}
	else
	{
		m_SiteCellSubject->siteAndCellData->setFlag(false);
		m_SiteCellSubject->clearItem();
		
	}
	m_SiteCellSubject->notify();
}

void VisualManager::saveCellFile(QString dir,int pci,int siteID)
{

	QFile f(dir);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Open failed." << endl;
		return;
	}
	globalContext *globalCtx = globalContext::GetInstance();
	ComputePara *cptPara = globalCtx->cptManager->getComputationPara();
	if (cptPara == nullptr)
	{
		qDebug() << "No computation para" << endl;
	}
	QJsonObject json;
	json.insert("File_Type", "仿真结果文件");
	json.insert("Site_ID", siteID);
	json.insert("Cell_PCI", pci);
	json.insert("reflect_count", cptPara->reflectNumPara);
	json.insert("sample_count", cptPara->RT_sample);
	json.insert("sampleRayNum", cptPara->RT_BeamNum);
	json.insert("computation_Enum", (int)(cptPara->computeEnum));
	json.insert("runtime", para->runTime);

	Cell_Data * tmpCell = visContainer->getCellDataByPCI(pci);
	if (cptPara->computeEnum==ComputationEnum::SimuPlane)
	{
		json.insert("simuPlane_row", tmpCell->row);//
		json.insert("simuPlane_col", tmpCell->col);
		json.insert("smuPlane_pricison", tmpCell->pricision);
	}

	QJsonArray data;
	//按照 id x y z RSBP存储
	for (int i = 0; i < tmpCell->efildVec.size();i++)
	{
		QJsonArray dataTmp;
		dataTmp.push_back(i);
		dataTmp.push_back(tmpCell->efildVec[i]->Position.x);
		dataTmp.push_back(tmpCell->efildVec[i]->Position.y);
		dataTmp.push_back(tmpCell->efildVec[i]->Position.z);
		dataTmp.push_back(tmpCell->efildVec[i]->MolStrength);
		data.push_back(dataTmp);
	}
	json.insert("data", data);
	QJsonDocument document;
	document.setObject(json);
	QByteArray byte_array = document.toJson(QJsonDocument::Indented);
//	QString json_str(byte_array);
	QTextStream st(&f);
	st << byte_array;
	f.close();
	return;
}

void VisualManager::loadMeasuredFile(QString path)
{
	visContainer->clearMeasuredData();
	ifstream infile((path.toStdString()).c_str(), ios::in | ios::_Nocreate);
	if (!infile)
	{
		cout << "can not open file!" << endl;
		return;
	}

	string str;
	getline(infile, str);
	//获取实测数据文件中所需信息对应所在列的ID号
	int x_id = -1, y_id = -1, z_id = -1, pci_id = -1, rsrp_id = -1;
	istringstream linestream1(str);
	string parameter1;
	int count = 0;
	while (getline(linestream1, parameter1, ','))
	{
		parameter1 = Trim(parameter1);
		if (parameter1 == "x" || parameter1 == "X")
		{
			x_id = count;
		}
		if (parameter1 == "y" || parameter1 == "Y")
		{
			y_id = count;
		}
		if (parameter1 == "z" || parameter1 == "Z")
		{
			z_id = count;
		}
		if (parameter1 == "PCI")
		{
			pci_id = count;
		}
		if (parameter1 == "RSRP(dBm)")
		{
			rsrp_id = count;
		}
		count++;
	}

	globalContext *globalCtx = globalContext::GetInstance();
	while (getline(infile, str))
	{
		istringstream linestream(str);
		vector<string>parameters;
		string parameter;
		while (getline(linestream, parameter, ','))
		{
			parameters.push_back(parameter);
		}
		string str_x = Trim(parameters[x_id]);
		string str_y = Trim(parameters[y_id]);
		string str_z = Trim(parameters[z_id]);
		string PCI = Trim(parameters[pci_id]);
		string RSRP = Trim(parameters[rsrp_id]);

		double x = atof(str_x.c_str());
		double y = atof(str_y.c_str());
		double z = atof(str_z.c_str()) + globalCtx->modelManager->getFirstCity()->getAltitude(x, y);
		int current_PCI = atoi(PCI.c_str());
		EField * tmpEfield = new EField;
		tmpEfield->Position = Vector3d(x,y,z);
		tmpEfield->MolStrength = atof(RSRP.c_str());
		visContainer->addEfield_Measured(tmpEfield, current_PCI);
	}
	infile.close();
}

void VisualManager::correct(Vector3d &before,Vector3d &after)
{
	int site_id = 0, PCI_id = 0, Field_id = 0;   //记录后面用于测试的30%数据的起始site_id、PCI_id和field_id
	double k1 = 0, k2 = 0, weight = 0;
	leastSquare(k1, k2, weight ,site_id, PCI_id, Field_id);

	beforeCorrection_calculationfield.clear();
	afterCorrection_calculationfield.clear();
	Measuredfield.clear();

	map<int, Site_Data*> &tmpMeasured = visContainer->getMeasuredData();
	map<int, Site_Data*> &tmpSim = visContainer->getSimData();

	auto it1 = tmpMeasured.find(site_id);
	auto it2 = it1->second->cellsMap.find(PCI_id);
	int id3 = Field_id;
	int id = 0;
	bool startFlag=true;
	for (; it1 != tmpMeasured.end();it1++)
	{

		if (startFlag==true)
		{
			it2 = it1->second->cellsMap.find(PCI_id);
		}
		else
		{
			it2 != it1->second->cellsMap.begin();
		}

		for (; it2 != it1->second->cellsMap.end(); it2++)
		{
			for (id3 = Field_id; id3 < it2->second->efildVec.size(); id3++)
			{
				EField * measuredE = it2->second->efildVec[id3];
				EField *simE = tmpSim[it1->first]->cellsMap[it2->first]->efildVec[id3];
				if (simE->Path.size()!=0)
				{
					Measuredfield.push_back(measuredE->MolStrength);
					beforeCorrection_calculationfield.push_back(simE->MolStrength);
					double afterCorrection_Molstrength = weight*simE->MolStrength + k1*log10(simE->HorizontalDis) + k2;
					afterCorrection_calculationfield.push_back(afterCorrection_Molstrength);
					id++;
				}

			}
			id3 = 0;
		}
		startFlag = false;
	}

	double beforeCorrection_mean_value = mean(Measuredfield, beforeCorrection_calculationfield);
	double beforeCorrection_Standard_Deviation_value = Standard_Deviation(Measuredfield, beforeCorrection_calculationfield);
	double beforeCorrection_Correlation_Coefficient_value = Correlation_Coefficient(Measuredfield, beforeCorrection_calculationfield);
	cout << endl;
	cout << "Before the model correction:" << endl;
	cout << "Mean Value:  " << beforeCorrection_mean_value << endl;
	cout << "Standard Deviation:  " << beforeCorrection_Standard_Deviation_value << endl;
	cout << "Correction Coefficient:  " << beforeCorrection_Correlation_Coefficient_value << endl;
	//after correction
	double afterCorrection_mean_value = mean(Measuredfield, afterCorrection_calculationfield);
	double afterCorrection_Standard_Deviation_value = Standard_Deviation(Measuredfield, afterCorrection_calculationfield);
	double afterCorrection_Correlation_Coefficient_value = Correlation_Coefficient(Measuredfield, afterCorrection_calculationfield);
	cout << endl;
	cout << "After the model correction:" << endl;
	cout << "Mean Value:  " << afterCorrection_mean_value << endl;
	cout << "Standard Deviation:  " << afterCorrection_Standard_Deviation_value << endl;
	cout << "Correction Coefficient:  " << afterCorrection_Correlation_Coefficient_value << endl;

	before = Vector3d(beforeCorrection_mean_value, beforeCorrection_Standard_Deviation_value, beforeCorrection_Correlation_Coefficient_value);
	after = Vector3d(afterCorrection_mean_value, afterCorrection_Standard_Deviation_value, afterCorrection_Correlation_Coefficient_value);
	cout << "end of correction"<< endl;
}

void VisualManager::leastSquare(double &a, double &b, double &weight, int &site_id, int &PCI_id, int &Field_id)
{
	double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	int total_ReceiverNum = 0;
	map<int, Site_Data*> &tmpMeasured = visContainer->getMeasuredData();
	map<int, Site_Data*> &tmpSim = visContainer->getSimData();
	for (auto it = tmpMeasured.begin(); it != tmpMeasured.end();it++)//遍历站点
	{
		for (auto it2 = it->second->cellsMap.begin(); it2 != it->second->cellsMap.end();it2++)//遍历pci
		{
			for (int  it3 =0; it3 < it2->second->efildVec.size();it3++)//遍历所有接收点
			{
				if (tmpSim[it->first]->cellsMap[it2->first]->efildVec[it3]->Path.size()!=0)//接收点有效
				{
					total_ReceiverNum++;
				}
			}
		}
	}
	//实测数据中 70%用来校正，30%用于测试验证正确性
	weight = 0.5;
	int n = 0;
	bool flag = true;
	for (auto it = tmpMeasured.begin();flag && it != tmpMeasured.end(); it++)//遍历站点
	{
		for (auto it2 = it->second->cellsMap.begin(); flag && it2 != it->second->cellsMap.end(); it2++)//遍历pci
		{
			for (int it3 = 0; flag && it3 < it2->second->efildVec.size(); it3++)//遍历所有接收点
			{
				EField* MeasureE = it2->second->efildVec[it3];
				EField* tmpE = tmpSim[it->first]->cellsMap[it2->first]->efildVec[it3];
				if (tmpE->Path.size() != 0) //仅对预测仿真中有路径到达的接收点，参与模型校正
				{
					n++;
					double x = log10(tmpE->HorizontalDis);
					double y = MeasureE->MolStrength - weight*tmpE->MolStrength;
					t1 += x*x;
					t2 += x;
					t3 += x*y;
					t4 += y;

					if (n>=0.7*total_ReceiverNum)
					{
						if (it3==it2->second->efildVec.size()-1)
						{
							Field_id = it3 ;
						}
						else
						{
							Field_id = it3 + 1;
						}
						//已经达到实测数据70%，退出最外层循环
						PCI_id = it2->first;
						site_id = it->first;
						flag = false;
						break;
					}
				}
			}
		}
	}

	a = (t3*n - t2*t4) / (t1*n - t2*t2);
	b = (t1*t4 - t2*t3) / (t1*n - t2*t2);
}

EFieldContainer * VisualManager::getContainer()
{
	return visContainer;

}
