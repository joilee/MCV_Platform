#include "emxVertex.h"



/*
 *	constructor
 */ 
emxVertex::emxVertex(double x, double y, double z)
    : pos(x, y, z), vertexIndex(0)
{
}


emxVertex::emxVertex(const emxVertex& vertex)
	: pos(vertex.GetPos()), vertexIndex(0)
{
}

/*
 *	destructor
 */
emxVertex::~emxVertex()
{
}


