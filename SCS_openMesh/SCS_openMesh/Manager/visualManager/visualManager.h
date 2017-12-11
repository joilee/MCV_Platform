#pragma once
#include <para/visualPara.h>
#include <Container/EFieldContainer.h>
#include <QString>
/*
可视化管理
1.散点图展示
2.结果保存（待添加）
3.仿真面展示（待添加）
*/

class VisualManager
{
public:
	VisualManager();
	~VisualManager();


	/**
	  * @Method:    setContainerData
	  * @Note: 	将仿真结果存储到结果容器，但是并不会更改容器中（已经导入的）实测数据。
	  * @Author:    Li Gen
	  * @ Date:     2017/12/10
	  * @Returns:   void
	  * @Parameter: visPara * a
	*/
	void setContainerData(visPara * a);

private:
	EFieldContainer * visContainer;
};

