#include "cityModelFactory.h"
abstractModel  * cityModelFactory::loadModel(string path)
{
	return (new cityModel(path));
}


cityModelFactory::cityModelFactory()
{
	cout << "Info: Model Factory initialized. " << endl;
}
abstractModel * cityModelFactory::loadModel(Vector3d center, double range, cityModel * cModel)
{
	return (new cityLocalModel(center, range, cModel->getCity(), cModel->getName()));
}

cityModelFactory::~cityModelFactory()
{

}