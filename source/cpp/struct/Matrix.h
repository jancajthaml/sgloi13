/*
 * Matrix.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "./../core/CrossReferenceDispatcher.h"
#include "Vertex.h"

static const int SIZE_FLOAT = sizeof(float) << 4;

//TODO COMMENT !!!!!!!
//Matrix
// vectors in columns
//Attributes are slower
struct Matrix
{
	float matrix[16];

    Matrix()
    { std::fill_n(matrix, 16, 0.0f); }

	Matrix(const float* other)
	{ memcpy(matrix, other, SIZE_FLOAT); }

	Matrix(float m11, float m21, float m31, float m41, float m12, float m22, float m32, float m42, float m13, float m23, float m33, float m43, float m14, float m24, float m34, float m44)
	{
		matrix[0] = m11;	//0
		matrix[1] = m21;	//1
		matrix[2] = m31;	//2
		matrix[3] = m41;	//3

		matrix[4] = m12;	//4
		matrix[5] = m22;	//5
		matrix[6] = m32;	//6
		matrix[7] = m42;	//7

		matrix[8] = m13;	//8
		matrix[9] = m23;	//9
		matrix[10] = m33;	//10
		matrix[11] = m34;	//11

		matrix[12] = m14;	//12
		matrix[13] = m24;	//13
		matrix[14] = m34;	//14
		matrix[15] = m44;	//15
	}

	inline Vertex operator*(const Vertex & other)
	{
		Vertex res
		(
			other.x * matrix[0] + other.y * matrix[4] + other.z * matrix[8] + other.w * matrix[12],
			other.x * matrix[1] + other.y * matrix[5] + other.z * matrix[9] + other.w * matrix[13],
			other.x * matrix[2] + other.y * matrix[6] + other.z * matrix[10] + other.w * matrix[14],
			other.x * matrix[3] + other.y * matrix[7] + other.z * matrix[11] + other.w * matrix[15]
		);

		return res;
	}

	inline Matrix operator*(const Matrix & other) const
	{
		Matrix res = Matrix();
		for (int r = 0; r < 4; r++) {
		    for (int c = 0; c < 4; c++) {
		      res.matrix[r + (c<<2)] = 0.0f;

		      for (int i = 0; i < 4; i++) {
		        res.matrix[r + (c<<2)] += matrix[r + (i<<2)] * other.matrix[i + (c<<2)];
		      }
		    }
		  }

		return res;
	}

	inline void operator=(const Matrix & other)
	{ memcpy(matrix, other.matrix, SIZE_FLOAT); }

	inline Matrix operator+(const Matrix &other)const
	{
		Matrix res
		(
			this->matrix[0] + other.matrix[0],
			this->matrix[1] + other.matrix[1],
			this->matrix[2] + other.matrix[2],
			this->matrix[3] + other.matrix[3],

			this->matrix[4] + other.matrix[4],
			this->matrix[5] + other.matrix[5],
			this->matrix[6] + other.matrix[6],
			this->matrix[7] + other.matrix[7],

			this->matrix[8] + other.matrix[8],
			this->matrix[9] + other.matrix[9],
			this->matrix[10] + other.matrix[10],
			this->matrix[11] + other.matrix[11],

			this->matrix[12] + other.matrix[12],
			this->matrix[13] + other.matrix[13],
			this->matrix[14] + other.matrix[14],
			this->matrix[15] + other.matrix[15]
		);

		return res;
	}

	inline void operator+=(const Matrix &other)
	{
		matrix[0] += other.matrix[0];
		matrix[1] += other.matrix[1];
		matrix[2] += other.matrix[2];
		matrix[3] += other.matrix[3];
		matrix[4] += other.matrix[4];
		matrix[5] += other.matrix[5];
		matrix[6] += other.matrix[6];
		matrix[7] += other.matrix[7];
		matrix[8] += other.matrix[8];
		matrix[9] += other.matrix[9];
		matrix[10] += other.matrix[10];
		matrix[11] += other.matrix[11];
		matrix[12] += other.matrix[12];
		matrix[13] += other.matrix[13];
		matrix[14] += other.matrix[14];
		matrix[15] += other.matrix[15];
	}

	Matrix operator-(const Matrix &other)const
	{
		Matrix res
		(
			this->matrix[0] - other.matrix[0],
			this->matrix[1] - other.matrix[1],
			this->matrix[2] - other.matrix[2],
			this->matrix[3] - other.matrix[3],

			this->matrix[4] - other.matrix[4],
			this->matrix[5] - other.matrix[5],
			this->matrix[6] - other.matrix[6],
			this->matrix[7] - other.matrix[7],

			this->matrix[8] - other.matrix[8],
			this->matrix[9] - other.matrix[9],
			this->matrix[10] - other.matrix[10],
			this->matrix[11] - other.matrix[11],

			this->matrix[12] - other.matrix[12],
			this->matrix[13] - other.matrix[13],
			this->matrix[14] - other.matrix[14],
			this->matrix[15] - other.matrix[15]
		);

		return res;
	}

	inline void operator-=(const Matrix &other)
	{
		matrix[0] -= other.matrix[0];
		matrix[1] -= other.matrix[1];
		matrix[2] -= other.matrix[2];
		matrix[3] -= other.matrix[3];
		matrix[4] -= other.matrix[4];
		matrix[5] -= other.matrix[5];
		matrix[6] -= other.matrix[6];
		matrix[7] -= other.matrix[7];
		matrix[8] -= other.matrix[8];
		matrix[9] -= other.matrix[9];
		matrix[10] -= other.matrix[10];
		matrix[11] -= other.matrix[11];
		matrix[12] -= other.matrix[12];
		matrix[13] -= other.matrix[13];
		matrix[14] -= other.matrix[14];
		matrix[15] -= other.matrix[15];
	}
	
	void print()
	{
		printf("%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n",
			   matrix[0], matrix[4], matrix[8], matrix[12],
			   matrix[1], matrix[5], matrix[9], matrix[13],
			   matrix[2], matrix[6], matrix[10], matrix[14],
			   matrix[3], matrix[7], matrix[11], matrix[15]);
		
	}
};



#endif /* MATRIX_H_ */