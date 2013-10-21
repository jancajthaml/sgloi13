/*
 * MatrixCache.h
 *
 *  Created on: 10.10.2013
 *      Author: jancajthaml
 */

#ifndef MATRIXCACHE_H_
#define MATRIXCACHE_H_

#include "./Matrix.h"

//TODO COMMENT !!!!!!!
struct MatrixCache
{
	static Matrix R;
	static Matrix T;
	static Matrix I;
	static Matrix S;
	static Matrix F;
	static Matrix O;

	static inline Matrix identity()
	{ return I; }

	static inline Matrix scale(float sc_x, float sc_y, float sc_z)
	{
		S.matrix[0]		= sc_x;
		S.matrix[5]		= sc_y;
		S.matrix[10]	= sc_z;
		return S;
	}

	static inline Matrix translate(float t_x, float t_y, float t_z)
	{
		T.matrix[12]	= t_x;
		T.matrix[13]	= t_y;
		T.matrix[14]	= t_z;
		return T;
	};

	static inline Matrix rotate2D(float angle, float c_x, float c_y)
	{
		float cos_a = cosf(angle);
		float sin_a = sinf(angle);

		R.matrix[0]	= cos_a;
		R.matrix[1]	= sin_a;
		R.matrix[4]	= -sin_a;
		R.matrix[5]	= cos_a;

		return R;
	}

	static inline Matrix frustum(float left, float right, float bottom, float top, float near, float far)
	{
		float r = (right-left);
		float t = (top-bottom);

		F.matrix[0] = 2.0f*near / r;
		F.matrix[5] = 2.0f*near / t;
		F.matrix[8] = (right + left) / (right - left);
		F.matrix[9] = (top + bottom) / (top - bottom);
		F.matrix[10] = -(far + near) / (far - near);
		F.matrix[11] = -1.0f;
		F.matrix[14] = -(2.0f * near * far) / (far - near);

		return F;
	}

	static inline Matrix ortho(float left, float right, float bottom, float top, float near, float far)
	{

	  O.matrix[0] = 2.0f/(right-left);
	  O.matrix[5] = 2.0f/(top-bottom);
	  O.matrix[10] = -2.0f/(far-near);
	  O.matrix[12] = -((right + left)/(right-left));
	  O.matrix[13] = -((top + bottom)/(top-bottom));
	  O.matrix[14] = -((far + near)/(far-near));

	  return O;
	}

	static inline Matrix rotateY(float angle)
	{ return identity(); }
};

Matrix MatrixCache::R = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::S = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::I = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::T = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::F = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::O = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

#endif /* MATRIXCACHE_H_ */
