#include "objModelFactory.h"

abstractGlobalModel  * objModelFactory::loadModel(string path)
{
	return (new objModel(path));
}


objModelFactory::objModelFactory()
{
	cout << "Info: ObjModel Factory initialized. " << endl;
}


objModelFactory::~objModelFactory()
{

}

