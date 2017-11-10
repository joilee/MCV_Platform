/**
 * ArcBall class.
 *
 * A UI to rotate a model in space, which has the
 * property that if you rotate an object from x to y, then from y
 * to x, you get back the original orientation.  This is
 * a result of using quaternions to represent rotations.
 **/
#ifndef __ARCBALL_H__
#define __ARCBALL_H__

//#include "util/apibase.h"
#include "Quaternion.h"
#include "Color.h"

//namespace emxUtility {

	typedef double HMatrix[4][4];
	typedef enum AxisSet {NoAxes, CameraAxes, BodyAxes, OtherAxes, NSets} AxisSet;

	class ArcBall
	{
	public:
		static Quat arcBallCoord(int x, int y, int width, int height);

		// create object
		ArcBall ();

		// set the center and size of the controller
		void place (const Quat &center, double radius);

		// incorporate new mouse position
		void mouse (const Quat &vNow);

		// choose a constraint set, or none
		void useSet (AxisSet axisSet);

		// begin/stop drawing arc for all drags combined
		void showResult () { show=true; }
		void hideResult () { show=false; }

		// using vDown, vNow, dragging, and axisSet, compute rotations
		void update ();

		// return rotation matrix defined by controller use
		void value (HMatrix& mNow);

		// drag sequence
		void beginDrag  ();
		void endDrag    ();

		// draw the controller with all its arcs
		void draw (double aspect);

		// color attributes
		Color rimColor, farColor, nearColor, dragColor, resColor;

		// set view
		void setViewXY();
		void setViewXZ();
		void setViewYZ();
		void setViewDefault();

	private:
		// draw all constraint arcs
		void drawConstraints ();
		void drawOuterRing ();

		// draw "rubber band" arc during dragging
		void drawDragArc ();

		// draw arc for result of all drags
		void drawResultArc ();

		// set Quat with 'quat'
		void setQuat(const Quat& quat);

		// private members
		Quat     center;
		double   radius;
		Quat     qNow, qDown, qDrag;
		Quat     vNow, vDown, vFrom, vTo, vrFrom, vrTo;
		HMatrix  mNow, mDown;
		bool     show, dragging;
		double   *(sets[NSets]);
		int      setSizes[NSets];
		AxisSet  axisSet;
		int      axisIndex;
	};

//} // namespace emxUtility
#endif
