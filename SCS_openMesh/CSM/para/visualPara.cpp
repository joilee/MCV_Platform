#include "visualPara.h"

visPara::visPara()
{
	
}

visPara::~visPara()
{
	simuResult.swap(map<int, Site_Data*>());
}