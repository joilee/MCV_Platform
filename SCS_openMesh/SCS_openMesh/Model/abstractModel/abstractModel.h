#ifndef ABSTRACT_MODEL
#define ABSTRACT_MODEL

#ifndef DEFAULT_MODEL_ID
#define DEFAULT_MODEL_ID 0
#endif 


#include <string>
#include <vector>
#include "Model/ModelType.h"
using namespace std;
class abstractModel
{
public:
	abstractModel();
	string name;
	int id;
	ModelType fileType;
	virtual ~abstractModel();

	void setID(int _id){ id = _id; }
	int getID(){ return id; }
	
	void setType(ModelType a){ fileType = a; }
	ModelType getType(){ return fileType; }
	string getName(){ return name; };
	void setName(string _name){ name = _name; };
	void virtual draw(vector<bool> mode,double alpha);
	void virtual initDraw();
private:

};



#endif