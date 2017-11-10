#ifndef EMXRENDERUTILITY_H
#define EMXRENDERUTILITY_H

#include "util/Vector.h"
#include "util/Color.h"
#include "util/Quaternion.h"
#include "util/ArcBall.h"
#include "windows.h"
#include <gl/gl.h>
#include <gl/glu.h>

#pragma warning( disable  : 4996 )

	inline void LoadVertex (const Vector3d &v)
	{ glVertex3d(v.x, v.y, v.z); }

	inline void LoadNormal (const Vector3d &n)
	{ glNormal3d(n.x, n.y, n.z); }

	inline void LoadColor (const Color &c)
	{ glColor3d(c.r,c.g,c.b); }
	inline void LoadColor (const Color &c, double alpha)
	{
		glColor4d(c.r, c.g, c.b, alpha);
	}

	inline void DrawLine(double xs, double ys, double zs, double xe, double ye, double ze)
	{
		glBegin(GL_LINES);
		glVertex3d(xs, ys, zs);
		glVertex3d(xe, ye, ze);
		glEnd();
	}
	/*
	*	draw box(minPos, maxPos)
	*/
	inline void DrawBox(const Vector3d& minPos, const Vector3d& maxPos)
	{
		// vertex in back face
		Vector3d vec1(minPos);
		Vector3d vec2(minPos.x, minPos.y, maxPos.z);
		Vector3d vec3(minPos.x, maxPos.y, maxPos.z);
		Vector3d vec4(minPos.x, maxPos.y, minPos.z);

		// vertex in front face
		Vector3d vec5(maxPos.x, minPos.y, minPos.z);
		Vector3d vec6(maxPos.x, minPos.y, maxPos.z);
		Vector3d vec7(maxPos);
		Vector3d vec8(maxPos.x, maxPos.y, minPos.z);

		// edges in front face
		glBegin(GL_LINE_LOOP);
		LoadVertex(vec5);
		LoadVertex(vec6);
		LoadVertex(vec7);
		LoadVertex(vec8);
		glEnd();

		// edges in back face
		glBegin(GL_LINE_LOOP);
		LoadVertex(vec1);
		LoadVertex(vec2);
		LoadVertex(vec3);
		LoadVertex(vec4);
		glEnd();

		// edges between back and front face
		glBegin(GL_LINES);
		LoadVertex(vec1);
		LoadVertex(vec5);
		LoadVertex(vec2);
		LoadVertex(vec6);
		LoadVertex(vec3);
		LoadVertex(vec7);
		LoadVertex(vec4);
		LoadVertex(vec8);
		glEnd();
	}
#endif //EMXRENDERUTILITY_H
