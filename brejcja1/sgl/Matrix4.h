#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vertex.h"
#include <cstdio>
#include <cstring>
#include <exception>
#include <cmath>

#define MATRIX4_LENGTH 16
#define MATRIX4_ROW_LENGTH 4
	struct Matrix4
	{
		float m[MATRIX4_LENGTH];

		Matrix4()
		{
			for (int i = 0; i < MATRIX4_LENGTH; i++)
				m[i] = 0.0f;
	}

	Matrix4(float m11, float m21, float m31, float m41,
		float m12, float m22, float m32, float m42,
		float m13, float m23, float m33, float m43, 
		float m14, float m24, float m34, float m44)
	{
		m[0] = m11;
		m[1] = m21;
		m[2] = m31;
		m[3] = m41;

		m[4] = m12;
		m[5] = m22;
		m[6] = m32;
		m[7] = m42;

		m[8] = m13;
		m[9] = m23;
		m[10] = m33;
		m[11] = m43;

		m[12] = m14;
		m[13] = m24;
		m[14] = m34;
		m[15] = m44;
	}		
	
	/**
	 * Initalizes 4x4 matrix from an array
	 * \param [in] arr 	the array from which will be this matrix initialized
	 * Elements in this array must be sorted column-wise.
	 */

	Matrix4(const float * arr)
	{
		memcpy(m, arr, sizeof(float) * MATRIX4_LENGTH);	
	}

	float operator[](const int i)const
	{
		if (i >=0 && i < MATRIX4_LENGTH)
			return m[i];
		else
			throw std::exception();
	}
	/*
	Vertex operator*(const Vertex & other)const
	{	
		Vertex res;
		for (int k = 0; k < VERTEX4_LENGTH; ++k)
		{
			for (int i = 0; i < VERTEX4_LENGTH; ++i)
			{
				res.v[k] += m[(i << 2) + k] * other.v[i]; 
			}	
		}	
		return res;
	}*/

	Vertex operator*(const Vertex & other)
	{
		Vertex res;
		res.v[0] = m[0] * other.v[0] + m[4] * other.v[1] + m[8] * other.v[2] + m[12] * other.v[3];	
		res.v[1] = m[1] * other.v[0] + m[5] * other.v[1] + m[9] * other.v[2] + m[13] * other.v[3];	
		res.v[2] = m[2] * other.v[0] + m[6] * other.v[1] + m[10] * other.v[2] + m[14] * other.v[3];	
		res.v[3] = m[3] * other.v[0] + m[7] * other.v[1] + m[11] * other.v[2] + m[15] * other.v[3];	
		return res;
	}

	/*Matrix4 operator*(const Matrix4 & other)const
	{
		Matrix4 result;
		for (int k = 0; k < MATRIX4_LENGTH; ++k)
		{
			for (int i = 0; i < MATRIX4_ROW_LENGTH; ++i)
			{
				result.m[k] += m[(i << 2) + (k % 4)] * other[i + MATRIX4_ROW_LENGTH * (k / 4)];
			}
		}
		return result;
	}*/

	Matrix4 operator*(const Matrix4 & other) const
	{
		Matrix4 res;
		res.m[0] = m[0] * other.m[0] + m[4] * other.m[1] + m[8] * other.m[2] + m[12] * other.m[3];
		res.m[1] = m[1] * other.m[0] + m[5] * other.m[1] + m[9] * other.m[2] + m[13] * other.m[3];
		res.m[2] = m[2] * other.m[0] + m[6] * other.m[1] + m[10] * other.m[2] + m[14] * other.m[3];
		res.m[3] = m[3] * other.m[0] + m[7] * other.m[1] + m[11] * other.m[2] + m[15] * other.m[3];

		res.m[4] = m[0] * other.m[4] + m[4] * other.m[5] + m[8] * other.m[6] + m[12] * other.m[7];
		res.m[5] = m[1] * other.m[4] + m[5] * other.m[5] + m[9] * other.m[6] + m[13] * other.m[7];
		res.m[6] = m[2] * other.m[4] + m[6] * other.m[5] + m[10] * other.m[6] + m[14] * other.m[7];
		res.m[7] = m[3] * other.m[4] + m[7] * other.m[5] + m[11] * other.m[6] + m[15] * other.m[7];

		res.m[8] = m[0] * other.m[8] + m[4] * other.m[9] + m[8] * other.m[10] + m[12] * other.m[11];
		res.m[9] = m[1] * other.m[8] + m[5] * other.m[9] + m[9] * other.m[10] + m[13] * other.m[11];
		res.m[10] = m[2] * other.m[8] + m[6] * other.m[9] + m[10] * other.m[10] + m[14] * other.m[11];
		res.m[11] = m[3] * other.m[8] + m[7] * other.m[9] + m[11] * other.m[10] + m[15] * other.m[11];

		res.m[12] = m[0] * other.m[12] + m[4] * other.m[13] + m[8] * other.m[14] + m[12] * other.m[15];
		res.m[13] = m[1] * other.m[12] + m[5] * other.m[13] + m[9] * other.m[14] + m[13] * other.m[15];
		res.m[14] = m[2] * other.m[12] + m[6] * other.m[13] + m[10] * other.m[14] + m[14] * other.m[15];
		res.m[15] = m[3] * other.m[12] + m[7] * other.m[13] + m[11] * other.m[14] + m[15] * other.m[15];
		return res;
	}
	
	void operator=(const Matrix4 & other)
	{
		memcpy(m, other.m, sizeof(float) << MATRIX4_ROW_LENGTH);
	}

	bool operator==(const Matrix4 & other)const
	{
		for (int i = 0; i < MATRIX4_LENGTH; ++i)
		{
			if (m[i] != other.m[i])
				return false;
		}
		return true;
	}	

	bool operator!=(const Matrix4 & other)const
	{
		return !(*this == other);
	}

	Matrix4 operator+(const Matrix4 &other)const
	{
		Matrix4 res;
		for (int i = 0; i < MATRIX4_LENGTH; ++i)
		{
			res.m[i] = this->m[i] + other.m[i];
		}
		return res;
	}

	void operator+=(const Matrix4 &other)
	{
		for (int i = 0; i < MATRIX4_LENGTH; ++i)
		{
			m[i] += other.m[i];
		}
	}
	
	Matrix4 operator-(const Matrix4 &other)const
	{
		Matrix4 res;
		for (int i = 0; i < MATRIX4_LENGTH; ++i)
		{
			res.m[i] = this->m[i] - other.m[i];
		}
		return res;
	}

	void operator-=(const Matrix4 &other)
	{
		for (int i = 0; i < MATRIX4_LENGTH; ++i)
		{
			m[i] -= other.m[i];
		}
	}
	


	void print()
	{
		printf("%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n",
			m[0], m[4], m[8], m[12],
			m[1], m[5], m[9], m[13],
			m[2], m[6], m[10], m[14],
			m[3], m[7], m[11], m[15]);

	}
	
	/**
	 * Factory method for creating identity matrix
	 */
	static Matrix4 makeIdentity()
	{
		return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
	}

	static Matrix4 makeScale(float sc_x, float sc_y, float sc_z)
	{
		return Matrix4( sc_x, 0.0f, 0.0f, 0.0f,
			       	0.0f, sc_y, 0.0f, 0.0f,
			       	0.0f, 0.0f, sc_z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
	}
	static Matrix4 makeTranslation(float t_x, float t_y, float t_z)
	{
		return Matrix4(	1.0f, 0.0f, 0.0f, 0.0f ,
				0.0f, 1.0f, 0.0f, 0.0f ,
				0.0f, 0.0f, 1.0f, 0.0f ,
				t_x, t_y, t_z, 1.0f);
	};

	static Matrix4 makeRotation2D(float angle, float c_x, float c_y)
	{
		float cos_a = cos(angle);
		float sin_a = sin(angle);
		return Matrix4(	cos_a, sin_a, 0.0f, 0.0f ,
				-sin_a, cos_a, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f ,
				0.0f, 0.0f, 0.0f, 1.0f);
		
	}
	

};


#endif
