/*
 *	emx Utilities include file
 */
#ifndef __EMXUTILITYINC_H__
#define __EMXUTILITYINC_H__

#include <complex>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#define _USE_MATH_DEFINES
//#include <cmath>
#include <ctime>
#include <cfloat>
#include <malloc.h>
#include<cassert>
#include <math.h>
#include "vector.h"
using namespace std;

#pragma warning( disable  : 4996 )



static const double M_PI_180       = M_PI / 180;
static const double M_180_PI       = 180 / M_PI;
//static const float INFINITY       = FLT_MAX;
//static const double RCS_DOUBLEZERO = 1e-10;
static const string ROOTTEXT       = "3D Objects";
static const string DEFAULTTEXT	   = "Default";
//static const double DOUBLE_EPSILON = DBL_EPSILON;
//static const double DOUBLE_EPSILON = 1e-10;
static const double DOUBLE_EPSILON = 1e-7;
// const type size
static const unsigned int INTSIZE     = sizeof(int);
//static const unsigned int LONGSIZE    = sizeof(long);
static const unsigned int SIZETSIZE   = sizeof(size_t);
static const unsigned int DOUBLESIZE  = sizeof(double);
static const unsigned int MAXSHORTINT = 65535;

// unary functions
//inline bool UnitValue (double x)  { return (0.<=x) && (x<=1.); }
inline bool UnitValue (double x)  { return (-DOUBLE_EPSILON<=x) && (x<=1+DOUBLE_EPSILON); }
inline bool IsPowerOf2(int v)     { return (v & (v - 1)) == 0; }
inline int Log2Int(float v)       { return ((*(int *) &v) >> 23) - 127; }

// square and cube, to go along with sqrt and cbrt
template <class T>
inline T sqr(const T &x)  { return x*x;   }
template <class T>
inline T cube(const T &x) { return x*x*x; }

// sign of a scalar ·ûºÅº¯Êý
template <class T>
inline T sgn(const T &x) { return (x < T(0)) ? T(-1) : T(1); }

// 1/tan(v)
inline double cot(double v) 
{
	if(fabs(tan(v)) < DOUBLE_EPSILON)
		assert(0);
	return 1.0 / tan(v);
}

// binary functions
template<class T>
inline T Max(T x, T y) { return x>=y ? x :  y; }
template<class T>
inline T Min(T x, T y) { return x<=y ? x :  y; }
template<class T>
inline T Mod(T x, T y) { T t = x % y; return t >= 0 ? t : y + t; }
inline bool NoCaseCompare(char c1, char c2) { return toupper(c1) == toupper(c2); }

// ternary functions
template<class T>
inline T Max  (T x, T y, T z) { return Max(Max(x, y), z); }
template<class T>
inline T Min  (T x, T y, T z) { return Min(Min(x, y), z); }
template <class T> 
inline T Clamp(T val, T low, T high) { return Min(Max(val, low), high); }


// Platform-specific definitions
#if defined(WIN32)
#define memalign(a,b) _aligned_malloc(b, a)
#elif defined(__APPLE__)
#define memalign(a,b) valloc(b)
#endif

// Memory Allocation Functions
inline void *AllocAligned(size_t size) 
{
#ifndef L1_CACHE_LINE_SIZE
#define L1_CACHE_LINE_SIZE 64
#endif
	return memalign(L1_CACHE_LINE_SIZE, size);
}
inline void FreeAligned(void *ptr) 
{
#ifdef WIN32
	_aligned_free(ptr);
#else
	free(ptr);
#endif
}

// numeral format conversion
#if (defined(__linux__) && defined(__i386__)) || defined(WIN32)
#define FAST_INT 1
#endif
#define _doublemagicroundeps	      (.5-1.4e-11)
//almost .5f = .5f - 1e^(number of exp bit)
inline int Round2Int(double val) 
{
#ifdef FAST_INT
#define _doublemagic			double (6755399441055744.0)
	//2^52 * 1.5,  uses limited precision to floor
	val		= val + _doublemagic;
	return ((long*)&val)[0];
#else
	return int (val+_doublemagicroundeps);
#endif
}
inline int Float2Int(double val) 
{
#ifdef FAST_INT
	return (val<0) ?  Round2Int(val+_doublemagicroundeps) :
	Round2Int(val-_doublemagicroundeps);
#else
	return (int)val;
#endif
}
inline int Floor2Int(double val) 
{
#ifdef FAST_INT
	return Round2Int(val - _doublemagicroundeps);
#else
	return (int)floorf(val);
#endif
}
inline int Ceil2Int(double val) 
{
#ifdef FAST_INT
	return Round2Int(val + _doublemagicroundeps);
#else
	return (int)ceilf(val);
#endif
}

#endif