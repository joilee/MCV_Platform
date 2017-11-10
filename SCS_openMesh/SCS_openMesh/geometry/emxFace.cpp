#include "geometry/emxFace.h"
#include "geometry/emxVertex.h"




/*
 * constructor
 *	save the vertices references
 *	build the edges of this face and save the first edge reference
 */
emxFace::emxFace (emxVertex* v0, emxVertex* v1, emxVertex* v2)
:normalValid(false)
{
	assert(v0 && v1 && v2);
	assert(v0!=v1 && v0!=v2 && v1!=v2);

	// save references to vertices
	vertex[0] = v0;
	vertex[1] = v1;
	vertex[2] = v2;

	// for valid face assertion
	assert(IsValid());

	//index = FACECOUNT++;

}

/*
 * destructor
 *	delete all the edges of this face
 */
emxFace::~emxFace()
{
}


/*
 * get the center point position of the face
 */
Vector3d emxFace::GetCenter() const
{
	return (vertex[0]->GetPos() + vertex[1]->GetPos() + vertex[2]->GetPos()) / 3.0; 
}

/*
 * get the normal vector of the face
 */
Vector3d emxFace::GetNormal() const
{
	if (!normalValid) 
	{
		normal = VectorCross(vertex[2]->GetPos() - vertex[1]->GetPos(), vertex[0]->GetPos() - vertex[1]->GetPos());
		if (normal.norm() == 0.0) 
		{
			normal = Vector3d(0.0, 0.0, 0.0);
		} 
		else 
		{
			normal = normal.normalize();
		}		
		normalValid = true;
	}
	return normal;
}



/*
 *	ray triangle intersect test
 */
bool emxFace::IntersectP(const emxRay& ray)
{
	Vector3d E1 = vertex[1]->GetPos() - vertex[0]->GetPos();
	Vector3d E2 = vertex[2]->GetPos() - vertex[0]->GetPos();
	Vector3d S  = ray.origin - vertex[0]->GetPos();
	Vector3d S1 = VectorCross(ray.direction, E2);
	Vector3d S2 = VectorCross(S, E1);

	double denometor = Dot(S1, E1);
	if(fabs(denometor) > 1.0e-15) {
		denometor = 1.0 / denometor;
		double t = Dot(S2, E2) * denometor;
		//if(t <= 0.0 || t > ray.maxt || t < ray.mint) // t <= DOUBLE_EPSILON for thickless plate
		//	return false;
		if(t <= DOUBLE_EPSILON || t > ray.maxt - DOUBLE_EPSILON || t < ray.mint + DOUBLE_EPSILON)		// avoid intersect with the plane which ray origin is on  
			return false;
		double u = Dot(S1, S) * denometor;
		double v = Dot(S2, ray.direction) * denometor;
		return (UnitValue(u) && UnitValue(v) && UnitValue(u + v));
	}

	return false;
}

/*
 * ray triangle intersect
 */
bool emxFace::Intersect(const emxRay& ray, double& tHit)
{
	Vector3d E1 = vertex[1]->GetPos() - vertex[0]->GetPos();
	Vector3d E2 = vertex[2]->GetPos() - vertex[0]->GetPos();
	Vector3d S  = ray.origin - vertex[0]->GetPos();
	Vector3d S1 = VectorCross(ray.direction, E2);
	Vector3d S2 = VectorCross(S, E1);

	double denometor = Dot(S1, E1);
	if(fabs(denometor) > 1.0e-15) {
		denometor = 1.0 / denometor;
		double t = Dot(S2, E2) * denometor;
		//if(t <= 0.0 || t > ray.maxt || t < ray.mint)		// avoid intersect with the edge of two triangles
		if(t <= DOUBLE_EPSILON || t > ray.maxt - DOUBLE_EPSILON || t < ray.mint + DOUBLE_EPSILON)		// avoid intersect with the plane which ray origin is on  
			return false;
		double v = Dot(S1, S) * denometor;
		double w = Dot(S2, ray.direction) * denometor;
		double u = 1 - v - w;
		if(UnitValue(u) && UnitValue(v) && UnitValue(w)) {
			tHit = t;
			return true;
		}
	}

	return false;
}

/*
 *	ray triangle intersect with calculate curvatures
 */
bool emxFace::Intersect(const emxRay& ray, double& tHit, emxIntersectionInfo& iInfo)
{
	Vector3d E1 = vertex[1]->GetPos() - vertex[0]->GetPos();
	Vector3d E2 = vertex[2]->GetPos() - vertex[0]->GetPos();
	Vector3d S  = ray.origin - vertex[0]->GetPos();
	Vector3d S1 = VectorCross(ray.direction, E2);
	Vector3d S2 = VectorCross(S, E1);

	double denometor = Dot(S1, E1);
	if(fabs(denometor) > 1.0e-15) 
	{
		denometor = 1.0 / denometor;
		double t = Dot(S2, E2) * denometor;
		//if(t <= 0.0 || t > ray.maxt || t < ray.mint)		// avoid intersect with the edge of two triangles
		//	return false;
		if(t <= DOUBLE_EPSILON || t > ray.maxt - DOUBLE_EPSILON || t < ray.mint + DOUBLE_EPSILON)		// avoid intersect with the plane which ray origin is on  
			return false;
		double v = Dot(S1, S) * denometor;
		double w = Dot(S2, ray.direction) * denometor;
		double u = 1 - v - w;
		if(UnitValue(u) && UnitValue(v) && UnitValue(w)) 
		{
			tHit = t;
			// Coordinate
			iInfo.iCoordinate = ray.origin + ray.direction * t;
			return true;
		}
	}

	return false;
}



int emxFace::FACECOUNT = 0;



/*
 *	if the 3 vertexes of this face are on the same line, return false
 */
bool emxFace::IsValid() const
{
	Vector3d vector1 = vertex[1]->GetPos() - vertex[0]->GetPos();
	Vector3d vector2 = vertex[2]->GetPos() - vertex[0]->GetPos();
	Vector3d crossVec = VectorCross(vector1, vector2);
	return (crossVec.norm() > DOUBLE_EPSILON);
}

/*
 *	the area of the face
 */
 double emxFace::GetArea()
 {
	 double la = (vertex[0]->GetPos() - vertex[1]->GetPos()).norm();
	 double lb = (vertex[1]->GetPos() - vertex[2]->GetPos()).norm();
	 double lc = (vertex[2]->GetPos() - vertex[0]->GetPos()).norm();
	 double ss = (la + lb + lc) / 2;
	 return sqrt(ss * ( ss - la) * (ss - lb) * (ss - lc));	
 }
