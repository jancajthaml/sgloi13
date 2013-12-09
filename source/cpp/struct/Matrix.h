/*
 * Matrix.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef MATRIX_H_
#define MATRIX_H_

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
		matrix[11] = m43;	//11
		
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
	
	inline Matrix operator*(const float &other) const
	{
		Matrix res
		(
		 this->matrix[0] * other,
		 this->matrix[1] * other,
		 this->matrix[2] * other,
		 this->matrix[3] * other,
		 
		 this->matrix[4] * other,
		 this->matrix[5] * other,
		 this->matrix[6] * other,
		 this->matrix[7] * other,
		 
		 this->matrix[8] * other,
		 this->matrix[9] * other,
		 this->matrix[10] * other,
		 this->matrix[11] * other,
		 
		 this->matrix[12] * other,
		 this->matrix[13] * other,
		 this->matrix[14] * other,
		 this->matrix[15] * other
		 );
		
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
	
    Matrix inverse()
    {
    	//Using LU determinant for 4x4 matricies
    	float k = 1.0f / this->determinant();

    	 //Save us some repetitious multiplications
    	 //FOUR TIMES
    	 float _10_15 = matrix[10] * matrix[15] ;
    	 float _8_15  = matrix[8]  * matrix[15] ;
    	 float _11_12 = matrix[11] * matrix[12] ;
    	 float _11_13 = matrix[11] * matrix[13] ;
    	 float _11_14 = matrix[11] * matrix[14] ;
    	 float _12_9  = matrix[12] * matrix[9]  ;
    	 float _15_9  = matrix[15] * matrix[9]  ;
    	 float _13_10 = matrix[13] * matrix[10] ;
    	 float _13_8  = matrix[13] * matrix[8]  ;
    	 float _14_7  = matrix[14] * matrix[7]  ;
    	 float _12_10 = matrix[12] * matrix[10] ;
    	 float _3_4   = matrix[3]  * matrix[4]  ;
    	 float _2_4   = matrix[2]  * matrix[4]  ;
    	 float _1_4   = matrix[1]  * matrix[4]  ;
    	 float _5_0   = matrix[5]  * matrix[0]  ;
    	 float _5_2   = matrix[5]  * matrix[2]  ;
    	 float _5_3   = matrix[5]  * matrix[3]  ;
    	 float _2_7   = matrix[2]  * matrix[7]  ;
    	 float _0_7   = matrix[0]  * matrix[7]  ;
    	 float _1_7   = matrix[1]  * matrix[7]  ;
    	 float _6_3   = matrix[6]  * matrix[3]  ;
    	 float _6_0   = matrix[6]  * matrix[0]  ;
    	 float _6_1   = matrix[6]  * matrix[1]  ;
    	 float _9_14  = matrix[9]  * matrix[14]  ;

    	 //Three TIMES
    	 float _14_8 = matrix[14] * matrix[8];

    	 Matrix inv = Matrix
    	 (
    	   k * ((matrix[5] * _10_15)  +  (matrix[6] * _11_13)  +  (_14_7 * matrix[9])  -  (_13_10 * matrix[7])  -  (_11_14 * matrix[5])  -  (_15_9 * matrix[6])),
    	  -k * ((matrix[1] * _10_15)  +  (matrix[2] * _11_13)  +  (matrix[3] * _9_14)  -  (_13_10 * matrix[3])  -  (_11_14 * matrix[1])  -  (_15_9 * matrix[2])),
    	   k * ((_6_1 * matrix[15])   +  (_2_7 * matrix[13])   +  (_5_3 * matrix[14])  -  (matrix[13] * _6_3)   -  (_14_7  * matrix[1])  -  (matrix[15] * _5_2)),
    	  -k * ((_6_1 * matrix[11])   +  (_2_7 * matrix[9])    +  (_5_3 * matrix[10])  -  (matrix[9]  * _6_3)   -  (matrix[10] * _1_7)   -  (matrix[11] * _5_2)),
    	  -k * ((matrix[4] * _10_15)  +  (matrix[6] * _11_12)  +  (_14_7 * matrix[8])  -  (_12_10 * matrix[7])  -  (_11_14 * matrix[4])  -  (_8_15 * matrix[6])),
    	   k * ((matrix[0] * _10_15)  +  (matrix[2] * _11_12)  +  (matrix[3] * _14_8)  -  (_12_10 * matrix[3])  -  (_11_14 * matrix[0])  -  (_8_15 * matrix[2])),
    	  -k * ((_6_0 * matrix[15])   +  (_2_7 * matrix[12])   +  (_3_4 * matrix[14])  -  (matrix[12] * _6_3)   -  (_14_7  * matrix[0])  -  (matrix[15] * _2_4)),
    	   k * ((_6_0 * matrix[11])   +  (_2_7 * matrix[8])    +  (_3_4 * matrix[10])  -  (matrix[8]  * _6_3)   -  (matrix[10] * _0_7)   -  (matrix[11] * _2_4)),
    	   k * ((matrix[4] * _15_9)   +  (matrix[5] * _11_12)  +  (matrix[7] * _13_8)  -  (_12_9 * matrix[7])   -  (_11_13 * matrix[4])  -  (_8_15 * matrix[5])),
    	  -k * ((matrix[0] * _15_9)   +  (matrix[1] * _11_12)  +  (matrix[3] * _13_8)  -  (_12_9 * matrix[3])   -  (_11_13 * matrix[0])  -  (_8_15 * matrix[1])),
    	   k * ((_5_0 * matrix[15])   +  (_1_7  * matrix[12])  +  (_3_4 * matrix[13])  -  (matrix[12] * _5_3)   -  (matrix[13] * _0_7)   -  (matrix[15] * _1_4)),
    	  -k * ((_5_0 * matrix[11])   +  (_1_7  * matrix[8])   +  (_3_4 * matrix[9])   -  (matrix[8]  * _5_3)   -  (matrix[9] * _0_7)    -  (matrix[11] * _1_4)),
    	  -k * ((matrix[4] * _9_14)   +  (matrix[5] * _12_10)  +  (matrix[6] * _13_8)  -  (_12_9 * matrix[6])   -  (_13_10 * matrix[4])  -  (_14_8 * matrix[5])),
    	   k * ((matrix[0] * _9_14)   +  (matrix[1] * _12_10)  +  (matrix[2] * _13_8)  -  (_12_9 * matrix[2])   -  (_13_10 * matrix[0])  -  (_14_8 * matrix[1])),
    	  -k * ((_5_0 * matrix[14])   +  (_6_1  * matrix[12])  +  (_2_4 * matrix[13])  -  (matrix[12] * _5_2)   -  (matrix[13] * _6_0)   -  (matrix[14] * _1_4)),
    	   k * ((_5_0 * matrix[10])   +  (_6_1  * matrix[8])   +  (_2_4 * matrix[9])   -  (matrix[8]  * _5_2)   -  (matrix[9] * _6_0)    -  (matrix[10] * _1_4))
    	 );

    	 return inv;
    }

    //Using LU Decomposition for determinant 4x4 (save method calls)
    float determinant()
    {
    	float LU[4][4];

    	LU[0][0] = matrix[0];
    	LU[0][1] = matrix[1];
    	LU[0][2] = matrix[2];
    	LU[0][3] = matrix[3];
    	LU[1][0] = matrix[4];
    	LU[1][1] = matrix[5];
    	LU[1][2] = matrix[6];
    	LU[1][3] = matrix[7];
    	LU[2][0] = matrix[8];
    	LU[2][1] = matrix[9];
    	LU[2][2] = matrix[10];
    	LU[2][3] = matrix[11];
    	LU[3][0] = matrix[12];
    	LU[3][1] = matrix[13];
    	LU[3][2] = matrix[14];
    	LU[3][3] = matrix[15];

    	int piv[4];
    	piv[0] = 0;
    	piv[1] = 1;
    	piv[2] = 2;
    	piv[3] = 3;

    	int sign= 1;
	    float t;
	    int p = 0;
	    int k = 0;

    	float C1;
    	float C2;
    	float C3;
    	float C4;

    	C1 = LU[0][0];
    	C2 = LU[1][0];
    	C3 = LU[2][0];
    	C4 = LU[3][0];

    	//Yup ... I know...
	    if (Helper::abs(C2) > Helper::abs(C1))	p = 1;
	    if (Helper::abs(C3) > Helper::abs(p==1?C2:C1))	p = 2;
	    if (Helper::abs(C4) > Helper::abs(p==1?C2:(p==2?C3:C1)))	p = 3;

    	if (p != 0)
    	{
    		t = LU[p][0];
    		LU[p][0] = LU[0][0];
    		LU[0][0] = t;

    		t = LU[p][1];
    		LU[p][1] = LU[0][1];
    		LU[0][1] = t;

    		t = LU[p][2];
    		LU[p][2] = LU[0][2];
    		LU[0][2] = t;

    		t = LU[p][3];
    		LU[p][3] = LU[0][3];
    		LU[0][3] = t;

    		k	= piv[p];
    		piv[p]	= piv[0];
    		piv[0]	= k;
    		sign	= -sign;
    	}

    	if( LU[0][0] != 0.0 )
    	{
    		float n = 1.0f/LU[0][0];
    		LU[1][0] *= n;
    		LU[2][0] *= n;
    		LU[3][0] *= n;
    	}

    	C1 = LU[0][1];
    	C2 = LU[1][1];
    	C3 = LU[2][1];
    	C4 = LU[3][1];

    	LU[0][1] = C1;
    	LU[1][1] = C2 -= LU[1][0]*C1;
    	LU[2][1] = C3 -= LU[2][0]*C1;
    	LU[3][1] = C4 -= LU[3][0]*C1;

    	if (Helper::abs(C2) > Helper::abs(C1))	p = 2;
    	if (Helper::abs(C3) > Helper::abs(p==2?C3:C1))	p = 3;

    	if( p!=1 )
    	{
    		t = LU[p][0];
    		LU[p][0] = LU[1][0];
    		LU[1][0] = t;

    		t = LU[p][1];
    		LU[p][1] = LU[1][1];
    		LU[1][1] = t;

    		t = LU[p][2];
    		LU[p][2] = LU[1][2];
    		LU[1][2] = t;

    		t = LU[p][3];
    		LU[p][3] = LU[1][3];
    		LU[1][3] = t;

    		k	= piv[p];
    		piv[p]	= piv[1];
    		piv[1]	= k;
    		sign		= -sign;
    	}

    	if( LU[1][1] != 0.0 )
    	{
    		float n = 1.0f/LU[1][1];
    		LU[2][1] *= n;
    		LU[3][1] *= n;
    	}

    	C1 = LU[0][2];
    	C2 = LU[1][2];
    	C3 = LU[2][2];
    	C4 = LU[3][2];

    	LU[0][2] = C1;
    	LU[1][2] = C2 -= LU[1][0]*C1;
    	LU[2][2] = C3 -= (LU[2][0]*C1)+(LU[2][1]*C2);
        LU[3][2] = C4 -= (LU[3][0]*C1)+(LU[3][1]*C2);

        if( Helper::abs(C4) > Helper::abs(C1) ) p = 3;

        if (p != 2)
        {
        	t = LU[p][0];
        	LU[p][0] = LU[2][0];
        	LU[2][0] = t;

        	t = LU[p][1];
        	LU[p][1] = LU[2][1];
        	LU[2][1] = t;

        	t = LU[p][2];
        	LU[p][2] = LU[2][2];
        	LU[2][2] = t;

        	t = LU[p][3];
        	LU[p][3] = LU[2][3];
        	LU[2][3] = t;

        	k	= piv[p];
        	piv[p]	= piv[2];
        	piv[2]	= k;
        	sign		= -sign;
        }

    	return sign * LU[0][0] * LU[1][1] * LU[2][2] * LU[3][3];
    }

};

#endif /* MATRIX_H_ */
