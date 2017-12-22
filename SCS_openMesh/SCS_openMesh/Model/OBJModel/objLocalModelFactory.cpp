#include "objLocalModelFactory.h"


objLocalModelFactory::objLocalModelFactory()
{
	cout << "Info: ObjLocalModel Factory initialized. " << endl;
}

objLocalModelFactory::~objLocalModelFactory()
{

}

abstractLocalModel * objLocalModelFactory::loadModel(Vector3d center, double localRange, string _name, objModel* cModel)
{
	return (new objLocalModel(center, localRange, _name, cModel));
}
