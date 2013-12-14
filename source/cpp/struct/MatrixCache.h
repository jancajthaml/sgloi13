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
	static Matrix Y;

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
		float invDiffRL = 1.0f / (right - left);
		float invDiffTB = 1.0f / (top - bottom);
		float invDiffFN = 1.0f / (far - near);

		F.matrix[0]		= 2.0f * near * invDiffRL;
		F.matrix[5]		= 2.0f * near * invDiffTB;
		F.matrix[8]		= (right + left) * invDiffRL;
		F.matrix[9]		= (top + bottom) * invDiffTB;
		F.matrix[10]	= -((far + near) * invDiffFN);
		F.matrix[11]	= -1.0f;
		F.matrix[14]	= -2.0f * far * near * invDiffFN;
		F.matrix[15]    = 0.0f;

		return F;
	}

	static inline Matrix ortho(float left, float right, float bottom, float top, float near, float far)
	{
		// helper variables
		float invDiffRL = 1.0f / (right - left);
		float invDiffTB = 1.0f / (top - bottom);
		float invDiffFN = 1.0f / (far - near);

		O.matrix[0]		= 2.0f * invDiffRL;
		O.matrix[5]		= 2.0f * invDiffTB;
		O.matrix[10]	= -2.0f * invDiffFN;
		O.matrix[12]	= -((right+left)*invDiffRL);
		O.matrix[13]	= -((top+bottom)*invDiffTB);
		O.matrix[14]	= -((far+near)*invDiffFN);

		return O;
	}

	static inline Matrix rotateY( float angle )
	{
		float sin    = sinf(-angle);
		float cos    = cosf(-angle);

		Y.matrix[0]  = cos;
		Y.matrix[2]  = -sin;
		Y.matrix[8]  = sin;
		Y.matrix[10] = cos;

		return Y;
	}
};

Matrix MatrixCache::R = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::S = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::I = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::T = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::F = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::O = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix MatrixCache::Y = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

#endif /* MATRIXCACHE_H_ */
