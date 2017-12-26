#include "pluginInstance.h"

#include "algo.h"
//#include "raysim.h"


PluginInstance::PluginInstance()
{
}



/************************************************************************/
/* 算法函数  动态加载                                                                                  */
/************************************************************************/
void PluginInstance::runAlgorithm(ModelPara *modelParameter,ComputePara *cptPara,visPara *vPara)
{
	
	algo * BTrun = new algo;
	BTrun->pluginAlgo(modelParameter, cptPara, vPara);
}