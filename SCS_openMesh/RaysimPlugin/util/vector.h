/**
 * Vector class.
 * Common mathematical operations on vectors in R3.
 *
 **/
#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <iostream>
#include <cassert>
#include <cmath>
#include <complex>
using namespace std;
using std::complex;

//namespace emxUtility {

/**
 * Vector3
 **/
template <class T>
struct Vector3
{
  // create a vector
  Vector3 (T x_=0, T y_=0, T z_=0) : x(x_), y(y_), z(z_) {}
  Vector3 (const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}

  // set coordinates
  void set (T x_, T y_, T z_) { x=x_; y=y_; z=z_; }

  // norm
  Vector3 normalize   () const { return (*this) / norm();    }
  double  norm        () const { return sqrt(normSquared()); }
  T       normSquared () const { return x*x+y*y+z*z;         }

  // assign operator
  Vector3& operator = (const Vector3& v) { x=v.x; y=v.y; z=v.z; return *this; }

  // boolean operators
  bool operator == (const Vector3& v) const { return x==v.x && y==v.y && z==v.z; }
  bool operator != (const Vector3& v) const { return x!=v.x || y!=v.y || z!=v.z; }

  // operators
  Vector3  operator +  (const Vector3 &v) const { return Vector3(x+v.x, y+v.y, z+v.z);  }
  Vector3& operator += (const Vector3 &v)       { x+=v.x; y+=v.y; z+=v.z; return *this; }
  Vector3  operator -  () const                 { return Vector3(-x, -y, -z);           }
  Vector3  operator -  (const Vector3 &v) const { return Vector3(x-v.x, y-v.y, z-v.z);  }
  Vector3& operator -= (const Vector3 &v)       { x-=v.x; y-=v.y; z-=v.z; return *this; }
  Vector3  operator *  (T s) const              { return Vector3(x*s, y*s, z*s);        }
  Vector3& operator *= (double s)               { x*=s; y*=s; z*=s; return *this;       }
  Vector3  operator /  (double s) const         { assert(s); return (*this)* (1/s);     }
  Vector3& operator /= (double s)               { assert(s); return (*this)*=(1/s);     }

  // array reference
  const T &operator [] (size_t i) const;
  T &operator [] (size_t i);

  // coordinates
  T x, y, z;
};

// Vector functions
template <class T> inline
	Vector3<T> Min(const Vector3<T> &l, const Vector3<T> &r)
{
	return Vector3<T>(Min(l.x,r.x), Min(l.y,r.y), Min(l.z,r.z));
}

template <class T> inline
	Vector3<T> Max(const Vector3<T> &l, const Vector3<T> &r)
{
	return Vector3<T>(Max(l.x,r.x), Max(l.y,r.y), Max(l.z,r.z));
}

// array reference - with bounds checking
template<class T> inline const T & Vector3<T>::operator [] (size_t i) const
{
	assert(i >= 0 && i < 3);
	switch(i) {
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	default:
		assert(false);
	}
	// error detection
	return x;
}

template<class T> T & Vector3<T>::operator [] (size_t i)
{
	assert(i >= 0 && i < 3);
	switch(i) {
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	default:
		assert(false);
	}
	// error detection
	return x;
}

// support scalar * Vector3<T>
template <class T> inline 
const Vector3<T> operator * (const T &s, const Vector3<T> &v)
{
	return Vector3<T>(v.x*s, v.y*s, v.z*s);
}
inline const Vector3<complex<double>> operator * (const double &s, const Vector3<complex<double>> &v)
{
	return Vector3<complex<double>>(v.x*s, v.y*s, v.z*s);
}

inline const Vector3<complex<double>> operator * (const Vector3<complex<double>> &v,const double &s)
{
	return Vector3<complex<double>>(v.x*s, v.y*s, v.z*s);
}

inline Vector3<complex<double>> operator *(const complex<double> &l, const Vector3<double> &r)
{
	return Vector3<complex<double>>(l*r.x, l*r.y, l*r.z);
}

inline Vector3<complex<double>> operator *(const Vector3<double> &r,const complex<double> &l)
{
	return Vector3<complex<double>>(l*r.x, l*r.y, l*r.z);
}
// dot product
template <class T> inline
T Dot (const Vector3<T>& l, const Vector3<T>& r)
{
  return l.x*r.x + l.y*r.y + l.z*r.z;
}

template<class T, class U>
inline T Dot(const Vector3<T> &l, const Vector3<U> &r)
{
	return l.x*r.x + l.y*r.y + l.z*r.z;
}

// VectorCross product
template <class T> inline
Vector3<T> VectorCross (const Vector3<T>& l, const Vector3<T>& r)
{
  return Vector3<T>(
    l.y*r.z - l.z*r.y,
    l.z*r.x - l.x*r.z,
    l.x*r.y - l.y*r.x );
}

// blend_product
template <class T> inline
T BlendProduct (const Vector3<T>& l, const Vector3<T>& m, const Vector3<T>& r)
{
  return Dot(VectorCross(l, m), r);
}

// ouput 
template <class T> inline
ostream& operator << (ostream &out, const Vector3<T> &r)
{
  return out << '[' << r.x << ',' << r.y << ',' << r.z << ']';
}

/**
 * types
 **/
typedef Vector3<double> Vector3d;
typedef Vector3<float>	Vector3f;
typedef Vector3<int> Vector3i;
typedef Vector3<complex<double>> Vector3cd;
//} // namespace emxUtility

#endif
