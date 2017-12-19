#include "visualManager.h"
#include <QJsonDocument>  
#include <QJsonArray>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QJsonObject>  
#include "Context/context.h"
#include <QDebug>
#include <QProgressDialog>

string Trim(string &str)   //��ȡ�������ո��Ʊ�����س������з����ַ���
{
	str.erase(0, str.find_first_not_of(" trn"));

	str.erase(str.find_last_not_of(" trn") + 1);

	return str;
}

VisualManager::VisualManager()
{
	m_SiteCellSubject = new SiteCellSubject;
	visContainer = new EFieldContainer;
}


VisualManager::~VisualManager()
{

}

void VisualManager::setContainerData()
{
	if (para==nullptr||para->simuResult.size()==0)
	{
		return;
	}
	map<int, Site_Data *> &  visRes = para->simuResult;

	//��һ���������֤visPara��Container�ֿ�
	auto it = visRes.begin();
	//���֮ǰ������
	visContainer->deleteAllSimuData();

	//����վ��
	for (; it != visRes.end();it++)
	{
		int siteID = it->second->siteID;
		//����cell
		auto cell_Iterator = it->second->cellsMap.begin();
		for (; cell_Iterator != it->second->cellsMap.end();cell_Iterator++)
		{
			Cell_Data * cell = cell_Iterator->second;//��cell
			Cell_Data * newCell = new Cell_Data(cell->pci);//��cell
			//�����򵥱���
			newCell->cell_name = cell->cell_name;
			newCell->col = cell->col;
			newCell->row = cell->row;
			newCell->pricision = cell->pricision;

			//����ÿһ������㣬���
			auto eField_Iterator = cell->efildVec.begin();
			for (; eField_Iterator != cell->efildVec.end();eField_Iterator++)
			{
				EField *source = *eField_Iterator;
				EField * tmp = new EField(*source);//������
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
				ite++;
			}
		}
	}
	else
	{
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
	json.insert("File_Type", "�������ļ�");
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
	//���� id x y z RSBP�洢
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
		string str_x = Trim(parameters[1]);
		string str_y = Trim(parameters[2]);
		string str_z = Trim(parameters[3]);
		string PCI = Trim(parameters[4]);
		string RSRP = Trim(parameters[5]);

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

void VisualManager::correct()
{
	int site_id = 0, PCI_id = 0, Field_id = 0;   //��¼�������ڲ��Ե�30%���ݵ���ʼsite_id��PCI_id��field_id
	double k1 = 0, k2 = 0, weight = 0;
	leastSquare(k1, k2, weight ,site_id, PCI_id, Field_id);

	vector<double>beforeCorrection_calculationfield;
	vector<double>afterCorrection_calculationfield;
	vector<double>Measuredfield;

	map<int, Site_Data*> &tmpMeasured = visContainer->getMeasuredData();
	map<int, Site_Data*> &tmpSim = visContainer->getSimData();

	auto it1 = tmpMeasured.find(site_id);
	auto it2 = it1->second->cellsMap.find(PCI_id);
	int id3 = Field_id;
	for (; it1 != tmpMeasured.end();it1++)
	{
		for (; it2 != it1->second->cellsMap.end();it2++)
		{
			for (; id3 < it2->second->efildVec.size();id3++)
			{
				EField * measuredE = it2->second->efildVec[id3];
				EField *simE = tmpSim[it1->first]->cellsMap[it2->first]->efildVec[id3];
				Measuredfield.push_back(measuredE->MolStrength);
				beforeCorrection_calculationfield.push_back(simE->MolStrength);
				double afterCorrection_Molstrength = weight*simE->MolStrength+k1*log10()
			}
		}
	}
}

void VisualManager::leastSquare(double &a, double &b, double &weight, int &site_id, int &PCI_id, int &Field_id)
{
	double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	int total_ReceiverNum = 0;
	map<int, Site_Data*> &tmpMeasured = visContainer->getMeasuredData();
	map<int, Site_Data*> &tmpSim = visContainer->getSimData();
	for (auto it = tmpMeasured.begin(); it != tmpMeasured.end();it++)//����վ��
	{
		for (auto it2 = it->second->cellsMap.begin(); it2 != it->second->cellsMap.end();it2++)//����pci
		{
			for (int  it3 =0; it3 < it2->second->efildVec.size();it3++)//�������н��յ�
			{
				if (tmpSim[it->first]->cellsMap[it2->first]->efildVec[it3]->Path.size()!=0)//���յ���Ч
				{
					total_ReceiverNum++;
				}
			}
		}
	}
	//ʵ�������� 70%����У����30%���ڲ�����֤��ȷ��
	weight = 0.5;
	int n = 0;
	bool flag = true;
	for (auto it = tmpMeasured.begin();flag && it != tmpMeasured.end(); it++)//����վ��
	{
		for (auto it2 = it->second->cellsMap.begin(); flag && it2 != it->second->cellsMap.end(); it2++)//����pci
		{
			for (int it3 = 0; flag && it3 < it2->second->efildVec.size(); it3++)//�������н��յ�
			{
				EField* MeasureE = it2->second->efildVec[it3];
				EField* tmpE = tmpSim[it->first]->cellsMap[it2->first]->efildVec[it3];
				if (tmpE->Path.size() != 0) //����Ԥ���������·������Ľ��յ㣬����ģ��У��
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
						//�Ѿ��ﵽʵ������70%���˳������ѭ��
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
