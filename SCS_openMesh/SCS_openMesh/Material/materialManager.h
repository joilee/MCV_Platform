#include <material/scsMaterial.h>

class scsMaterialManager
{
public:
	scsMaterialManager();
	~scsMaterialManager();

	/*
	@brief 从文件中添加材料，如果已经有，就删除以前的
	*/
	void addMatertial(string path);

	/*
	@brief 删除当前的材料
	*/
	inline void deleteMaterial(){ materialdatabase.clear(); }

	/*
	@brief 判断当前是否已经有材料信息存在
	*/
	inline bool materialExisted(){ return !materialdatabase.empty(); }

	/*
	@brief 根据材料的ID获得再数组中材料对应的编号
	@note 一种材料可能存在多个对应的id，只返回第一个。
	*/
	inline int getVectorIndexFromID(int id)
	{
		for (int i = 0; i < materialdatabase.size();i++)
		{
			if (materialdatabase[i].Id==id)
			{
				return i;
			}
		}
		return -1;
	}

	void setDefault(int i);
	int getDefaultMaterial();
	vector<scsMaterial> &getMaterialVector(){ return materialdatabase; }
	/*
	@brief 获得材料向量中，第i个元素的颜色
	*/
	inline Color getColor(int i){ return materialdatabase[i].color; }
private:
	vector<scsMaterial> materialdatabase;
	int material_ID;
};

