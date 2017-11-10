#include "Quaternion.h"

static const double M_PI_4         = 0.785398163397448309616;


	const double Quat::COSPI4 = cos(M_PI_4);

	const Quat
		Quat::ZERO,
		Quat::ONE(0,0,0,1),
		//Quat::VIEWDEFAULT(-0.56156111, 0.13810496, 0.21097302, 0.78807765),
		Quat::VIEWDEFAULT(0.261225, 0.565943, 0.707711, -0.332589),
		Quat::VIEWXY(0, 0, 0, 1),
		Quat::VIEWXZ(-COSPI4, 0, 0, COSPI4),
		Quat::VIEWYZ(-0.5, -0.5, -0.5, 0.5);

	// operators
	Quat Quat::operator* (const Quat &r) const
	{
		return Quat(
			w*r.x + x*r.w + y*r.z - z*r.y,
			w*r.y + y*r.w + z*r.x - x*r.z,
			w*r.z + z*r.w + x*r.y - y*r.x,
			w*r.w - x*r.x - y*r.y - z*r.z
			);
	}

//} // namespace emxUtility