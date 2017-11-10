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
		cout << "error: 没有天线增益文件" << endl;
		return false;
	}
	if (siteFlag==false)
	{
		cout << "error:没有小区文件" << endl;
		return false;
	}
	return true;
}

void computeManager::openTransAntennas_DirGain(QStringList paths)
{
	for (int i = 0; i < cptPara->Sites.size(); i++)
	{
		for (int j = 0; j < cptPara->Sites[i].Site_Antennas.size(); j++)
		{
			//对每个site中每个cell从批量导入的方向增益文件中找到匹配的增益文件
			string cell_name = cptPara->Sites[i].Site_Antennas[j].Cell_Name + ".txt";
			for (int path_id = 0; path_id < paths.size(); path_id++)
			{
				string path = paths[path_id].toStdString();
				if (path.find(cell_name) != std::string::npos)
				{
					//打开发射天线方向增益文件，读取各方向增益
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
	QMessageBox::warning(NULL, QStringLiteral("多个站点方向增益文件"), QStringLiteral("加载成功"));
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
	getline(infile, str);//跳过第一行
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
		//从全局中获取
		double z = atof(height.c_str()) + globalCtx->modelManager->getFirstCity()->getCity()->getAltitude(x, y);

		TransAntenna new_antenna;
		new_antenna.Cell_Name = CellName;
		new_antenna.PCI = atoi(PCI.c_str());
		new_antenna.frequency = 1750; //单位为MHZ，设置了一个默认
		new_antenna.trans_power = 12.2; //单位为dBm
		new_antenna.wire_loss = 0.5;
		new_antenna.enlarge_power = 0;
		new_antenna.position = Vector3d(x, y, z);
		new_antenna.polor_direction = Vector3d(0, 0, 1);

		new_antenna.phi = 0;
		new_antenna.theta = 0;

		if (new_antenna.polor_direction.norm() < 1e-10)
		{
			QMessageBox::warning(NULL, QStringLiteral("发射天线属性设置"), QStringLiteral("请输入正确的极化方向"));
			cout << "error : 天线文件读取错误。已返回！" << endl;
			return;
		}	
		//针对新获得的cell，检测是否存在一个已知的site中，如果是，则插入，否则新建
		bool newsite = true;   //是否需要新建一个site
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
		if (newsite)//如果是新的小区
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
	cout << "info:读取天线文件成功" << endl;
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
		//是不是不能返回局部变量的引用
		cptPara->Sites.clear();
		return  cptPara->Sites;
	}
}