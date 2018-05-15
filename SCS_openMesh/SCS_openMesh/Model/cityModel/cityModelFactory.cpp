#include "cityModelFactory.h"

abstractGlobalModel  * cityModelFactory::loadModel(string path)
{
	return (new cityModel(path));
}

abstractGlobalModel * cityModelFactory::loadModel(string name, vector<string> bPath, vector<string> hPath, string aPath)
{
	return (new cityModel(name, bPath, hPath, aPath));
}


cityModelFactory::cityModelFactory()
{
	cout << "Info: Model Factory initialized. " << endl;
}


cityModelFactory::~cityModelFactory()
{

}

