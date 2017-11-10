#ifndef ABSTRACT_MODEL_MANAGER_H
#define ABSTRACT_MODEL_MANAGER_H
#include "abstractModel.h"
#include <string>
class abstractModelFactory
{
public:
	abstractModelFactory();
	virtual ~abstractModelFactory ()= 0;

	virtual abstractModel * loadModel(std::string path)=0;

private:

};
#endif