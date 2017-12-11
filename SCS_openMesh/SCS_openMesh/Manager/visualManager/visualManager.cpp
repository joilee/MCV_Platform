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
			newCell->cell_name = cell->cell_name;

			//����ÿһ�������
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

}
