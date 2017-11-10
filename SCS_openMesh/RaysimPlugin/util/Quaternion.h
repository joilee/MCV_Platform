/**
 * Quaternion class.
 *
 * Quaternions are a mathematical entity, similar to complex numbers.
 * Each quaternion has 3 "imaginary" parts and one "real" part.
 * There are rules to add, subtract, multiply and divide quaternions
 * in a consistent manner.
 *
 **/
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Vector.h"
#include <math.h>
#include <assert.h>

//namespace emxUtility {

	struct Quat
	{
		static const double COSPI4;
		static const Quat ZERO, ONE, VIEWDEFAULT, VIEWXY, VIEWXZ, VIEWYZ;

		// create object
		Quat (double x_=0, double y_=0, double z_=0, double w_=0) : x(x_), y(y_), z(z_), w(w_) {}
		Quat (const Vector3d &v)                                  : x(v.x), y(v.y), z(v.z), w(0) {}

		// normSquared
		double normSquared () const { return x*x + y*y + z*z + w*w; }
		double norm () const { return sqrt(normSquared()); }
		Quat normalize () const { return (*this) / norm(); }

		// conjugate
		Quat conjugate () const { return Quat(-x, -y, -z, w); }

		// operators
		bool  operator== (const Quat &r) const { return x==r.x && y==r.y && z==r.z && w==r.w; }
		Quat  operator+  (const Quat &r) const { return Quat(x+r.x, y+r.y, z+r.z, w+r.w); }
		Quat& operator+= (const Quat &r)       { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
		Quat  operator-  () const              { return Quat(-x, -y, -z, -w); }
		Quat  operator-  (const Quat &r) const { return Quat(x-r.x, y-r.y, z-r.z, w-r.w); }
		Quat& operator-= (const Quat &r)       { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
		Quat  operator*  (double s) const      { return Quat(x*s, y*s, z*s, w*s); }
		Quat& operator*= (double s)            { x*=s; y*=s; z*=s; w*=s; return *this; }
		Quat  operator/  (double s) const      { assert(s); return (*this)* (1/s); }
		Quat& operator/= (double s)            { assert(s); return (*this)*=(1/s); }
		Quat  operator*  (const Quat &r) const;
		Quat& operator*= (const Quat &r);
		Quat  operator/  (const Quat &r) const { return (*this) *  r.conjugate() /  r.normSquared(); }
		Quat& operator/= (const Quat &r)       { return (*this) *= r.conjugate() /= r.normSquared(); }

		// components
		double x, y, z, w;
	};

	inline double Dot (const Quat &l, const Quat &r)
	{
		return l.x*r.x + l.y*r.y + l.z*r.z + l.w*r.w;
	}

//} // namespace emxUtility

#endif
