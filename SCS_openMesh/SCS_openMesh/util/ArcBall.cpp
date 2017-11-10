
#include "ArcBall.h"
#include "emxRenderUtility.h"


//namespace emxUtility {

	static const int
		LOG_NUM_SEGS = 4,
		NUM_SEGS     = 1 << 4;

	static HMatrix mID = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 } };

		//static HMatrix mID = {
		//	{ 0.824087, 0.16567, -0.541695, 0 },
		//	{ -0.563556, 0.336534, -0.75442, 0 },
		//	{ 0.0573136, 0.926984, 0.370697, 0 },
		//	{ 0, 0, 0, 1 } };

		static double OTHER_AXIS[][4] = { {-0.48, 0.80, 0.36, 1} };

		enum QuatPart {X, Y, Z, W};

		// auxilory functions
		static void getRotation  (const Quat &q, HMatrix& out);
		static Quat cross        (const Quat &q1, const Quat &q2);
		static Quat bisect       (const Quat &v1, const Quat &v2);
		static void circ         ();
		static void drawAnyArc   (const Quat &vFrom, const Quat &vTo);
		static void drawHalfArc  (const Quat &n);


		// ball math
		static Quat mouseOnSphere         (const Quat &mouse, const Quat &ballCenter, double ballRadius);
		static Quat constrainToAxis       (const Quat &loose, const Quat &axis);
		static int  nearestConstraintAxis (const Quat &loose, const Quat *axes, int nAxes);
		static Quat fromBallPoints        (const Quat &from,  const Quat &to);
		static void toBallPoints          (const Quat &q,     Quat &arcFrom, Quat &arcTo);


		/**
		* ArcBall
		**/
		// static
		Quat ArcBall::arcBallCoord (int x, int y, int width, int height)
		{
			double
				scaleX = (width>height) ? ((double)width/height) : 1,
				scaleY = (height>width) ? ((double)height/width) : 1;

			return Quat(
				((double)x/width -.5f) * 2*scaleX,
				-((double)y/height-.5f) * 2*scaleY,
				0 );
		}

		// create object
		ArcBall::ArcBall () :
		rimColor  (Color::WHITE),
			farColor  (Color::CYAN ),
			nearColor (Color::CYAN ),
			dragColor (Color::CYAN ),
			resColor  (Color::CYAN ),

			center     (Quat::ONE),
			radius     (1),

			qNow       (Quat::ONE),
			qDown      (Quat::ONE),

			vNow       (Quat::ONE),
			vDown      (Quat::ONE),
			show       (false),
			dragging   (false),
			axisSet    (NoAxes)
		{
			memcpy(mNow,  mID, sizeof(HMatrix));
			memcpy(mDown, mID, sizeof(HMatrix));

			sets[CameraAxes] = mID[X];
			setSizes[CameraAxes] = 3;

			sets[BodyAxes] = mDown[X];
			setSizes[BodyAxes] = 3;

			sets[OtherAxes] = OTHER_AXIS[X];
			setSizes[OtherAxes] = 1;
		}

		// set the center and size of the controller
		void ArcBall::place (const Quat &center_, double radius_)
		{
			center = center_;
			radius = radius_;
		}

		// incorporate new mouse position
		void ArcBall::mouse (const Quat &vNow_)
		{ vNow = vNow_; }

		// choose a constraint set, or none
		void ArcBall::useSet (AxisSet axisSet_)
		{
			if(!dragging)
				axisSet = axisSet_;
		}

		// using vDown, vNow, dragging, and axisSet, compute rotations
		void ArcBall::update ()
		{
			int setSize = setSizes[axisSet];

			Quat *set = (Quat *)(sets[axisSet]);

			vFrom = mouseOnSphere(vDown, center, radius);
			vTo   = mouseOnSphere(vNow, center, radius);

			if(dragging)
			{
				if(axisSet != NoAxes)
				{
					vFrom = constrainToAxis(vFrom, set[axisIndex]);
					vTo   = constrainToAxis(vTo, set[axisIndex]);
				}
				qDrag = fromBallPoints(vFrom, vTo);
				qNow = qDrag * qDown;
			}
			else if(axisSet != NoAxes)
				axisIndex = nearestConstraintAxis(vTo, set, setSize);

			toBallPoints(qDown, vrFrom, vrTo);
			getRotation(qNow.conjugate(), mNow); // gives transpose to OpenGL
		}

		// return rotation matrix defined by controller use
		void ArcBall::value (HMatrix& mNow_)
		{ memcpy(mNow_, mNow, sizeof(HMatrix)); }

		// drag sequence
		void ArcBall::beginDrag ()
		{
			dragging = true;
			vDown = vNow;
		}

		void ArcBall::endDrag ()
		{
			dragging = false;
			qDown = qNow;
			//cout<<qDown.x<<" "<<qDown.y<<" "<<qDown.z<<" "<<qDown.w<<endl;
			memcpy(mDown, mNow, sizeof(HMatrix));
		}

		// draw the controller with all its arcs
		void ArcBall::draw (double aspect)
		{
			if(dragging)
				drawOuterRing();  // draw edges

			drawResultArc();
			drawConstraints();
			drawDragArc();
		}

		// draw all constraint arcs
		void ArcBall::drawConstraints ()
		{
			if(axisSet == NoAxes)
				return;

			int setSize = setSizes[axisSet];
			double *set = sets[axisSet];

			Quat axis;
			for(int axisI=0; axisI<setSize; ++axisI)
			{
				if(axisIndex != axisI)
				{
					if(dragging) continue;
					LoadColor(farColor);
				}
				else
					LoadColor(nearColor);

				axis = *(Quat *)&set[4*axisI];

				if(axis.z == 1)
					circ();
				else
					drawHalfArc(axis);
			}
		}

		void ArcBall::drawOuterRing ()
		{
			LoadColor(rimColor);
			circ();
		}

		// draw "rubber band" arc during dragging
		void ArcBall::drawDragArc ()
		{
			LoadColor(dragColor);
			if(dragging)
				drawAnyArc(vFrom, vTo);
		}

		// draw arc for result of all drags
		void ArcBall::drawResultArc ()
		{
			LoadColor(resColor);
			if(show)
				drawAnyArc(vrFrom, vrTo);
		}

		// set Quat with 'quat'
		void ArcBall::setQuat(const Quat& quat)
		{
			qNow = quat;
			qDown = quat;
		}

		// set view
		void ArcBall::setViewXY()
		{
			setQuat(Quat::VIEWXY);
		}

		void ArcBall::setViewXZ()
		{
			setQuat(Quat::VIEWXZ);
		}

		void ArcBall::setViewYZ()
		{
			setQuat(Quat::VIEWYZ);
		}

		void ArcBall::setViewDefault()
		{
			setQuat(Quat::VIEWDEFAULT);
		}


		/**
		* auxilory functions
		*/
		// construct rotation matrix from (possibly non-normalize) quaternion.
		static
			void getRotation (const Quat &q, HMatrix& out)
		{
			double n = q.normSquared();

			Quat
				s = q * (n ? (2.f / n) : 0),
				x = s * q.x,
				y = s * q.y,
				z = s * q.z,
				w = s * q.w;

			double a[4][4] = {
				{ 1-(y.y+z.z),    x.y-w.z ,    x.z+w.y , 0 },
				{    x.y+w.z , 1-(x.x+z.z),    y.z-w.x , 0 },
				{    x.z-w.y ,    y.z+w.x , 1-(x.x+y.y), 0 },
				{          0 ,          0 ,          0 , 1 } };

				memcpy(out, a, sizeof(HMatrix));
		}

		// treat quaternions as vectors and take cross product
		static
			Quat cross (const Quat &q1, const Quat &q2)
		{
			assert(!q1.w && !q2.w);

			Vector3d
				v1 (q1.x, q1.y, q1.z),
				v2 (q2.x, q2.y, q2.z);

			return Quat(VectorCross(v1,v2));
		}

		// find normalize bisection of two quaternions
		static
			Quat bisect (const Quat &v1, const Quat &v2)
		{
			assert(!v1.w && !v2.w);

			Quat v = v1 + v2;
			double n = v.normSquared();

			if (n < 1.0e-5)
				return Quat(0, 0, 1);
			else
				return v * (1.0 / sqrt(n));
		}

		// draw circle?
		static
			void circ ()
		{
			Quat p(1,0,0,0), m(0,1,0,0);

			drawAnyArc( p,  m);
			drawAnyArc( m, -p);
			drawAnyArc(-p, -m);
			drawAnyArc(-m,  p);
		}

		// draw an arc defined by its ends
		static void drawAnyArc (const Quat &vFrom, const Quat &vTo)
		{
			Quat pts[NUM_SEGS+1];

			pts[0] = vFrom;
			pts[1] = pts[NUM_SEGS] = vTo;

			int i;
			for(i=0; i<LOG_NUM_SEGS; ++i)
				pts[1] = bisect(pts[0], pts[1]);

			double dot = 2 * Dot(pts[0],pts[1]);
			for(i=2; i<NUM_SEGS; ++i)
				pts[i] = pts[i-1]*dot - pts[i-2];

			glBegin(GL_LINE_STRIP);
			for(i=0; i<=NUM_SEGS; ++i)
				glVertex3f(pts[i].x, pts[i].y, pts[i].z);
			glEnd();
		}

		// draw the arc of a semi-circle defined by its axis
		static
			void drawHalfArc (const Quat &n)
		{
			Quat p, m;

			if(n.z != 1.f)
				p.x=n.y, p.y=-n.x, p=p.normalize();
			else
				p.x=0, p.y=1;

			m = cross(p, n);
			drawAnyArc(p,  m);
			drawAnyArc(m, -p);
		}

		/**
		* Ball math
		**/
		// convert window coordinates to sphere coordinates
		static
			Quat mouseOnSphere (const Quat &mouse, const Quat &ballCenter, double ballRadius)
		{
			// ball mouse
			Quat bm (
				(double)((mouse.x - ballCenter.x) / ballRadius),
				(double)((mouse.y - ballCenter.y) / ballRadius),
				0);

			double mag = bm.normSquared();
			if (mag > 1.0)
				bm *= 1.f / sqrt(mag);
			else
				bm.z = sqrt(1 - mag);

			return bm;
		}

		// force sphere point to be perpendicular to axis
		static
			Quat constrainToAxis (const Quat &loose, const Quat &axis)
		{
			Quat onPlane = loose - axis * Dot(axis, loose);
			double norm = onPlane.normSquared();

			if(norm > 0)
			{
				if(onPlane.z < 0)
					onPlane = -onPlane;

				onPlane *= 1.f / sqrt(norm);
			}
			else
				onPlane = axis.z==1 ? Quat(1,0,0) : Quat(-axis.y,axis.x,0).normalize();

			return onPlane;
		}

		// find the index of nearest arc of axis set
		static
			int nearestConstraintAxis (const Quat &loose, const Quat *axes, int nAxes)
		{
			double emx = -1.0f;
			int   nearest = 0;

			for (int i=0; i<nAxes; ++i)
			{
				Quat onPlane = constrainToAxis(loose, axes[i]);
				double dot = Dot(onPlane, loose);
				if(dot > emx)
					emx = dot, nearest = i;
			}

			return nearest;
		}

		// construct a normalize quaternion from two points on normalize sphere
		static
			Quat fromBallPoints (const Quat &from, const Quat &to)
		{
			return Quat(
				from.y*to.z - from.z*to.y,
				from.z*to.x - from.x*to.z,
				from.x*to.y - from.y*to.x,
				from.x*to.x + from.y*to.y + from.z*to.z );
		}

		// convert a normalize quaternion to two points on normalize sphere
		static
			void toBallPoints (const Quat &q, Quat &arcFrom, Quat &arcTo)
		{
			double s = sqrt(q.x*q.x + q.y*q.y);
			arcFrom = s ? Quat(-q.y/s, q.x/s, 0) : Quat(0, 1, 0);

			arcTo = Quat(
				q.w * arcFrom.x - q.z * arcFrom.y,
				q.w * arcFrom.y + q.z * arcFrom.x,
				q.x * arcFrom.y - q.y * arcFrom.x );

			if(q.w < 0)
				arcFrom = Quat(-arcFrom.x, -arcFrom.y, 0);
		}

//} // namespace emxUtility
