#include "visualPluginItem.h"

VisualPluginItem::VisualPluginItem()
{

}

VisualPluginItem::~VisualPluginItem()
{

}

void VisualPluginItem::setPluginName(QString a)
{
	pluginName = a;
}

void VisualPluginItem::setFlag(bool a)
{
	existFlag = a;
}

bool VisualPluginItem::itemExist()
{
	return existFlag;
}

QString VisualPluginItem::getPluginName()
{
	return pluginName;
}
