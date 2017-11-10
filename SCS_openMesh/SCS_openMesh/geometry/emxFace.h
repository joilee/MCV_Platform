/**
* emxFace class.
**/
#ifndef __EMXFACE_H__
#define __EMXFACE_H__

#include "geometry/emxRayD.h"
#include "util/emxUtilityInc.h"


class emxVertex;


class emxFace
{
public:
	// constructor / destructor
	emxFace(emxVertex* v0, emxVertex* v1, emxVertex* v2);
	~emxFace();

	// vertex number of the face
	size_t Size() const  { return SIZE; }
	
	// vertex access
	emxVertex*       GetVertex(int i)           { return vertex[Mod(i, (int)SIZE)];    }	// -1 --> 2
	emxVertex*       GetVertex(size_t i)        { return vertex[Mod(i, (size_t)SIZE)]; }
	const emxVertex* GetVertex(int i)    const  { return vertex[Mod(i, (int)SIZE)];    }	// -1 --> 2
	const emxVertex* GetVertex(size_t i) const  { return vertex[Mod(i, (size_t)SIZE)]; }


	// get the center point position of the face
	Vector3d GetCenter() const;

	// get the normal vector(normalized) of the face
	Vector3d GetNormal() const;


	// ray triangle intersect test
	bool IntersectP(const emxRay& ray);

	// ray triangle intersect
	bool Intersect(const emxRay& ray, double& tHit);

    //ray triangle intersect with calculate curvatures
	bool Intersect(const emxRay& ray, double& tHit, emxIntersectionInfo& iInfo);


	// if the 3 vertexes of this face are on the same line, return false
	bool IsValid() const;
	static int FACECOUNT;

	// used for debug information in 'emxMeshSimplify'
	int index;


	//// the area of the face
	double GetArea();

	enum { SIZE = 3 };			// vertex number of the face
	emxVertex* vertex[SIZE];	// vertices

private:
	mutable Vector3d normal;	// the normal of this face
	mutable bool normalValid;	// indicate whether 'normal' is valid

};



#endif

