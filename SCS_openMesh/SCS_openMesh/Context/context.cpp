#include "context.h"


globalContext::globalContext()
{
	modelManager = new ModelManager;
	cptManager = new computeManager;
	visualManager = new VisualManager;
}

globalContext::~globalContext()
{
	

	 delete cptManager;
}