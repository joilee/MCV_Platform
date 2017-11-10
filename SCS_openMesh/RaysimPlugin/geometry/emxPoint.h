#ifndef __EMXPOINT_H__
#define __EMXPOINT_H__

#include "util/vector.h"
#include "util/emxUtilityInc.h"

struct emxIntersection {  //���������ཻ�Ľ������ݽṹ
	size_t   faceID;
	Vector3d position;
	double trans_length;
	double two_point_length;
	emxIntersection() : faceID(0) {}
};


/*
* intersections of a reference point
*/
struct emxPoint {  //���յ�����ݽṹ�����յ�λ�ã����ߴ����������ཻ�Ľ�����Ŀ����������Ĵ洢��¼
	Vector3d position;
	size_t   number;           //���ߴ����������ཻ�Ľ�����Ŀ
	emxIntersection* pIntersection;   //��������Ĵ洢��¼
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
	Vector3d ExitPosition() const { assert(number > 0); return pIntersection[number - 1].position; }  //���ش������������һ���ཻ�Ľ���
};

#endif