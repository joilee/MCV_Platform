#include "cityModelFactory.h"

void int2str(const int &int_temp, string &string_temp)
{
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();   //此处也可以用 stream>>string_temp  
}

abstractModel  * cityModelFactory::loadModel(string path)
{
	return (new cityModel(path));
}


cityModelFactory::cityModelFactory()
{
	cout << "Info: Model Factory initialized. " << endl;
}
abstractModel * cityModelFactory::loadModel(Vector3d center,int siteID, double range, cityModel * cModel)
{
	string a;
	int2str(siteID, a);
	return (new cityLocalModel(center, range, cModel->getCity(), a));
}

cityModelFactory::~cityModelFactory()
{

}

