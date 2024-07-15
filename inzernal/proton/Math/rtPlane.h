#ifndef rtPlane_h__
#define rtPlane_h__

#include "../clanlib/vec3.h"

struct rtPlane
{
	rtPlane(){};

	rtPlane(CL_Vec3f vPos, CL_Vec3f vNormal)
	{

		plane[0] = vNormal.x;
		plane[1] = vNormal.y;
		plane[2] = vNormal.z;
		plane[3] = vNormal.x * -vPos.x + vNormal.y * -vPos.y + vNormal.z * -vPos.z;
	}

	double plane[4];
};
#endif // rtPlane_h__
