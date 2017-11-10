#ifndef __EMXPOINT_H__
#define __EMXPOINT_H__

#include "util/vector.h"
#include "util/emxUtilityInc.h"

struct emxIntersection {  //传播过程相交的交点数据结构
	size_t   faceID;
	Vector3d position;
	double trans_length;
	double two_point_length;
	emxIntersection() : faceID(0) {}
};


/*
* intersections of a reference point
*/
struct emxPoint {  //接收点的数据结构：接收点位置，射线传播过程中相交的交点数目，各个交点的存储记录
	Vector3d position;
	size_t   number;           //射线传播过程中相交的交点数目
	emxIntersection* pIntersection;   //各个交点的存储记录
	emxPoint() : number(0), pIntersection(NULL) {}
	~emxPoint() { delete []pIntersection; }

	// init the size of intersection
	void InitSize(size_t size) {
		position = Vector3d(INFINITY, INFINITY, INFINITY);
		number = 0;
		if(pIntersection != NULL) {
			delete []pIntersection;
			pIntersection = NULL;
		}
		pIntersection = new emxIntersection[size];
	}
	// assignment operator
	emxPoint& operator = (const emxPoint& point) {
		if(&point == this) return *this;
		position = point.position;
		number   = point.number;
		// assume the intersection has enough capacity
		if(number > 0)
			memcpy(pIntersection, point.pIntersection, sizeof(emxIntersection) * number);
		return *this;
	}
	// equal operator
	bool operator == (const emxPoint& point) const { 
		bool same = (number == point.number);
		for(size_t i = 0; same && i < number; ++i)
			same = (pIntersection[i].faceID == point.pIntersection[i].faceID);
		return same;
	}
	// exit position
	Vector3d ExitPosition() const { assert(number > 0); return pIntersection[number - 1].position; }  //返回传播过程中最后一次相交的交点
};

#endif