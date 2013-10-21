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

	static inline Matrix rotateY(float angle)
	{ return identity(); }
};



#endif /* MATRIXCACHE_H_ */
