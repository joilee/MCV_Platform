 /**
 * emxVertex class.
 **/
#ifndef __EMXVERTEX_H__
#define __EMXVERTEX_H__

#include "util/emxUtilityInc.h"

class emxVertex
{
public:
	// constructor
	emxVertex(double x, double y, double z);
	emxVertex(const emxVertex& vertex);

	// destructor
	~emxVertex();

	// get position
	Vector3d&	    GetPos()	   { return pos; }
	const Vector3d&	GetPos() const { return pos; }


	// get / set 'index'
	size_t GetIndex() const       { return vertexIndex;  }
	void   SetIndex(size_t index) { vertexIndex = index; }



private:
	bool NonObtuse(double x)  { return (0.<=x) && (x<=M_PI/2); }
	

private:
	Vector3d pos;				// position
	size_t vertexIndex;			// index used for export to file, be assign a value in emxModel::AddVertex when import from file
	static size_t VERTEXCOUNT;
};



#endif
