/*
 * Matrix.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef MATRIXCACHE_H_
#define MATRIXCACHE_H_

#include "Matrix.h"

//Matrix
// vectors in columns
//Attributes are slower
struct MatrixCache
{
	static Matrix R;
	static Matrix T;
	static Matrix I;
	static Matrix S;

	static Matrix identity()
	{ return I; }

	static Matrix scale(float sc_x, float sc_y, float sc_z)
	{
		S.matrix[0]		= sc_x;
		S.matrix[5]		= sc_y;
		S.matrix[10]	= sc_z;
		return S;
	}

	static Matrix translate(float t_x, float t_y, float t_z)
	{
		T.matrix[12]	= t_x;
		T.matrix[13]	= t_y;
		T.matrix[14]	= t_z;
		return T;
	};

	static Matrix rotate2D(float angle, float c_x, float c_y)
	{
		float cos_a = cos(angle);
		float sin_a = sin(angle);

		R.matrix[0]	= cos_a;
		R.matrix[1]	= -sin_a;
		R.matrix[4]	= sin_a;
		R.matrix[5]	= cos_a;

		return R;
	}

	static Matrix rotateY(float angle)
	{ return identity(); }

};



#endif /* MATRIX_H_ */
