#ifndef ABSTRACT_MODEL
#define ABSTRACT_MODEL
#include <string>
#include <vector>
using namespace std;
class abstractModel
{
public:
	abstractModel();
	string name;
	virtual ~abstractModel();

	string getName(){ return name; };
	void setName(string _name){ name = _name; };
	void virtual draw(vector<bool> mode);
	void virtual initDraw();
private:

};



#endif