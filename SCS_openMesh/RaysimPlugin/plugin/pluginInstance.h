#ifndef PLUGININSTANCE_H
#define PLUGININSTANCE_H

#include "pluginInterface.h"
#include <QObject>

class PluginInstance : public QObject, public ComputeInterface
{
	Q_OBJECT
		Q_PLUGIN_METADATA ( IID QtPlugin_iid)  
		Q_INTERFACES(ComputeInterface)
public:
	PluginInstance();
	void runAlgorithm(ModelPara *modelParameter,ComputePara *cptPara,visPara *vPara=0);
};

#endif