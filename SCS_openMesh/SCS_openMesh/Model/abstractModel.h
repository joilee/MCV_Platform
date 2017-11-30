#ifndef ABSTRACT_MODEL
#define ABSTRACT_MODEL

#ifndef DEFAULT_MODEL_ID
#define DEFAULT_MODEL_ID 0
#endif 


#include <string>
#include <vector>
using namespace std;
class abstractModel
{
public:
	abstractModel();
	string name;
	int id;
	virtual ~abstractModel();

	void setID(int _id){ id = _id; }
	int getID(){ return id; }
	string getName(){ return name; };
	void setName(string _name){ name = _name; };
	void virtual draw(vector<bool> mode);
	void virtual initDraw();
private:

};



#endif