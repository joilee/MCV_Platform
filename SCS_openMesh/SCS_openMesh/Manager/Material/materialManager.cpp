#include "materialManager.h"
#include <fstream>
void scsMaterialManager:: addMatertial(string path)
{
	if (materialExisted())
	{
		deleteMaterial();
	}
	ifstream infile((path).c_str(), ios::in | ios::_Nocreate);
	if (!infile)
	{
		return;
	}
	string str;
	getline(infile, str);
	string unit;
	string sub_name; //����String���󣬵���Ĭ�Ϲ��캯����ʼ��Ϊ���ַ���
	while (getline(infile, str))
	{
		scsMaterial new_material;
		istringstream linestream(str);
		linestream >> new_material.Id;
		linestream >> sub_name;
		while (sub_name != "thickness:")
		{
			sub_name = " " + sub_name;
			new_material.name += sub_name;
			linestream >> sub_name;
		}
		linestream >> new_material.thickness >> unit >> new_material.color.r >> new_material.color.g >> new_material.color.b >> new_material.frequency >> new_material.dielectric >> new_material.permeability >> new_material.conductivity >> new_material.Transmission_Loss >> new_material.Reflection_Loss;
		if (unit == "m.\"")
		{
		}
		else if (unit == "cm.\"")
		{
			new_material.thickness /= 100;
		}
		else if (unit == "mm.\"")
		{
			new_material.thickness /= 1000;
		}
		materialdatabase.push_back(new_material);
		
	}
	infile.close();
	cout << "success: �ɹ���Ӳ����ļ���" << endl;
}
scsMaterialManager::scsMaterialManager()
{
	material_ID = 10;//Ĭ���ǻ������ļ�
}

scsMaterialManager::~scsMaterialManager()
{

}

int  scsMaterialManager::getDefaultMaterial(){
	return material_ID;
}

void scsMaterialManager:: setDefault(int i)
{
	if (i>=0&&i<materialdatabase.size())
	{
		material_ID = i;
	}
	cout << "info: ����Ĭ�ϲ��ϱ���� i" << endl;
}