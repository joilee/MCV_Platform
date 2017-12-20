#include "objModelFactory.h"

abstractGlobalModel  * objModelFactory::loadModel(string path)
{
	return (new objModel(path));
}


objModelFactory::objModelFactory()
{
	cout << "Info: Model Factory initialized. " << endl;
}


objModelFactory::~objModelFactory()
{

}

