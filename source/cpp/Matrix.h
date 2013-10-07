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
struct Matrix
{
    float matrix[16];

    Matrix()
    {
    	std::fill_n(matrix, 16, 0.0f);
    }

	Matrix(const float* arr)
	{
		memcpy(matrix, arr, sizeof(float) << 2);
	}

	Matrix(float m11, float m21, float m31, float m41, float m12, float m22, float m32, float m42, float m13, float m23, float m33, float m43,  float m14, float m24, float m34, float m44)
	{
		matrix[0] = m11;
		matrix[1] = m21;
		matrix[2] = m31;
		matrix[3] = m41;

		matrix[4] = m12;
		matrix[5] = m22;
		matrix[6] = m32;
		matrix[7] = m42;

		matrix[8] = m13;
		matrix[9] = m23;
		matrix[10] = m33;
		matrix[11] = m43;

		matrix[12] = m14;
		matrix[13] = m24;
		matrix[14] = m34;
		matrix[15] = m44;
	}

    float& operator[](int i)
    { return matrix[i]; }

    const float& operator[](int i) const
    { return matrix[i]; }

    inline Matrix operator +(const Matrix &other)
    { return Matrix(*this) += other; }

    inline Matrix operator -(const Matrix &other)
    { return Matrix(*this) -= other; }

    inline Matrix operator *(const Matrix &other)
    { return Matrix(*this) *= other; }

    Vertex operator*(const Vertex& other)const
	{
		Vertex res;
		res.x = (other.x*this->matrix[0]) + (other.y*this->matrix[4]) + (other.z*this->matrix[8]) + (other.w*this->matrix[12]);
		res.y = (other.x*this->matrix[1]) + (other.y*this->matrix[5]) + (other.z*this->matrix[9]) + (other.w*this->matrix[13]);
		res.z = (other.x*this->matrix[2]) + (other.y*this->matrix[6]) + (other.z*this->matrix[10]) + (other.w*this->matrix[14]);
		res.w = (other.x*this->matrix[3]) + (other.y*this->matrix[7]) + (other.z*this->matrix[11]) + (other.w*this->matrix[15]);

		return res;
	}

    Matrix& operator+=(const Matrix& other)
	{
    	this->matrix[0]  += other.matrix[0];
    	this->matrix[1]  += other.matrix[1];
    	this->matrix[2]  += other.matrix[2];
    	this->matrix[3]  += other.matrix[3];
    	this->matrix[4]  += other.matrix[4];
    	this->matrix[5]  += other.matrix[5];
    	this->matrix[6]  += other.matrix[6];
    	this->matrix[7]  += other.matrix[7];
    	this->matrix[8]  += other.matrix[8];
    	this->matrix[9]  += other.matrix[9];
    	this->matrix[10] += other.matrix[10];
    	this->matrix[11] += other.matrix[11];
    	this->matrix[12] += other.matrix[12];
    	this->matrix[13] += other.matrix[13];
    	this->matrix[14] += other.matrix[14];
    	this->matrix[15] += other.matrix[15];

        return *this;
	}

    Matrix& operator-=(const Matrix& other)
	{
    	this->matrix[0]  -= other.matrix[0];
    	this->matrix[1]  -= other.matrix[1];
    	this->matrix[2]  -= other.matrix[2];
    	this->matrix[3]  -= other.matrix[3];
    	this->matrix[4]  -= other.matrix[4];
    	this->matrix[5]  -= other.matrix[5];
    	this->matrix[6]  -= other.matrix[6];
    	this->matrix[7]  -= other.matrix[7];
    	this->matrix[8]  -= other.matrix[8];
    	this->matrix[9]  -= other.matrix[9];
    	this->matrix[10] -= other.matrix[10];
    	this->matrix[11] -= other.matrix[11];
    	this->matrix[12] -= other.matrix[12];
    	this->matrix[13] -= other.matrix[13];
    	this->matrix[14] -= other.matrix[14];
    	this->matrix[15] -= other.matrix[15];

    	return *this;
    }

    Matrix& operator*=(const Matrix& other)
    {
    	// Row 1
    	this->matrix[0]  = this->matrix[0]  * other.matrix[0]  +  this->matrix[1]  * other.matrix[4]  +  this->matrix[2] * other.matrix[8]   +  this->matrix[3]  * other.matrix[12];    // Column 1
    	this->matrix[1]  = this->matrix[0]  * other.matrix[1]  +  this->matrix[1]  * other.matrix[5]  +  this->matrix[2] * other.matrix[9]   +  this->matrix[3]  * other.matrix[13];    // Column 2
    	this->matrix[2]  = this->matrix[0]  * other.matrix[2]  +  this->matrix[1]  * other.matrix[6]  +  this->matrix[2] * other.matrix[10]  +  this->matrix[3]  * other.matrix[14];    // Column 3
    	this->matrix[3]  = this->matrix[0]  * other.matrix[3]  +  this->matrix[1]  * other.matrix[7]  +  this->matrix[2] * other.matrix[11]  +  this->matrix[3]  * other.matrix[15];    // Column 4

    	// Row 2
    	this->matrix[4]  = this->matrix[4]  * other.matrix[0]  +  this->matrix[5]  * other.matrix[4]  +  this->matrix[6] * other.matrix[8]   +  this->matrix[7]  * other.matrix[12];    // Column 1
    	this->matrix[5]  = this->matrix[4]  * other.matrix[1]  +  this->matrix[5]  * other.matrix[5]  +  this->matrix[6] * other.matrix[9]   +  this->matrix[7]  * other.matrix[13];    // Column 2
    	this->matrix[6]  = this->matrix[4]  * other.matrix[2]  +  this->matrix[5]  * other.matrix[6]  +  this->matrix[6] * other.matrix[10]  +  this->matrix[7]  * other.matrix[14];    // Column 3
    	this->matrix[7]  = this->matrix[4]  * other.matrix[3]  +  this->matrix[5]  * other.matrix[7]  +  this->matrix[6] * other.matrix[11]  +  this->matrix[7]  * other.matrix[15];    // Column 4

    	// Row 3
    	this->matrix[8]  = this->matrix[8]  * other.matrix[0]  +  this->matrix[9]  * other.matrix[4]  +  this->matrix[10] * other.matrix[8]  +  this->matrix[11] * other.matrix[12];    // Column 1
    	this->matrix[9]  = this->matrix[8]  * other.matrix[1]  +  this->matrix[9]  * other.matrix[5]  +  this->matrix[10] * other.matrix[9]  +  this->matrix[11] * other.matrix[13];    // Column 2
    	this->matrix[10] = this->matrix[8]  * other.matrix[2]  +  this->matrix[9]  * other.matrix[6]  +  this->matrix[10] * other.matrix[10] +  this->matrix[11] * other.matrix[14];    // Column 3
    	this->matrix[11] = this->matrix[8]  * other.matrix[3]  +  this->matrix[9]  * other.matrix[7]  +  this->matrix[10] * other.matrix[11] +  this->matrix[11] * other.matrix[15];    // Column 4

    	// Row 4
    	this->matrix[12] = this->matrix[12] * other.matrix[0]  +  this->matrix[13] * other.matrix[4]  +  this->matrix[14] * other.matrix[8]  +  this->matrix[15] * other.matrix[12];    // Column 1
    	this->matrix[13] = this->matrix[12] * other.matrix[1]  +  this->matrix[13] * other.matrix[5]  +  this->matrix[14] * other.matrix[9]  +  this->matrix[15] * other.matrix[13];    // Column 2
    	this->matrix[14] = this->matrix[12] * other.matrix[2]  +  this->matrix[13] * other.matrix[6]  +  this->matrix[14] * other.matrix[10] +  this->matrix[15] * other.matrix[14];    // Column 3
    	this->matrix[15] = this->matrix[12] * other.matrix[3]  +  this->matrix[13] * other.matrix[7]  +  this->matrix[14] * other.matrix[11] +  this->matrix[15] * other.matrix[15];    // Column 4

        return *this;
    }

    inline void operator=(float* m)
    { memcpy(&matrix, m, sizeof(float) << 4 ); }

    inline void operator=(Matrix m)
    { memcpy(&matrix, m.matrix, sizeof(float) << 4 ); }

    inline void set(const Matrix &m)
    { *this = m; }


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

	/**
	 * Factory method for creating identity matrix
	 */

	void print()
	{
		printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", matrix[0], matrix[4], matrix[8], matrix[12], matrix[1], matrix[5], matrix[9], matrix[13], matrix[2], matrix[6], matrix[10], matrix[14], matrix[3], matrix[7], matrix[11], matrix[15]);
	}

};



#endif /* MATRIX_H_ */
