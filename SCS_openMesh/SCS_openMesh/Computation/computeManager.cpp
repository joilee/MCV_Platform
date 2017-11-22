#include "computeManager.h"
#include "../Context/context.h"
#include <fstream>
#include <QMessageBox>
#include <util/stringUtil.h>
computeManager::computeManager()
{
	cptPara = new ComputePara;
	subject = new antennaSubject;
	siteFlag = false;
	antennaFlag = false;
	noSimFlag = false;
}

computeManager::~computeManager()
{
	delete cptPara;
	delete subject;
}

bool computeManager::checkPara()
{
	if (antennaFlag==false)
	{
		cout << "error: û�����������ļ�" << endl;
		return false;
	}
	if (siteFlag==false)
	{
		cout << "error:û��С���ļ�" << endl;
		return false;
	}
	return true;
}

void computeManager::setCptCatagory(ComputationEnum ce)
{
	cptPara->computeEnum = ce;
}

void computeManager::setCptCatagory(int mode)
{
	cptPara->computeEnum = static_cast<ComputationEnum>(mode);
	cout <<"��������"<< static_cast<int>(cptPara->computeEnum)<<endl;
}

Vector3d computeManager::getSitePosition(int id)
{
	if (id>=cptPara->Sites.size())
	{
		QMessageBox::critical(NULL, QStringLiteral("����"), QStringLiteral("��ȡSiteλ�ó��������±곬����Χ��"));
	}
	Vector3d res=cptPara->Sites[id].Site_Antennas[0].position;
	return res;
}

void computeManager::openTransAntennas_DirGain(QStringList paths)
{
	for (int i = 0; i < cptPara->Sites.size(); i++)
	{
		for (int j = 0; j < cptPara->Sites[i].Site_Antennas.size(); j++)
		{
			//��ÿ��site��ÿ��cell����������ķ��������ļ����ҵ�ƥ��������ļ�
			string cell_name = cptPara->Sites[i].Site_Antennas[j].Cell_Name + ".txt";
			for (int path_id = 0; path_id < paths.size(); path_id++)
			{
				string path = paths[path_id].toStdString();
				if (path.find(cell_name) != std::string::npos)
				{
					//�򿪷������߷��������ļ�����ȡ����������
					ifstream infile(path.c_str(), ios::in | ios::_Nocreate);
					if (!infile)
					{
						cout << "can not open file!" << endl;
						return;
					}

					string str, str_flag;
					getline(infile, str);
					istringstream linestream(str);
					linestream >> str_flag;
					if (str_flag == "NAME")
					{
						getline(infile, str);
						getline(infile, str);
					}
					getline(infile, str);
					istringstream linestream1(str);
					linestream1 >> str_flag;
					if (str_flag == "Gain")
					{
						getline(infile, str);
						istringstream linestream2(str);
						linestream2 >> str_flag;
						cptPara->Sites[i].Site_Antennas[j].initial_Gain = atof(str_flag.c_str());
						getline(infile, str);
						getline(infile, str);
						getline(infile, str);
					}
					while (getline(infile, str))
					{
						istringstream linestream3(str);
						linestream3 >> str_flag;
						vector<double> antenna_property(3); // V_angle  H_angle  attenuation
						antenna_property[0] = atof(str_flag.c_str());
						linestream3 >> antenna_property[1] >> antenna_property[2];
						cptPara->Sites[i].Site_Antennas[j].direction_Gain.push_back(antenna_property);
						linestream3.str("");
					}
					infile.close();
					break;
				}
			}
		}
	}
	QMessageBox::warning(NULL, QStringLiteral("���վ�㷽�������ļ�"), QStringLiteral("���سɹ�"));
	antennaFlag = true;
}

void computeManager::openNo_simplaneReceiver(string path)
{
	cptPara->No_SimPlanePoint.clear();
	fstream infile(path.c_str(), ios::in | ios::_Nocreate);
	if (!infile)
	{
		cout << "error: can not open file!" << endl;
		return;
	}
	string str;
	getline(infile, str);
	globalContext *glbctx = globalContext::GetInstance();
	while (getline(infile, str))
	{
		istringstream linestream(str);
		vector<string>parameters;
		string parameter;
		while (getline(linestream, parameter, ','))
		{
			parameters.push_back(parameter);
		}
		string str_x = Trim(parameters[0]);
		string str_y = Trim(parameters[1]);
		string str_z = Trim(parameters[2]);
		string PCI = Trim(parameters[3]);
		double x = atof(str_x.c_str());
		double y = atof(str_y.c_str());
		double z = atof(str_z.c_str()) + glbctx->modelManager->getFirstLocal()->getAltitude(x, y);
		no_simplaneReceiver receiver;
		receiver.position = Vector3d(x, y, z);
		receiver.PCI = atoi(PCI.c_str());
		cptPara->No_SimPlanePoint.push_back(receiver);
	}
	infile.close();
	cptPara->no_simplane = true;
	noSimFlag = true;
}

void computeManager:: openTransAntenna_ParamFile(QString path)
{
	if (path.isEmpty())
		return;
	cptPara->Sites.clear();
	ifstream infile((path.toStdString()).c_str(), ios::in | ios::_Nocreate);
	if (!infile)
	{
		cout << "error:can not open file!" << endl;
		return;
	}

	globalContext *globalCtx = globalContext::GetInstance();
	string str, str_flag;
	getline(infile, str);//������һ��
	while (getline(infile, str))
	{
		istringstream linestream(str);
		vector<string>parameters;
		string parameter;
		while (getline(linestream, parameter, ','))
		{
			parameters.push_back(parameter);
		}
		string SiteName = Trim(parameters[0]);
		string CellName = Trim(parameters[1]);
		string PCI = Trim(parameters[3]);
		string height = Trim(parameters[8]);
		string str_x = Trim(parameters[14]);
		string str_y = Trim(parameters[15]);
		double x = atof(str_x.c_str());
		double y = atof(str_y.c_str());
		//��ȫ���л�ȡ
		double z = atof(height.c_str()) + globalCtx->modelManager->getFirstCity()->getCity()->getAltitude(x, y);

		TransAntenna new_antenna;
		new_antenna.Cell_Name = CellName;
		new_antenna.PCI = atoi(PCI.c_str());
		new_antenna.frequency = 1750; //��λΪMHZ��������һ��Ĭ��
		new_antenna.trans_power = 12.2; //��λΪdBm
		new_antenna.wire_loss = 0.5;
		new_antenna.enlarge_power = 0;
		new_antenna.position = Vector3d(x, y, z);
		new_antenna.polor_direction = Vector3d(0, 0, 1);

		new_antenna.phi = 0;
		new_antenna.theta = 0;

		if (new_antenna.polor_direction.norm() < 1e-10)
		{
			QMessageBox::warning(NULL, QStringLiteral("����������������"), QStringLiteral("��������ȷ�ļ�������"));
			cout << "error : �����ļ���ȡ�����ѷ��أ�" << endl;
			return;
		}	
		//����»�õ�cell������Ƿ����һ����֪��site�У�����ǣ�����룬�����½�
		bool newsite = true;   //�Ƿ���Ҫ�½�һ��site
		int current_sitename = stof(SiteName.c_str());
		for (int i = 0; i < cptPara->Sites.size(); i++)
		{
			if (current_sitename == cptPara->Sites[i].Site_Name)
			{
				newsite = false;
				cptPara->Sites[i].Site_Antennas.push_back(new_antenna);
				break;
			}
		}
		if (newsite)//������µ�С��
		{
			Site new_site;
			new_site.Site_Name = current_sitename;
			new_site.Site_Antennas.push_back(new_antenna);
			cptPara->Sites.push_back(new_site);
		}
		subject->AntennaItem->addSite(current_sitename, CellName);
	}
	infile.close();
	subject->notify();
	cout << "info:��ȡ�����ļ��ɹ�" << endl;
	siteFlag = true;
}

vector<Site> & computeManager::getSite()
{
	if (cptPara->Sites.size()!=0)
	{
		return cptPara->Sites;
	}
	else
	{
		//�ǲ��ǲ��ܷ��ؾֲ�����������
		cptPara->Sites.clear();
		return  cptPara->Sites;
	}
}