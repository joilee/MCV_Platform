#include "cityLocalModelFactory.h"

void int2str(const int &int_temp, string &string_temp)
{
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();   //此处也可以用 stream>>string_temp  
}


cityLocalModelFactory::cityLocalModelFactory()
{
	cout << "Info: Local Model Factory initialized. " << endl;
}

cityLocalModelFactory::~cityLocalModelFactory()
{

}

abstractLocalModel * cityLocalModelFactory::loadModel(Vector3d center, int siteID, double range, cityModel * cModel)
{
	string a;
	int2str(siteID, a);
	return (new cityLocalModel(center, range, cModel->getCity(), a));
}
