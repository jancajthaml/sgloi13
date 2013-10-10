/*
 * Matrix.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef MATRIX_H_
#define MATRIX_H_

//Matrix
// vectors in columns
//Attributes are slower
struct Matrix
{
	float matrix[16];



    Matrix()
    {
    	std::fill_n(matrix, 16, 0.0f);
    }

	Matrix(const float* other)
	{
		matrix[0] = other[0];	//0
		matrix[1] = other[1];	//1
		matrix[2] = other[2];	//2
		matrix[3] = other[3];	//3

		matrix[4] = other[4];	//4
		matrix[5] = other[5];	//5
		matrix[6] = other[6];	//6
		matrix[7] = other[7];	//7

		matrix[8] = other[8];	//8
		matrix[9] = other[9];	//9
		matrix[10] = other[10];	//10
		matrix[11] = other[11];	//11

		matrix[12] = other[12];	//12
		matrix[13] = other[13];	//13
		matrix[14] = other[14];	//14
		matrix[15] = other[15];	//15

		//memcpy(matrix, arr, sizeof(float) << 2);
	}

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

	/**
	 * Initalizes 4x4 matrix from an array
	 * \param [in] arr 	the array from which will be this matrix initialized
	 * Elements in this array must be sorted column-wise.
	 */

	float operator[](const int i)const
	{
		//if (i >=0 && i < 16)
			return matrix[i];
		//else
			//throw std::exception();
	}
	/*
	Vertex operator*(const Vertex & other)const
	{
		Vertex res;
		for (int k = 0; k < VERTEX4_LENGTH; ++k)
		{
			for (int i = 0; i < VERTEX4_LENGTH; ++i)
			{
				res.v[k] += matrix[(i << 2) + k] * other.v[i];
			}
		}
		return res;
	}*/

	Vertex operator*(const Vertex & other)
	{
			Vertex res;
			res.x = matrix[0] * other.x + matrix[4] * other.y + matrix[8] * other.z + matrix[12] * other.w;
			res.y = matrix[1] * other.x + matrix[5] * other.y + matrix[9] * other.z + matrix[13] * other.w;
			res.z = matrix[2] * other.x + matrix[6] * other.y + matrix[10] * other.z + matrix[14] * other.w;
			res.w = matrix[3] * other.x + matrix[7] * other.y + matrix[11] * other.z + matrix[15] * other.w;
			return res;
	}

	/*Matrix operator*(const Matrix & other)const
	{
		Matrix result;
		for (int k = 0; k < 16; ++k)
		{
			for (int i = 0; i < 4; ++i)
			{
				result.matrix[k] += matrix[(i << 2) + (k % 4)] * other[i + 4 * (k / 4)];
			}
		}
		return result;
	}*/

	Matrix operator*(const Matrix & other) const
	{
			Matrix res;
			res.matrix[0] = matrix[0] * other.matrix[0] + matrix[4] * other.matrix[1] + matrix[8] * other.matrix[2] + matrix[12] * other.matrix[3];
			res.matrix[1] = matrix[1] * other.matrix[0] + matrix[5] * other.matrix[1] + matrix[9] * other.matrix[2] + matrix[13] * other.matrix[3];
			res.matrix[2] = matrix[2] * other.matrix[0] + matrix[6] * other.matrix[1] + matrix[10] * other.matrix[2] + matrix[14] * other.matrix[3];
			res.matrix[3] = matrix[3] * other.matrix[0] + matrix[7] * other.matrix[1] + matrix[11] * other.matrix[2] + matrix[15] * other.matrix[3];

			res.matrix[4] = matrix[0] * other.matrix[4] + matrix[4] * other.matrix[5] + matrix[8] * other.matrix[6] + matrix[12] * other.matrix[7];
			res.matrix[5] = matrix[1] * other.matrix[4] + matrix[5] * other.matrix[5] + matrix[9] * other.matrix[6] + matrix[13] * other.matrix[7];
			res.matrix[6] = matrix[2] * other.matrix[4] + matrix[6] * other.matrix[5] + matrix[10] * other.matrix[6] + matrix[14] * other.matrix[7];
			res.matrix[7] = matrix[3] * other.matrix[4] + matrix[7] * other.matrix[5] + matrix[11] * other.matrix[6] + matrix[15] * other.matrix[7];

			res.matrix[8] = matrix[0] * other.matrix[8] + matrix[4] * other.matrix[9] + matrix[8] * other.matrix[10] + matrix[12] * other.matrix[11];
			res.matrix[9] = matrix[1] * other.matrix[8] + matrix[5] * other.matrix[9] + matrix[9] * other.matrix[10] + matrix[13] * other.matrix[11];
			res.matrix[10] = matrix[2] * other.matrix[8] + matrix[6] * other.matrix[9] + matrix[10] * other.matrix[10] + matrix[14] * other.matrix[11];
			res.matrix[11] = matrix[3] * other.matrix[8] + matrix[7] * other.matrix[9] + matrix[11] * other.matrix[10] + matrix[15] * other.matrix[11];

			res.matrix[12] = matrix[0] * other.matrix[12] + matrix[4] * other.matrix[13] + matrix[8] * other.matrix[14] + matrix[12] * other.matrix[15];
			res.matrix[13] = matrix[1] * other.matrix[12] + matrix[5] * other.matrix[13] + matrix[9] * other.matrix[14] + matrix[13] * other.matrix[15];
			res.matrix[14] = matrix[2] * other.matrix[12] + matrix[6] * other.matrix[13] + matrix[10] * other.matrix[14] + matrix[14] * other.matrix[15];
			res.matrix[15] = matrix[3] * other.matrix[12] + matrix[7] * other.matrix[13] + matrix[11] * other.matrix[14] + matrix[15] * other.matrix[15];
			return res;
	}

	void operator=(const Matrix & other)
	{
		memcpy(matrix, other.matrix, sizeof(float) << 4);
	}


	bool operator==(const Matrix & other)const
		{
			for (int i = 0; i < 16; ++i)
			{
				if (matrix[i] != other.matrix[i])
					return false;
			}
			return true;
		}

	bool operator!=(const Matrix & other)const
	{
		return !(*this == other);
	}

	Matrix operator+(const Matrix &other)const
	{
		Matrix res;

		for (int i = 0; i < 16; ++i)
		{
			res.matrix[i] = this->matrix[i] + other.matrix[i];
		}
		return res;
	}

	void operator+=(const Matrix &other)
	{
		for (int i = 0; i < 16; ++i)
		{
			matrix[i] += other.matrix[i];
		}
	}

	Matrix operator-(const Matrix &other)const
	{
		Matrix res;
		for (int i = 0; i < 16; ++i)
		{
			res.matrix[i] = this->matrix[i] - other.matrix[i];
		}
		return res;
	}

	void operator-=(const Matrix &other)
	{
		for (int i = 0; i < 16; ++i)
		{
			matrix[i] -= other.matrix[i];
		}
	}

	/**
	 * Factory method for creating identity matrix
	 */
	static Matrix identity()
	{
		return Matrix(	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
	}

	static Matrix scale(float sc_x, float sc_y, float sc_z)
	{
		return Matrix( sc_x, 0.0f, 0.0f, 0.0f,
			       	0.0f, sc_y, 0.0f, 0.0f,
			       	0.0f, 0.0f, sc_z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
	}
	static Matrix translate(float t_x, float t_y, float t_z)
	{
		return Matrix(	1.0f, 0.0f, 0.0f, 0.0f ,
				0.0f, 1.0f, 0.0f, 0.0f ,
				0.0f, 0.0f, 1.0f, 0.0f ,
				t_x, t_y, t_z, 1.0f);
	};

	static Matrix rotate2D(float angle, float c_x, float c_y)
	{
		float cos_a = cos(angle);
		float sin_a = sin(angle);
		return Matrix(	cos_a, -sin_a, 0.0f, 0.0f ,
				sin_a, cos_a, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f ,
				0.0f, 0.0f, 0.0f, 1.0f);

	}

	static Matrix rotateY(float angle)
	{
		return identity();
	}


};



#endif /* MATRIX_H_ */
