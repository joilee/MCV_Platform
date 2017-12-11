#include "visualManager.h"


VisualManager::VisualManager()
{
}


VisualManager::~VisualManager()
{

}

void VisualManager::setContainerData(visPara * a)
{
	map<int, Site_Data *> &  visRes = a->simuResult;
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
			newCell->cell_name = cell->cell_name;

			//遍历每一个仿真点
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

}
