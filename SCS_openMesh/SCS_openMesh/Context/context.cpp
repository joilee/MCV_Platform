#include "context.h"


globalContext::globalContext()
{

	visualPara=new visPara;
	modelManager = new ModelManager;
	cptManager = new computeManager;
	
}

globalContext::~globalContext()
{
	

	 delete cptManager;
}