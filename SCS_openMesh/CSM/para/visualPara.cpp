#include "visualPara.h"

visPara::visPara()
{
	simuResult.clear();
}

visPara::~visPara()
{
	simuResult.swap(map<int, Site_Data*>());
}