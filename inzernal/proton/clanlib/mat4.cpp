/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/


#include "mat4.h"
#include "vec4.h"
#include "angle.h"
#include <limits>
#include <cassert>

#ifndef WINDOWS
#define CL_DISABLE_SSE2
#endif



#ifndef CL_DISABLE_SSE2
#include <mmintrin.h>
#include <xmmintrin.h>
#endif

#ifndef FLT_EPSILON
#include "float.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Mat4 operations: That needs to be listed first because GCC is not very flexible handling templates

template<>
CL_Mat4<float> &CL_Mat4<float>::multiply(const CL_Mat4<float> &mult)
{
#ifndef CL_DISABLE_SSE2
	__m128 m1col0 = _mm_loadu_ps(matrix);
	__m128 m1col1 = _mm_loadu_ps(matrix+4);
	__m128 m1col2 = _mm_loadu_ps(matrix+8);
	__m128 m1col3 = _mm_loadu_ps(matrix+12);
	_MM_TRANSPOSE4_PS(m1col0, m1col1, m1col2, m1col3);

	for (int cur_row = 0; cur_row < 4; cur_row++)
	{
		__m128 m2row = _mm_loadu_ps(mult.matrix+cur_row*4);

		__m128 cell0 = _mm_mul_ps(m1col0, m2row);
		__m128 cell1 = _mm_mul_ps(m1col1, m2row);
		__m128 cell2 = _mm_mul_ps(m1col2, m2row);
		__m128 cell3 = _mm_mul_ps(m1col3, m2row);

		_MM_TRANSPOSE4_PS(cell0, cell1, cell2, cell3);
		__m128 row = _mm_add_ps(_mm_add_ps(_mm_add_ps(cell0, cell1), cell2), cell3);

		_mm_storeu_ps(matrix+cur_row*4, row);
	}

	return *this;
#else
	CL_Mat4<float> result;
	for (int x=0; x<4; x++)
	{
		for (int y=0; y<4; y++)
		{
			result.matrix[x+y*4] =
				matrix[0*4 + x]*mult.matrix[y*4 + 0] +
				matrix[1*4 + x]*mult.matrix[y*4 + 1] +
				matrix[2*4 + x]*mult.matrix[y*4 + 2] +
				matrix[3*4 + x]*mult.matrix[y*4 + 3];
		}
	}
	*this = result;
	return *this;
#endif
}


/////////////////////////////////////////////////////////////////////////////
// CL_Mat4 construction:

template<typename Type>
CL_Mat4<Type>::CL_Mat4(const CL_Mat3<Type> &copy)
{
	matrix[0*4 + 0] = copy.matrix[0*3 + 0];
	matrix[0*4 + 1] = copy.matrix[0*3 + 1];
	matrix[0*4 + 2] = copy.matrix[0*3 + 2];
	matrix[0*4 + 3] = 0;
	matrix[1*4 + 0] = copy.matrix[1*3 + 0];
	matrix[1*4 + 1] = copy.matrix[1*3 + 1];
	matrix[1*4 + 2] = copy.matrix[1*3 + 2];
	matrix[1*4 + 3] = 0;
	matrix[2*4 + 0] = copy.matrix[2*3 + 0];
	matrix[2*4 + 1] = copy.matrix[2*3 + 1];
	matrix[2*4 + 2] = copy.matrix[2*3 + 2];
	matrix[2*4 + 3] = 0;
	matrix[3*4 + 0] = 0;
	matrix[3*4 + 1] = 0;
	matrix[3*4 + 2] = 0;
	matrix[3*4 + 3] = 1;
}

template<typename Type>
CL_Mat4<Type>::CL_Mat4(const CL_Mat2<Type> &copy)
{
	matrix[0*4 + 0] = copy.matrix[0*2 + 0];
	matrix[0*4 + 1] = copy.matrix[0*2 + 1];
	matrix[0*4 + 2] = 0;
	matrix[0*4 + 3] = 0;
	matrix[1*4 + 0] = copy.matrix[1*2 + 0];
	matrix[1*4 + 1] = copy.matrix[1*2 + 1];
	matrix[1*4 + 2] = 0;
	matrix[1*4 + 3] = 0;
	matrix[2*4 + 0] = 0;
	matrix[2*4 + 1] = 0;
	matrix[2*4 + 2] = 1;
	matrix[2*4 + 3] = 0;
	matrix[3*4 + 0] = 0;
	matrix[3*4 + 1] = 0;
	matrix[3*4 + 2] = 0;
	matrix[3*4 + 3] = 1;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::null()
{
	CL_Mat4<Type> m;
	memset(m.matrix, 0, sizeof(m.matrix));
	return m;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::identity()
{
	CL_Mat4<Type> m = null();
	m.matrix[0] = 1;
	m.matrix[5] = 1;
	m.matrix[10] = 1;
	m.matrix[15] = 1;
	return m;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::frustum(Type  left, Type  right, Type  bottom, Type  top, Type  z_near, Type  z_far)
{
	CL_Mat4<Type> frustum_matrix = null();
	Type  a = (right + left) / (right - left);
	Type  b = (top + bottom) / (top - bottom);
	Type  c = -(z_far + z_near) / (z_far - z_near);
	Type  d = -(2 * z_far * z_near) / (z_far - z_near);
	frustum_matrix.matrix[0+0*4] = (Type) (2*z_near / (right - left));
	frustum_matrix.matrix[1+1*4] = (Type) (2*z_near / (top - bottom));
	frustum_matrix.matrix[0+2*4] = (Type) a;
	frustum_matrix.matrix[1+2*4] = (Type) b;
	frustum_matrix.matrix[2+2*4] = (Type) c;
	frustum_matrix.matrix[2+3*4] = (Type) d;
	frustum_matrix.matrix[3+2*4] = (Type) -1;
	return frustum_matrix;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::perspective(
	Type field_of_view_y_degrees,
	Type aspect,
	Type z_near,
	Type z_far)
{
	CL_Mat4<Type> projection_matrix = null();
	Type field_of_view_y_rad = Type(field_of_view_y_degrees * CL_PI / 180.0);
	Type f = Type(1.0 / tan(double(field_of_view_y_rad) / 2.0));
	projection_matrix.matrix[0+0*4] = (Type) (f/aspect);
	projection_matrix.matrix[1+1*4] = (Type) f;
	projection_matrix.matrix[2+2*4] = (Type) ((z_far + z_near) / (z_near - z_far));
	projection_matrix.matrix[2+3*4] = (Type) ((2 * z_far * z_near) / (z_near - z_far));
	projection_matrix.matrix[3+2*4] = (Type) -1;
	return projection_matrix;
}

// For floats
template<>
CL_Mat4f CL_Mat4f::perspective(
	float field_of_view_y_degrees,
	float aspect,
	float z_near,
	float z_far)
{
	CL_Mat4f projection_matrix = null();
	float field_of_view_y_rad = field_of_view_y_degrees * float(CL_PI / 180.0f);
	float f = 1.0f / tan(field_of_view_y_rad / 2.0f);
	projection_matrix.matrix[0+0*4] = (f/aspect);
	projection_matrix.matrix[1+1*4] = f;
	projection_matrix.matrix[2+2*4] = ((z_far + z_near) / (z_near - z_far));
	projection_matrix.matrix[2+3*4] = ((2.0f * z_far * z_near) / (z_near - z_far));
	projection_matrix.matrix[3+2*4] = -1.0f;
	return projection_matrix;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::ortho(Type left, Type right, Type bottom, Type top, Type z_near, Type z_far)
{
	CL_Mat4<Type> ortho_matrix = null();
	Type tx = -(right + left) / (right - left);
	Type ty = -(top + bottom) / (top - bottom);
	Type tz = -(z_far + z_near) / (z_far - z_near);
	ortho_matrix.matrix[0+0*4] = (Type) (2 / (right - left));
	ortho_matrix.matrix[1+1*4] = (Type) (2 / (top - bottom));
	ortho_matrix.matrix[2+2*4] = (Type) (-2 / (z_far - z_near));
	ortho_matrix.matrix[0+3*4] = (Type) tx;
	ortho_matrix.matrix[1+3*4] = (Type) ty;
	ortho_matrix.matrix[2+3*4] = (Type) tz;
	ortho_matrix.matrix[3+3*4] = (Type) 1;
	return ortho_matrix;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::ortho_2d(Type left, Type right, Type bottom, Type top)
{
	return ortho(left, right, bottom, top, (Type)-1.0, (Type)1.0);
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::rotate(const CL_Angle &angle, Type x, Type y, Type z, bool normalize)
{
	if (normalize)
	{
		Type len2 = x*x+y*y+z*z;
		if (len2 != (Type)1)
		{	
			Type length = sqrt(len2);
			if (length > (Type) 0)
			{
				x /= length;
				y /= length;
				z /= length;
			}
			else
			{
				x = (Type) 0;
				y = (Type) 0;
				z = (Type) 0;
			}
		}
	}

	CL_Mat4<Type> rotate_matrix = null();
	Type c = cos(angle.to_radians());
	Type s = sin(angle.to_radians());
	rotate_matrix.matrix[0+0*4] = (Type) (x*x*(1.0f - c) + c);
	rotate_matrix.matrix[0+1*4] = (Type) (x*y*(1.0f - c) - z*s);
	rotate_matrix.matrix[0+2*4] = (Type) (x*z*(1.0f - c) + y*s);
	rotate_matrix.matrix[1+0*4] = (Type) (y*x*(1.0f - c) + z*s);
	rotate_matrix.matrix[1+1*4] = (Type) (y*y*(1.0f - c) + c);
	rotate_matrix.matrix[1+2*4] = (Type) (y*z*(1.0f - c) - x*s);
	rotate_matrix.matrix[2+0*4] = (Type) (x*z*(1.0f - c) - y*s);
	rotate_matrix.matrix[2+1*4] = (Type) (y*z*(1.0f - c) + x*s);
	rotate_matrix.matrix[2+2*4] = (Type) (z*z*(1.0f - c) + c);
	rotate_matrix.matrix[3+3*4] = 1;
	return rotate_matrix;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::rotate(const CL_Angle &angle_x, const CL_Angle &angle_y, const CL_Angle &angle_z, CL_EulerOrder order)
{
	CL_Mat4<Type> rotation_matrix_x = CL_Mat4<Type>::rotate(angle_x, 1.0f, 0.0f, 0.0f, false);
	CL_Mat4<Type> rotation_matrix_y = CL_Mat4<Type>::rotate(angle_y, 0.0f, 1.0f, 0.0f, false);
	CL_Mat4<Type> rotation_matrix_z = CL_Mat4<Type>::rotate(angle_z, 0.0f, 0.0f, 1.0f, false);

	switch (order)
	{
		case cl_XYZ:
			return rotation_matrix_z * rotation_matrix_y * rotation_matrix_x;
		case cl_XZY:
			return rotation_matrix_y * rotation_matrix_z * rotation_matrix_x;
		case cl_YZX:
			return rotation_matrix_x * rotation_matrix_z * rotation_matrix_y;
		case cl_YXZ:
			return rotation_matrix_z * rotation_matrix_x * rotation_matrix_y;
		case cl_ZXY:
			return rotation_matrix_y * rotation_matrix_x * rotation_matrix_z;
		case cl_ZYX:
			return rotation_matrix_x * rotation_matrix_y * rotation_matrix_z;
		default:
			
			;
			//throw CL_Exception("Unknown euler order");
	}

	assert(!"Unknown euler order");
	return CL_Mat4<Type>::identity();
}

template<typename Type>
CL_Vec3<Type> CL_Mat4<Type>::get_euler(CL_EulerOrder order) const
{
	CL_Vec3<Type> angles;

	int pos_i;
	int pos_j;
	int pos_k;

	// Obtain the correct xyz rotation position from the original rotation order
	switch (order)
	{
		case cl_XYZ:
			pos_i = 0; pos_j = 1; pos_k = 2; break;
		case cl_XZY:
			pos_i = 0; pos_j = 2; pos_k = 1; break;
		case cl_YZX:
			pos_i = 2; pos_j = 0; pos_k = 1; break;
		case cl_YXZ:
			pos_i = 1; pos_j = 0; pos_k = 2; break;
		case cl_ZXY:
			pos_i = 1; pos_j = 2; pos_k = 0; break;
		case cl_ZYX:
			pos_i = 2; pos_j = 1; pos_k = 0; break;
		default:
			//throw CL_Exception("Unknown euler order");
			;
	}

	Type cy = sqrt(matrix[ (4*pos_i) + pos_i ]*matrix[ (4*pos_i) + pos_i ] + matrix[ (4*pos_j) + pos_i ]*matrix[ (4*pos_j) + pos_i ]);
	if (cy > (Type) 16.0*FLT_EPSILON)
	{
		angles.x = atan2(matrix[ (4*pos_k) + pos_j ], matrix[ (4*pos_k) + pos_k ]);
		angles.y = atan2(-matrix[ (4*pos_k) + pos_i ], cy);
		angles.z = atan2(matrix[ (4*pos_j) + pos_i ], matrix[ (4*pos_i) + pos_i ]);
	}
	else
	{
		angles.x = atan2(-matrix[ (4*pos_j) + pos_k ], matrix[ (4*pos_j) + pos_j ]);
		angles.y = atan2(-matrix[ (4*pos_k) + pos_i ], cy);
		angles.z = 0;
	}

	// Swap the xyz value to the specified euler angle
	switch (order)
	{
		case cl_XYZ:
			break;
		case cl_XZY:
			angles = CL_Vec3<Type>(angles.x, angles.z, angles.y);
			break;
		case cl_YZX:
			angles = CL_Vec3<Type>(angles.y, angles.z, angles.x);
			break;
		case cl_YXZ:
			angles = CL_Vec3<Type>(angles.y, angles.x, angles.z);
			break;
		case cl_ZXY:
			angles = CL_Vec3<Type>(angles.z, angles.x, angles.y);
			break;
		case cl_ZYX:
			angles = CL_Vec3<Type>(angles.z, angles.y, angles.x);
			break;
	}

	return angles;
}

template<typename Type>
CL_Vec3<Type> CL_Mat4<Type>::get_transformed_point(const CL_Vec3<Type> &vector) const
{
	CL_Vec3<Type> dest;

	dest.x = vector.x * matrix[0 + 0*4] +
			vector.y * matrix[0 + 1*4] +
			vector.z * matrix[0 + 2*4] +
			matrix[0 + 3*4];

	dest.y = vector.x * matrix[1 + 0*4] +
			vector.y * matrix[1 + 1*4] +
			vector.z * matrix[1 + 2*4] +
			matrix[1 + 3*4];

	dest.z = vector.x * matrix[2 + 0*4] +
			vector.y * matrix[2 + 1*4] +
			vector.z * matrix[2 + 2*4] +
			matrix[2 + 3*4];

	Type w = vector.x * matrix[3 + 0*4] +
			vector.y * matrix[3 + 1*4] +
			vector.z * matrix[3 + 2*4] +
			matrix[3 + 3*4];

	if (w != (Type) 0.0)
	{
		dest.x /= w;
		dest.y /= w;
		dest.z /= w;
	}

	return dest;
}

// For ints
template<>
CL_Vec3<int> CL_Mat4<int>::get_euler(CL_EulerOrder order) const
{
	//throw CL_Exception("Function not supported for ints");
	//assert(!"no");
	return CL_Vec3<int>(0,0,0);
}

// For ints
template<>
CL_Mat4<int> CL_Mat4<int>::rotate(const CL_Angle &angle, int x, int y, int z, bool normalize)
{
	if (normalize)
	{
		int len2 = x*x+y*y+z*z;
		if (len2 != (int)1)
		{	
			int length = sqrt( (float) len2);
			if (length > 0)
			{
				x /= length;
				y /= length;
				z /= length;
			}
			else
			{
				x = 0;
				y = 0;
				z = 0;
			}
		}
	}

	CL_Mat4<int> rotate_matrix = null();
	float c = cos(angle.to_radians());
	float s = sin(angle.to_radians());
	rotate_matrix.matrix[0+0*4] = (int) floor((x*x*(1.0f - c) + c)+0.5f);
	rotate_matrix.matrix[0+1*4] = (int) floor((x*y*(1.0f - c) - z*s)+0.5f);
	rotate_matrix.matrix[0+2*4] = (int) floor((x*z*(1.0f - c) + y*s)+0.5f);
	rotate_matrix.matrix[1+0*4] = (int) floor((y*x*(1.0f - c) + z*s)+0.5f);
	rotate_matrix.matrix[1+1*4] = (int) floor((y*y*(1.0f - c) + c)+0.5f);
	rotate_matrix.matrix[1+2*4] = (int) floor((y*z*(1.0f - c) - x*s)+0.5f);
	rotate_matrix.matrix[2+0*4] = (int) floor((x*z*(1.0f - c) - y*s)+0.5f);
	rotate_matrix.matrix[2+1*4] = (int) floor((y*z*(1.0f - c) + x*s)+0.5f);
	rotate_matrix.matrix[2+2*4] = (int) floor((z*z*(1.0f - c) + c)+0.5f);
	rotate_matrix.matrix[3+3*4] = 1;
	return rotate_matrix;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::scale(Type x, Type y, Type z)
{
	CL_Mat4<Type> scale_matrix = null();
	scale_matrix.matrix[0+0*4] = x;
	scale_matrix.matrix[1+1*4] = y;
	scale_matrix.matrix[2+2*4] = z;
	scale_matrix.matrix[3+3*4] = 1;
	return scale_matrix;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::translate(Type x, Type y, Type z)
{
	CL_Mat4<Type> translate_matrix = identity();
	translate_matrix.matrix[0+3*4] = x;
	translate_matrix.matrix[1+3*4] = y;
	translate_matrix.matrix[2+3*4] = z;
	return translate_matrix;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::look_at(
	Type eye_x, Type eye_y, Type eye_z,
	Type center_x, Type center_y, Type center_z,
	Type up_x, Type up_y, Type up_z)
{
	Type f[3] = { center_x - eye_x, center_y - eye_y, center_z - eye_z };
	Type length_f = (Type)sqrt(double(f[0]*f[0] + f[1]*f[1] + f[2]*f[2]));
	f[0] /= length_f;
	f[1] /= length_f;
	f[2] /= length_f;

	Type up[3] = { up_x, up_y, up_z };
	Type length_up = (Type)sqrt(double(up[0]*up[0] + up[1]*up[1] + up[2]*up[2]));
	up[0] /= length_up;
	up[1] /= length_up;
	up[2] /= length_up;

	CL_Vec4<Type> fv = CL_Vec4<Type>(f[0], f[1], f[2]);
	CL_Vec4<Type> s = CL_Vec4<Type>::cross3(fv, CL_Vec4<Type>(up[0], up[1], up[2]));
	CL_Vec4<Type> u = CL_Vec4<Type>::cross3(s, fv);

	CL_Mat4<Type> m = null();
	m.matrix[0+0*4] = (Type) s.x;
	m.matrix[0+1*4] = (Type) s.y;
	m.matrix[0+2*4] = (Type) s.z;
	m.matrix[1+0*4] = (Type) u.x;
	m.matrix[1+1*4] = (Type) u.y;
	m.matrix[1+2*4] = (Type) u.z;
	m.matrix[2+0*4] = (Type) -f[0];
	m.matrix[2+1*4] = (Type) -f[1];
	m.matrix[2+2*4] = (Type) -f[2];
	m.matrix[3+3*4] = 1;
	m.multiply(translate((Type) -eye_x, (Type) -eye_y, (Type) -eye_z));
	return m;
}

// For floats
template<>
CL_Mat4f CL_Mat4f::look_at(
	float eye_x, float eye_y, float eye_z,
	float center_x, float center_y, float center_z,
	float up_x, float up_y, float up_z)
{
	float f[3] = { center_x - eye_x, center_y - eye_y, center_z - eye_z };
	float length_f = sqrt(f[0]*f[0] + f[1]*f[1] + f[2]*f[2]);
	f[0] /= length_f;
	f[1] /= length_f;
	f[2] /= length_f;

	float up[3] = { up_x, up_y, up_z };
	float length_up = sqrt(up[0]*up[0] + up[1]*up[1] + up[2]*up[2]);
	up[0] /= length_up;
	up[1] /= length_up;
	up[2] /= length_up;

	CL_Vec4f fv = CL_Vec4f(f[0], f[1], f[2]);
	CL_Vec4f s = CL_Vec4f::cross3(fv, CL_Vec4f(up[0], up[1], up[2]));
	CL_Vec4f u = CL_Vec4f::cross3(s, fv);

	CL_Mat4f m = null();
	m.matrix[0+0*4] =  s.x;
	m.matrix[0+1*4] =  s.y;
	m.matrix[0+2*4] =  s.z;
	m.matrix[1+0*4] =  u.x;
	m.matrix[1+1*4] =  u.y;
	m.matrix[1+2*4] =  u.z;
	m.matrix[2+0*4] =  -f[0];
	m.matrix[2+1*4] =  -f[1];
	m.matrix[2+2*4] =  -f[2];
	m.matrix[3+3*4] = 1;
	m.multiply(translate( -eye_x,  -eye_y,  -eye_z));
	return m;
}
template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::multiply(const CL_Mat4<Type> &matrix_1, const CL_Mat4<Type> &matrix_2)
{
	CL_Mat4<Type> dest(matrix_2);
	dest.multiply(matrix_1);
	return dest;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::add(const CL_Mat4<Type> &matrix_1, const CL_Mat4<Type> &matrix_2)
{
	CL_Mat4<Type> dest(matrix_2);
	dest.add(matrix_1);
	return dest;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::subtract(const CL_Mat4<Type> &matrix_1, const CL_Mat4<Type> &matrix_2)
{
	CL_Mat4<Type> dest(matrix_2);
	dest.subtract(matrix_1);
	return dest;
}
template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::adjoint(const CL_Mat4<Type> &matrix)
{
	CL_Mat4<Type> dest(matrix);
	dest.adjoint();
	return dest;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::inverse(const CL_Mat4<Type> &matrix)
{
	CL_Mat4<Type> dest(matrix);
	dest.inverse();
	return dest;
}

template<typename Type>
CL_Mat4<Type> CL_Mat4<Type>::transpose(const CL_Mat4<Type> &matrix)
{
	CL_Mat4<Type> dest(matrix);
	dest.transpose();
	return dest;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mat4 attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Mat4 operators:

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::operator =(const CL_Mat3<Type> &copy)
{
	matrix[0*4 + 0] = copy.matrix[0*3 + 0];
	matrix[0*4 + 1] = copy.matrix[0*3 + 1];
	matrix[0*4 + 2] = copy.matrix[0*3 + 2];
	matrix[0*4 + 3] = 0;
	matrix[1*4 + 0] = copy.matrix[1*3 + 0];
	matrix[1*4 + 1] = copy.matrix[1*3 + 1];
	matrix[1*4 + 2] = copy.matrix[1*3 + 2];
	matrix[1*4 + 3] = 0;
	matrix[2*4 + 0] = copy.matrix[2*3 + 0];
	matrix[2*4 + 1] = copy.matrix[2*3 + 1];
	matrix[2*4 + 2] = copy.matrix[2*3 + 2];
	matrix[2*4 + 3] = 0;
	matrix[3*4 + 0] = 0;
	matrix[3*4 + 1] = 0;
	matrix[3*4 + 2] = 0;
	matrix[3*4 + 3] = 1;
	return *this;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::operator =(const CL_Mat2<Type> &copy)
{
	matrix[0*4 + 0] = copy.matrix[0*2 + 0];
	matrix[0*4 + 1] = copy.matrix[0*2 + 1];
	matrix[0*4 + 2] = 0;
	matrix[0*4 + 3] = 0;
	matrix[1*4 + 0] = copy.matrix[1*2 + 0];
	matrix[1*4 + 1] = copy.matrix[1*2 + 1];
	matrix[1*4 + 2] = 0;
	matrix[1*4 + 3] = 0;
	matrix[2*4 + 0] = 0;
	matrix[2*4 + 1] = 0;
	matrix[2*4 + 2] = 1;
	matrix[2*4 + 3] = 0;
	matrix[3*4 + 0] = 0;
	matrix[3*4 + 1] = 0;
	matrix[3*4 + 2] = 0;
	matrix[3*4 + 3] = 1;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mat4 operations:

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::multiply(const CL_Mat4<Type> &mult)
{
	CL_Mat4<Type> result;
	for (int x=0; x<4; x++)
	{
		for (int y=0; y<4; y++)
		{
			result.matrix[x+y*4] =
				matrix[0*4 + x]*mult.matrix[y*4 + 0] +
				matrix[1*4 + x]*mult.matrix[y*4 + 1] +
				matrix[2*4 + x]*mult.matrix[y*4 + 2] +
				matrix[3*4 + x]*mult.matrix[y*4 + 3];
		}
	}
	*this = result;
	return *this;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::scale_self(Type x, Type y, Type z)
{
	matrix[0+4*0] *= x;
	matrix[0+4*1] *= y;
	matrix[0+4*2] *= z;
	
	matrix[1+4*0] *= x;
	matrix[1+4*1] *= y;
	matrix[1+4*2] *= z;

	matrix[2+4*0] *= x;
	matrix[2+4*1] *= y;
	matrix[2+4*2] *= z;

	matrix[3+4*0] *= x;
	matrix[3+4*1] *= y;
	matrix[3+4*2] *= z;
	return *this;
}

template<>
CL_Mat4<float> &CL_Mat4<float>::scale_self(float x, float y, float z)
{
	/*
	// FIXME
	__m128 row0 = _mm_loadu_ps(matrix);
	__m128 row1 = _mm_loadu_ps(matrix+4);
	__m128 row2 = _mm_loadu_ps(matrix+8);
	__m128 row3 = _mm_loadu_ps(matrix+12);
	__m128 vec = _mm_set_ps(x, y, z, 1.0f);

	row0 = _mm_mul_ps(row0, vec);
	row1 = _mm_mul_ps(row1, vec);
	row2 = _mm_mul_ps(row2, vec);
	row3 = _mm_mul_ps(row3, vec);

	_mm_storeu_ps(matrix, row0);
	_mm_storeu_ps(matrix+4, row1);
	_mm_storeu_ps(matrix+8, row2);
	_mm_storeu_ps(matrix+12, row3);
*/
	matrix[0+4*0] *= x;
	matrix[0+4*1] *= y;
	matrix[0+4*2] *= z;
	
	matrix[1+4*0] *= x;
	matrix[1+4*1] *= y;
	matrix[1+4*2] *= z;

	matrix[2+4*0] *= x;
	matrix[2+4*1] *= y;
	matrix[2+4*2] *= z;

	matrix[3+4*0] *= x;
	matrix[3+4*1] *= y;
	matrix[3+4*2] *= z;
	return *this;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::translate_self(Type x, Type y, Type z)
{
	Type translate_value_1 = (matrix[0+4*0] * x) + (matrix[0+4*1] * y) + (matrix[0+4*2] * z) + matrix[0+4*3];
	Type translate_value_2 = (matrix[1+4*0] * x) + (matrix[1+4*1] * y) + (matrix[1+4*2] * z) + matrix[1+4*3];
	Type translate_value_3 = (matrix[2+4*0] * x) + (matrix[2+4*1] * y) + (matrix[2+4*2] * z) + matrix[2+4*3];
	Type translate_value_4 = (matrix[3+4*0] * x) + (matrix[3+4*1] * y) + (matrix[3+4*2] * z) + matrix[3+4*3];

	matrix[0+4*3] = translate_value_1;
	matrix[1+4*3] = translate_value_2;
	matrix[2+4*3] = translate_value_3;
	matrix[3+4*3] = translate_value_4;
	return *this;
}

template<>
CL_Mat4<float> &CL_Mat4<float>::translate_self(float x, float y, float z)
{
	/*
	// FIXME

	__m128 row0 = _mm_loadu_ps(matrix);
	__m128 row1 = _mm_loadu_ps(matrix+4);
	__m128 row2 = _mm_loadu_ps(matrix+8);
	__m128 row3 = _mm_loadu_ps(matrix+12);

	row0 = _mm_mul_ps(row0, _mm_set1_ps(x));
	row1 = _mm_mul_ps(row1, _mm_set1_ps(y));
	row2 = _mm_mul_ps(row2, _mm_set1_ps(z));

	_MM_TRANSPOSE4_PS(row0, row1, row2, row3);

	__m128 result = _mm_add_ps(_mm_add_ps(_mm_add_ps(row0, row1), row2), row3);
	_mm_storeu_ps(matrix+12, result);
*/
	float translate_value_1 = (matrix[0+4*0] * x) + (matrix[0+4*1] * y) + (matrix[0+4*2] * z) + matrix[0+4*3];
	float translate_value_2 = (matrix[1+4*0] * x) + (matrix[1+4*1] * y) + (matrix[1+4*2] * z) + matrix[1+4*3];
	float translate_value_3 = (matrix[2+4*0] * x) + (matrix[2+4*1] * y) + (matrix[2+4*2] * z) + matrix[2+4*3];
	float translate_value_4 = (matrix[3+4*0] * x) + (matrix[3+4*1] * y) + (matrix[3+4*2] * z) + matrix[3+4*3];

	matrix[0+4*3] = translate_value_1;
	matrix[1+4*3] = translate_value_2;
	matrix[2+4*3] = translate_value_3;
	matrix[3+4*3] = translate_value_4;

	return *this;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::add(const CL_Mat4<Type> &add_matrix)
{
	matrix[(0*4) + 0] += add_matrix.matrix[(0*4) + 0];
	matrix[(0*4) + 1] += add_matrix.matrix[(0*4) + 1];
	matrix[(0*4) + 2] += add_matrix.matrix[(0*4) + 2];
	matrix[(0*4) + 3] += add_matrix.matrix[(0*4) + 3];
	matrix[(1*4) + 0] += add_matrix.matrix[(1*4) + 0];
	matrix[(1*4) + 1] += add_matrix.matrix[(1*4) + 1];
	matrix[(1*4) + 2] += add_matrix.matrix[(1*4) + 2];
	matrix[(1*4) + 3] += add_matrix.matrix[(1*4) + 3];
	matrix[(2*4) + 0] += add_matrix.matrix[(2*4) + 0];
	matrix[(2*4) + 1] += add_matrix.matrix[(2*4) + 1];
	matrix[(2*4) + 2] += add_matrix.matrix[(2*4) + 2];
	matrix[(2*4) + 3] += add_matrix.matrix[(2*4) + 3];
	matrix[(3*4) + 0] += add_matrix.matrix[(3*4) + 0];
	matrix[(3*4) + 1] += add_matrix.matrix[(3*4) + 1];
	matrix[(3*4) + 2] += add_matrix.matrix[(3*4) + 2];
	matrix[(3*4) + 3] += add_matrix.matrix[(3*4) + 3];
	return *this;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::subtract(const CL_Mat4<Type> &sub_matrix)
{
	matrix[(0*4) + 0] = sub_matrix.matrix[(0*4) + 0] - matrix[(0*4) + 0];
	matrix[(0*4) + 1] = sub_matrix.matrix[(0*4) + 1] - matrix[(0*4) + 1];
	matrix[(0*4) + 2] = sub_matrix.matrix[(0*4) + 2] - matrix[(0*4) + 2];
	matrix[(0*4) + 3] = sub_matrix.matrix[(0*4) + 3] - matrix[(0*4) + 3];
	matrix[(1*4) + 0] = sub_matrix.matrix[(1*4) + 0] - matrix[(1*4) + 0];
	matrix[(1*4) + 1] = sub_matrix.matrix[(1*4) + 1] - matrix[(1*4) + 1];
	matrix[(1*4) + 2] = sub_matrix.matrix[(1*4) + 2] - matrix[(1*4) + 2];
	matrix[(1*4) + 3] = sub_matrix.matrix[(1*4) + 3] - matrix[(1*4) + 3];
	matrix[(2*4) + 0] = sub_matrix.matrix[(2*4) + 0] - matrix[(2*4) + 0];
	matrix[(2*4) + 1] = sub_matrix.matrix[(2*4) + 1] - matrix[(2*4) + 1];
	matrix[(2*4) + 2] = sub_matrix.matrix[(2*4) + 2] - matrix[(2*4) + 2];
	matrix[(2*4) + 3] = sub_matrix.matrix[(2*4) + 3] - matrix[(2*4) + 3];
	matrix[(3*4) + 0] = sub_matrix.matrix[(3*4) + 0] - matrix[(3*4) + 0];
	matrix[(3*4) + 1] = sub_matrix.matrix[(3*4) + 1] - matrix[(3*4) + 1];
	matrix[(3*4) + 2] = sub_matrix.matrix[(3*4) + 2] - matrix[(3*4) + 2];
	matrix[(3*4) + 3] = sub_matrix.matrix[(3*4) + 3] - matrix[(3*4) + 3];
	return *this;
}

template<typename Type>
double CL_Mat4<Type>::det() const
{
	CL_Mat3<Type> temp_matrix;
	Type a, b, c, d;
	Type value;

	temp_matrix = CL_Mat3<Type>(
		matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
		matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
		matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
	a = (Type)temp_matrix.det();

	temp_matrix = CL_Mat3<Type>(
		matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
		matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2],
		matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
	b = (Type)temp_matrix.det();

	temp_matrix = CL_Mat3<Type>(
		matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
		matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
		matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3]);
	c = (Type)temp_matrix.det();

	temp_matrix = CL_Mat3<Type>(
		matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0],
		matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1],
		matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2]);
	d = (Type)temp_matrix.det();

	value  = matrix[0 * 4 + 0] * a;
	value -= matrix[0 * 4 + 1] * b;
	value += matrix[0 * 4 + 2] * c;
	value -= matrix[0 * 4 + 3] * d;

	return value;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::adjoint()
{
	CL_Mat4<Type> result;
	CL_Mat3<Type> m;

	m = CL_Mat3<Type>(
			matrix[1*4 + 1], matrix[2*4 + 1], matrix[3*4 + 1],
			matrix[1*4 + 2], matrix[2*4 + 2], matrix[3*4 + 2],
			matrix[1*4 + 3], matrix[2*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 0 * 4 + 0 ] = (Type) m.det();

	m = CL_Mat3<Type>(
			matrix[1*4 + 0], matrix[2*4 + 0], matrix[3*4 + 0],
			matrix[1*4 + 2], matrix[2*4 + 2], matrix[3*4 + 2],
			matrix[1*4 + 3], matrix[2*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 1 * 4 + 0 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[1*4 + 0], matrix[2*4 + 0], matrix[3*4 + 0],
			matrix[1*4 + 1], matrix[2*4 + 1], matrix[3*4 + 1],
			matrix[1*4 + 3], matrix[2*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 2 * 4 + 0 ] = (Type)  m.det();

	m = CL_Mat3<Type>(
			matrix[1*4 + 0], matrix[2*4 + 0], matrix[3*4 + 0],
			matrix[1*4 + 1], matrix[2*4 + 1], matrix[3*4 + 1],
			matrix[1*4 + 2], matrix[2*4 + 2], matrix[3*4 + 2] );
	result.matrix[ 3 * 4 + 0 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 1], matrix[2*4 + 1], matrix[3*4 + 1],
			matrix[0*4 + 2], matrix[2*4 + 2], matrix[3*4 + 2],
			matrix[0*4 + 3], matrix[2*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 0 * 4 + 1 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[2*4 + 0], matrix[3*4 + 0],
			matrix[0*4 + 2], matrix[2*4 + 2], matrix[3*4 + 2],
			matrix[0*4 + 3], matrix[2*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 1 * 4 + 1 ] = (Type)  m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[2*4 + 0], matrix[3*4 + 0],
			matrix[0*4 + 1], matrix[2*4 + 1], matrix[3*4 + 1],
			matrix[0*4 + 3], matrix[2*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 2 * 4 + 1 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[2*4 + 0], matrix[3*4 + 0],
			matrix[0*4 + 1], matrix[2*4 + 1], matrix[3*4 + 1],
			matrix[0*4 + 2], matrix[2*4 + 2], matrix[3*4 + 2] );
	result.matrix[ 3 * 4 + 1 ] = (Type)  m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 1], matrix[1*4 + 1], matrix[3*4 + 1],
			matrix[0*4 + 2], matrix[1*4 + 2], matrix[3*4 + 2],
			matrix[0*4 + 3], matrix[1*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 0 * 4 + 2 ] = (Type)  m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[1*4 + 0], matrix[3*4 + 0],
			matrix[0*4 + 2], matrix[1*4 + 2], matrix[3*4 + 2],
			matrix[0*4 + 3], matrix[1*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 1 * 4 + 2 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[1*4 + 0], matrix[3*4 + 0],
			matrix[0*4 + 1], matrix[1*4 + 1], matrix[3*4 + 1],
			matrix[0*4 + 3], matrix[1*4 + 3], matrix[3*4 + 3] );
	result.matrix[ 2 * 4 + 2 ] = (Type)  m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[1*4 + 0], matrix[3*4 + 0],
			matrix[0*4 + 1], matrix[1*4 + 1], matrix[3*4 + 1],
			matrix[0*4 + 2], matrix[1*4 + 2], matrix[3*4 + 2] );
	result.matrix[ 3 * 4 + 2 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 1], matrix[1*4 + 1], matrix[2*4 + 1],
			matrix[0*4 + 2], matrix[1*4 + 2], matrix[2*4 + 2],
			matrix[0*4 + 3], matrix[1*4 + 3], matrix[2*4 + 3] );
	result.matrix[ 0 * 4 + 3 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[1*4 + 0], matrix[2*4 + 0],
			matrix[0*4 + 2], matrix[1*4 + 2], matrix[2*4 + 2],
			matrix[0*4 + 3], matrix[1*4 + 3], matrix[2*4 + 3] );
	result.matrix[ 1 * 4 + 3 ] = (Type)  m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[1*4 + 0], matrix[2*4 + 0],
			matrix[0*4 + 1], matrix[1*4 + 1], matrix[2*4 + 1],
			matrix[0*4 + 3], matrix[1*4 + 3], matrix[2*4 + 3] );
	result.matrix[ 2 * 4 + 3 ] = (Type) -m.det();

	m = CL_Mat3<Type>(
			matrix[0*4 + 0], matrix[1*4 + 0], matrix[2*4 + 0],
			matrix[0*4 + 1], matrix[1*4 + 1], matrix[2*4 + 1],
			matrix[0*4 + 2], matrix[1*4 + 2], matrix[2*4 + 2] );
	result.matrix[ 3 * 4 + 3 ] = (Type)  m.det();

	*this = result;
	return *this;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::inverse()
{
	double d;

	d = det();

	// Inverse unknown when determinant is close to zero
	if (fabs(d) < 1e-15)
	{
		*this = null();
	}
	else
	{
		CL_Mat4<Type> result = *this;
		result.adjoint();

		d=1.0/d;	// Inverse the determinant
		for (int i=0; i<16; i++)
		{
			result.matrix[i] = (Type) (result.matrix[i] * d);
		}

		*this = result;
	}
	return *this;
}

template<typename Type>
CL_Mat4<Type> &CL_Mat4<Type>::transpose()
{
	Type original[16];
	for (int cnt=0; cnt<16; cnt++)
		original[cnt] = matrix[cnt];

	matrix[0] = original[0];
	matrix[1] = original[4];
	matrix[2] = original[8];
	matrix[3] = original[12];
	matrix[4] = original[1];
	matrix[5] = original[5];
	matrix[6] = original[9];
	matrix[7] = original[13];
	matrix[8] = original[2];
	matrix[9] = original[6];
	matrix[10] = original[10];
	matrix[11] = original[14];
	matrix[12] = original[3];
	matrix[13] = original[7];
	matrix[14] = original[11];
	matrix[15] = original[13];

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mat4 implementation:

// Explicit instantiate the versions we use:
template class CL_Mat4<int>;
template class CL_Mat4<float>;
template class CL_Mat4<double>;
