/**
* Bounding Box class.
* Common mathematical operations on bounding boxes
*
**/
#ifndef __EMXBBOXD_H__
#define __EMXBBOXD_H__

#include "util/emxUtilityInc.h"
#include "geometry/emxRayD.h"


class emxFace;

// emxMailboxPrim Declarations
class emxMailboxPrim
{
public:
	emxMailboxPrim(const int faceID, emxFace *face) {
		this->faceID  = faceID;
		this->face    = face;
		lastMailboxId = -1;
	}
	int faceID;
	emxFace *face;
	int lastMailboxId;
};

class emxBBox {
public:
	// emxBBox Public Methods
	emxBBox() {
		pMin = Vector3d( INFINITY,  INFINITY,  INFINITY);
		pMax = Vector3d(-INFINITY, -INFINITY, -INFINITY);
	}
	emxBBox(const Vector3d &p) : pMin(p), pMax(p) { }
	emxBBox(const Vector3d &p1, const Vector3d &p2) {
		pMin = Min(p1, p2);
		pMax = Max(p1, p2);
	}
	emxBBox(const Vector3d &p1, const Vector3d &p2, const Vector3d &p3) {
		pMin = Min(p1, Min(p2, p3));
		pMax = Max(p1, Max(p2, p3));
	}
		
	friend inline ostream & operator<<(ostream &os, const emxBBox &b);
	friend  emxBBox Union(const emxBBox &b, const Vector3d &p);
	friend  emxBBox Union(const emxBBox &b, const emxBBox &b2);
		
	bool Overlaps(const emxBBox &b) const {
		bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
		bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
		bool z = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
		return (x && y && z);
	}
		
	bool Inside(const Vector3d &pt) const {
		return (pt.x >= pMin.x && pt.x <= pMax.x &&
				pt.y >= pMin.y && pt.y <= pMax.y &&
				pt.z >= pMin.z && pt.z <= pMax.z);
	}
		
	void Expand(double delta) {
		pMin -= Vector3d(delta, delta, delta);
		pMax += Vector3d(delta, delta, delta);
	}
		
	double Volume() const {
		Vector3d d = pMax - pMin;
		return d.x * d.y * d.z;
	}
		
	int MaximumExtent() const {
		Vector3d diag = pMax - pMin;
		if (diag.x > diag.y && diag.x > diag.z)
			return 0;
		else if (diag.y > diag.z)
			return 1;
		else
			return 2;
	}

	Vector3d CornerPoint(size_t i) const {
		switch(i % 8) {
			case 0: return Vector3d(pMin.x, pMin.y, pMin.z);
			case 1: return Vector3d(pMax.x, pMin.y, pMin.z);
			case 2: return Vector3d(pMax.x, pMax.y, pMin.z);
			case 3: return Vector3d(pMin.x, pMax.y, pMin.z);
			case 4: return Vector3d(pMin.x, pMin.y, pMax.z);
			case 5: return Vector3d(pMax.x, pMin.y, pMax.z);
			case 6: return Vector3d(pMax.x, pMax.y, pMax.z);
			case 7: return Vector3d(pMin.x, pMax.y, pMax.z);
		}
		return pMin;
	}

	bool IntersectP(const emxRay &ray, double *hitt0 = NULL, double *hitt1 = NULL) const;

	// BBox Public Data
	Vector3d pMin, pMax;	// minimun and maximun of point
};

inline ostream &operator<<(ostream &os, const emxBBox &b) 
{
	os << b.pMin << " -> " << b.pMax;
	return os;
}



#endif
