#ifndef ABSTRACT_MODEL
#define ABSTRACT_MODEL
#include <string>
#include <vector>
using namespace std;
class abstractModel
{
public:
	abstractModel();
	virtual ~abstractModel();

	virtual string  getName();
	void virtual draw(vector<bool> mode);
	void virtual initDraw();
private:

};



#endif