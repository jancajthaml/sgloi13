/*
 * data.h
 *
 *  Created on: 23.9.2013
 *      Author: jancajthaml
 */

#include <vector>
#include <list>
#ifndef DATA_H_
#define DATA_H_

/*
 * Side-notes:
 * Jan Cajthaml -using struct instead of class because of no need of encapsulation
 *
 * */

/*
 * ChangeLog:
 * 23.9.2013, Jan Cajthaml - added struct Vertex
 * 23.9.2013, Jan Cajthaml - added struct Edge
 * 24.9.2013, Jan Cajthaml - added struct Color
 * 24.9.2013, Jan Cajthaml - added struct Matrix
 * 24.9.2013, Jan Cajthaml - added struct Context
 * 25.9.2013, Jan Cajthaml - added ViewPort & depth flag to Context
 * 25.9.2013, Jan Cajthaml - added struct ContextManager
 * 30.9.2013, Jan Cajthaml - added operator overload for aritmetic operations on Matrix
 * 30.9.2013, Jan Cajthaml - added operator overload for aritmetic operations on Color
 * */

//---------------------------------------------------------------------------
// Data Structures
//---------------------------------------------------------------------------

//Vertex
struct Vertex
{
	float x;
	float y;
	float z;
	float w;

	Vertex()
	{ x = y = z = w = 0.0f; }

	Vertex(float X, float Y)
	{
		x = X;
		y = Y;
		z = w = 0.0f;
	}

	Vertex(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
		w = 0.0f;
	}

	Vertex(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
};

//Edge
struct Edge
{
	float	x_s;
	float	y_s;
	float	z_s;
	float	x_e;
	float	y_e;
	float	z_e;

    Edge()
    { x_s = y_s = z_s = x_e = y_e = z_e = 0.0f; }

    Edge(Vertex start, Vertex end)
	{
    	x_s	= start.x;
    	y_s	= start.y;
    	z_s	= start.z;
    	x_e	= end.x;
    	y_e	= end.y;
    	z_e	= end.z;
	}

};

//Color
struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color()
	{
		r = g = b = 0.0f;

		// ? is alpha channel 1 by default or 0 ?
		a = 1.0f;
	}

    Color(float R, float G, float B)
    {
    	r	= R;
    	g	= G;
    	b	= B;

    	// ? is alpha channel 1 by default or 0 ?
    	a	= 1.0f;
    }

    Color(float R, float G, float B, float A)
    {
    	r	= R;
    	g	= G;
    	b	= B;
    	a	= A;
    }


    Color* operator +(const Color &other)
      { return Color(*this) += other; }

    Color* operator -(const Color &other)
      { return Color(*this) -= other; }

    Color* operator+=(const Color& other)
  	{
        this->r = (other.r + this->r) * 0.5f;
        this->g = (other.g + this->g) * 0.5f;
        this->b = (other.b + this->b) * 0.5f;

        return &*this;
  	}

    Color* operator-=(const Color& other)
  	{
    	this->r = (this->r - other.r) * 0.5f;
    	this->g = (this->g - other.g) * 0.5f;
    	this->b = (this->b - other.b) * 0.5f;

      	return &*this;
      }
};

//Matrix
// vectors in columns
struct Matrix
{
    float matrix[16];

    float& operator[](int i)       			{ return matrix[i]; }
    const float& operator[](int i) const	{ return matrix[i]; }

    Matrix* operator *(const Matrix &other)
    { return Matrix(*this) *= other; }

    Matrix* operator +(const Matrix &other)
    { return Matrix(*this) += other; }

    Matrix* operator -(const Matrix &other)
    { return Matrix(*this) -= other; }

    Matrix* operator+=(const Matrix& other)
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

        return &*this;
	}

    Matrix* operator-=(const Matrix& other)
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

    	return &*this;
    }

    Matrix* operator*=(const Matrix& other)
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

    	return &*this;
    }

    void operator=(float* m)
    { memcpy(&matrix, m, sizeof(float) << 4 ); }

    void operator=(Matrix m)
    { memcpy(&matrix, m.matrix, sizeof(float) << 4 ); }

    void set(Matrix &m)
    { *this = m; }

    Matrix()
    {
    	std::fill_n(matrix, 16, 0.0f);
    }

};



//---------------------------------------------------------------------------
// Helper Structures
//---------------------------------------------------------------------------

//Context
struct Context
{
	int id;

    //Drawing
	float *buffer;
	int w;
	int h;
	Color clear;
	Color color;

	//State
	bool depth;

	//Pixel size
	float size;

	//Viewport
	float x;
	float y;
	float w2;
	float h2;
	int min_x;
	int min_y;
	int max_x;
	int max_y;

	//Transformation matrices
	Matrix model_view;
	Matrix projection;

	Context(int width, int height)
	{
		//----------------------//

		//Initialise Drawing
		w		= width;
		h		= height;
		id		= 0;
		buffer	= NULL;

		// ? this shoud be static or const equivalent to NULL ?
		clear	= Color(255,0,0);

		// ? this shoud be static or const equivalent to NULL ?
		color	= Color(0,255,0);

		//----------------------//

		//Initialise Viewport
		x		= 0.0f;
		y		= 0.0f;
		w2		= 0.0f;
		h2		= 0.0f;
		min_x	= 0;
		min_y	= 0;
		max_x	= 0;
		max_y	= 0;

		//----------------------//

		//Initialise Flags
		depth	= false;

		//----------------------//

		size	= 1;
	}
};

//Context Manager
struct ContextManager
{
    std::vector<Context*> contexts;
    int current;
};

#endif /* DATA_H_ */
