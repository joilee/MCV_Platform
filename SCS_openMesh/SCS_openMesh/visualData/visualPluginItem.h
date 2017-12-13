#pragma once

#include <QString>
using namespace std;
class VisualPluginItem
{
public:
	VisualPluginItem();
	~VisualPluginItem();
	void setPluginName(QString a);
	void setFlag(bool a);
	bool itemExist();
	QString getPluginName();
private:
	QString pluginName;
	bool existFlag;
};
