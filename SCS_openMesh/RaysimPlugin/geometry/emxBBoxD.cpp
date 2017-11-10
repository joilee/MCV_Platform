#include "geometry/emxBBoxD.h"
#include "geometry/emxRayD.h"



emxBBox Union(const emxBBox &b, const Vector3d &p) 
{
	emxBBox bbox;
	bbox.pMax = Max(b.pMax, p);
	bbox.pMin = Min(b.pMin, p);
	return bbox;
}

emxBBox Union(const emxBBox &b, const emxBBox &b2) 
{
	emxBBox bbox;
	bbox.pMax = Max(b.pMax, b2.pMax);
	bbox.pMin = Min(b.pMin, b2.pMin);
	return bbox;
}

bool emxBBox::IntersectP(const emxRay &ray, double *hitt0, double *hitt1) const
{
	double t0 = -INFINITY, t1 = INFINITY;
	for (int i = 0; i < 3; ++i) {
		if(fabs(ray.direction[i]) > 0/*DOUBLE_EPSILON*/) {
			// Update interval for _i_th bounding box slab
			double invRayDir = 1.0 / ray.direction[i];
			double tNear = (pMin[i] - ray.origin[i]) * invRayDir;
			double tFar  = (pMax[i] - ray.origin[i]) * invRayDir;
			// Update parametric interval from slab intersection $t$s
			if (tNear > tFar) swap(tNear, tFar);
			t0 = tNear > t0 ? tNear : t0;
			t1 = tFar  < t1 ? tFar  : t1;
			if (t0 > t1) return false;
		}
		else if(ray.origin[i] < pMin[i] || ray.origin[i] > pMax[i]) {
			// if direction Xd = 0 then the ray is parallel to the X planes, so
			//	if origin Xo is not between the slabs ( Xo < Xl or Xo > Xh) then return false
			return false;
		}
	}
	if(hitt0 != NULL) *hitt0 = t0;
	if(hitt1 != NULL) *hitt1 = t1;
	return true;
}

