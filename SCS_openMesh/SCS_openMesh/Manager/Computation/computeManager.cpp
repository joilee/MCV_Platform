#include "computeManager.h"
#include "../../Context/context.h"
#include <fstream>
#include <QMessageBox>
#include <util/stringUtil.h>
#include <QFileInfo>
computeManager::computeManager()
{
	sitesContainer = new SitesContainer;
	cptPara = new ComputePara;
	subject = new antennaSubject;
	m_pluginSubject = new PluginSubject;
	siteFlag = false;
	antennaFlag = false;
	noSimFlag = false;
}

computeManager::~computeManager()
{
	delete cptPara;
	delete subject;
	delete sitesContainer;
}

bool computeManager::checkPara()
{
	if (antennaFlag==false)
	{
		cout << "error: û�����������ļ�" << endl;
		QMessageBox::warning(nullptr, QStringLiteral("���߲���"), QStringLiteral("ȱ�����������ļ�"));
		return false;
	}
	if (siteFlag==false)
	{
		QMessageBox::warning(nullptr, QStringLiteral("���߲���"), QStringLiteral("ȱ��վ��site�ļ�"));
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
	return sitesContainer->getSitePositionById(id);
}

void computeManager::transferContainerToPara()
{
	cptPara->Sites = sitesContainer->getSitesMap();
}

SitesContainer* computeManager::getContainer()
{
	return sitesContainer;
}

void computeManager::setPluginPath(QString path)
{
	pluginPath = path;
	sendPluginNewState();
}


QString computeManager::getPluginPath()
{
	return pluginPath;
}

PluginSubject * computeManager::getPluginSubject()
{
	return m_pluginSubject;
}

void computeManager::sendPluginNewState()
{
	if (pluginPath!=NULL)
	{
		QFileInfo f(pluginPath);
		if (f.isFile())
		{
			m_pluginSubject->item->setPluginName(f.baseName());
			m_pluginSubject->item->setFlag(true);
		}
		else
		{
			m_pluginSubject->item->setFlag(false);;
		}
		
	}
	else
	{
		m_pluginSubject->item->setFlag(false);
	}
	m_pluginSubject->notify();
}

void computeManager::openTransAntennas_DirGain(QStringList paths)
{
	vector<int> SiteIDs = sitesContainer->getIDs();
	for (int i = 0; i <SiteIDs.size(); i++)
	{
		//��i��վ�㣬��3��cell
		for (int j = 0; j <sitesContainer->getAntennasSize(SiteIDs[i]); j++)
		{
			//��ÿ��site��ÿ��cell����������ķ��������ļ����ҵ�ƥ��������ļ�
			string cell_name = sitesContainer->getSiteByID(SiteIDs[i])->Site_Antennas[j].Cell_Name + ".txt";
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
						sitesContainer->setInitialGain(SiteIDs[i], j, atof(str_flag.c_str()));
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
						sitesContainer->addDirectionGain(SiteIDs[i], j, antenna_property);
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
	//���� xyz  pci
	int x_id = -1, y_id = -1, z_id = -1, pci_id = -1;
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
		if (parameter1 == "PCI"||parameter1=="pci")
		{
			pci_id = count;
		}
		count++;
	}

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
		string str_x = Trim(parameters[x_id]);
		string str_y = Trim(parameters[y_id]);
		string str_z = Trim(parameters[z_id]);
		string PCI = Trim(parameters[pci_id]);
		double x = atof(str_x.c_str());
		double y = atof(str_y.c_str());
		double z = atof(str_z.c_str()) + glbctx->modelManager->getFirstCity()->getAltitude(x, y);
		no_simplaneReceiver receiver;
		receiver.position = Vector3d(x, y, z);
		receiver.PCI = atoi(PCI.c_str());
		cptPara->No_SimPlanePoint.push_back(receiver);
	}
	infile.close();
	noSimFlag = true;
}

void computeManager:: openTransAntenna_ParamFile(QString path)
{
	if (path.isEmpty())
		return;

	sitesContainer->clear();

	//׼����ȡ
	ifstream infile((path.toStdString()).c_str(), ios::in | ios::_Nocreate);
	if (!infile)
	{
		cout << "error:can not open file!" << endl;
		return;
	}

	globalContext *globalCtx = globalContext::GetInstance();
	string str, str_flag;
	getline(infile, str);
	//��ȡ���߲����ļ������������Ϣ��Ӧ�����е�ID��
	int sitename_id = -1, cellname_id = -1, frequency_id = -1, pci_id = -1, height_id = -1, x_id = -1, y_id = -1, power_id = -1, loss_id = -1;
	istringstream linestream1(str);
	string parameter1;
	int count = 0;
	while (getline(linestream1, parameter1, ','))
	{
		parameter1 = Trim(parameter1);
		if (parameter1 == "Site Name(*)")
		{
			sitename_id = count;
		}
		if (parameter1 == "Cell Name(*)")
		{
			cellname_id = count;
		}
		if (parameter1 == "Frequency Band(*)")
		{
			frequency_id = count;
		}
		if (parameter1 == "PCI")
		{
			pci_id = count;
		}
		if (parameter1 == "Height")
		{
			height_id = count;
		}
		if (parameter1 == "RS Power(dBm)" || parameter1 == "RS Power")
		{
			power_id = count;
		}
		if (parameter1 == "Total Loss(DL)")
		{
			loss_id = count;
		}
		if (parameter1 == "X" || parameter1 == "x")
		{
			x_id = count;
		}
		if (parameter1 == "Y" || parameter1 == "y")
		{
			y_id = count;
		}
		count++;
	}

	while (getline(infile, str))
	{
		istringstream linestream(str);
		vector<string>parameters;
		string parameter;
		while (getline(linestream, parameter, ','))
		{
			parameters.push_back(parameter);
		}
		string SiteName = ExtractNumber(Trim(parameters[sitename_id]));
		string CellName = Trim(parameters[cellname_id]);
		string PCI = Trim(parameters[pci_id]);
		string height = Trim(parameters[height_id]);
		string str_x = Trim(parameters[x_id]);
		string str_y = Trim(parameters[y_id]);
		double x = atof(str_x.c_str());
		double y = atof(str_y.c_str());
		//��ȫ���л�ȡ
		double z = atof(height.c_str()) + globalCtx->modelManager->getFirstCity()->getAltitude(x, y);

		string frequency_str;
		istringstream frequency_stream(parameters[frequency_id]);
		frequency_stream >> frequency_str;

		string frequency = Trim(frequency_str);
		string power_str = Trim(parameters[power_id]);
		string loss_str = Trim(parameters[loss_id]);

		TransAntenna new_antenna;
		new_antenna.Cell_Name = CellName;
		new_antenna.PCI = atoi(PCI.c_str());
		new_antenna.frequency = atof(frequency.c_str());  //��λΪMHZ
		new_antenna.trans_power = atof(power_str.c_str()); //��λΪdBm
		new_antenna.wire_loss = atof(loss_str.c_str());
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
		int current_sitename = stof(SiteName.c_str());
		sitesContainer->addAntenna(new_antenna, current_sitename);
		subject->AntennaItem->addSite(current_sitename, CellName);
	}
	infile.close();
	subject->notify();
	cout << "info:��ȡ�����ļ��ɹ�" << endl;
	siteFlag = true;
}

vector<Site*>  computeManager::getSite()
{
	return sitesContainer->getAllSites();
}