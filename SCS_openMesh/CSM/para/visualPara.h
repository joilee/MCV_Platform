#ifndef VISPARA_H
#define VISPARA_H
#include "../util/EField.h"
#include "../util/computationEnum.h"
#include <map>
class visPara
{
public:
	visPara();
	~visPara();

	//存储仿真结果
	map<int,Site_Data *> simuResult;

	//存储计算类型
	ComputationEnum comEnum;


protected:

private:
};



#endif