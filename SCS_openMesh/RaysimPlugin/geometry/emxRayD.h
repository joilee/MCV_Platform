/**
* A ray is a directed line segment which is defined by a pair of 3D
* coordinates (or points). The first point specifies the starting point of the
* ray and the second point specifies the ending point of the ray, and the third 
* specifies its direction. It supports methods for finding the parameterized 
* distance along a ray. It records the last intersected face ID.
*
* An intersectionInfo is the intersected point's information, which is defined by 
* the intersected face ID, the intersected coordinate, and the normal of the 
* intersected point.
**/
#ifndef __EMXRAYD_H__
#define __EMXRAYD_H__

#include "util/emxUtilityInc.h"




class emxRay {
public:
	Vector3d origin,		// ray origin 
			 end,		    // ray destination 
			 direction;	    // unit vector, indicating ray direction
	int      lastID;		// last intersected face ID
	mutable double mint, maxt;
};

class emxIntersectionInfo {
public:
	int      faceID;		// the intersected face ID
	Vector3d iCoordinate;	// the intersected coordinate
	Vector3d inVec;
	Vector3d refVec;
};


#endif