#include "visualManager.h"
#include <QJsonDocument>  
#include <QJsonArray>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QJsonObject>  
#include "Context/context.h"
#include <QDebug>
#include <QProgressDialog>
VisualManager::VisualManager()
{
	m_SiteCellSubject = new SiteCellSubject;
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

EFieldContainer * VisualManager::getContainer()
{
	return visContainer;
}
