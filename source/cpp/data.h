/*
 * data.h
 *
 *  Created on: 23.9.2013
 *      Author: jancajthaml
 */

#include <vector>
#include <list>
#include <cmath>
#include "sgl.h"
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
 * 30.9.2013, Jan Cajthaml - added Model,Projection,Current and MPV (Model-Projection-View pre-calculated) matricies to Context
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

    inline Color operator +(const Color &other)
    { return Color(*this) += other; }

    inline Color operator -(const Color &other)
    { return Color(*this) -= other; }

    Color& operator+=(const Color& other)
	{
        this->r = (other.r + this->r) * 0.5f;
        this->g = (other.g + this->g) * 0.5f;
        this->b = (other.b + this->b) * 0.5f;

        return *this;
    }

    Color& operator-=(const Color& other)
    {
    	this->r = (this->r - other.r) * 0.5f;
    	this->g = (this->g - other.g) * 0.5f;
    	this->b = (this->b - other.b) * 0.5f;

    	return *this;
    }

};

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
		res.x = (other.x*this->matrix[0]) + (other.y*this->matrix[1]) + (other.z*this->matrix[2]) + this->matrix[3];
		res.y = (other.x*this->matrix[4]) + (other.y*this->matrix[5]) + (other.z*this->matrix[6]) + this->matrix[7];
		res.z = (other.x*this->matrix[8]) + (other.y*this->matrix[9]) + (other.z*this->matrix[10]) + this->matrix[11];
		res.w = (other.x*this->matrix[12]) + (other.y*this->matrix[13]) + (other.z*this->matrix[14]) + this->matrix[15];

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
	{ return Matrix(	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

	static Matrix scale(float x, float y, float z, float w)
	{ return Matrix(x, 0.0f, 0.0f, 0.0f, 0.0f, y, 0.0f, 0.0f, 0.0f, 0.0f, z, 0.0f, 0.0f, 0.0f, 0.0f, w); }

	static Matrix rotate(float angle, float centerx, float centery)
	{ return Matrix(cos(angle), sin(angle), 0.0f, 0.0f, -sin(angle), cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

	static Matrix rotateY(float angle)
	{ return Matrix(cos(angle), 0.0f, sin(angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin(angle), 0.0f, cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

};



//---------------------------------------------------------------------------
// Helper Structures
//---------------------------------------------------------------------------

struct Viewport
{

	int width;
	int height;
	float width_2_x;
	float height_2_y;
	int x;
	int y;
	bool ready;

	Viewport()
	{
		width = 0;
		height = 0;
		width_2_x = 0.0f;
		height_2_y = 0.0f;
		x = 0;
		y = 0;
		ready = false;
	}

	Viewport(int width, int height, int x, int y)
	{
		changeViewport(width, height, x, y);
	}

	void calculateWindowCoordinates(Vertex & v)
	{
		v.x = (v.x + 1) * width_2_x;
		v.y = (v.y + 1) * height_2_y;
	}

	void changeViewport(int width, int height, int x, int y)
	{
		this->width			= width;
		this->height		= height;
		this->width_2_x		= (float)(width>>1) + x;
		this->height_2_y	= (float)(height>>1) + y;
		this->x				= x;
		this->y				= y;
		ready				= true;
	}

	//transform to window coordinates according to http://msdn.microsoft.com/en-us/library/dd374202(v=vs.85).aspx
	void normalize(float &x, float &y)
	{
	    x = (x+1)*width_2_x;
	    y = (y+1)*height_2_y;
	}
};

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
	int size;


	//Transformation matrices

	//Matrix model_view;
	//Matrix projection;

	Matrix current_mv;
	Matrix current_p;

	std::vector<Vertex> vertices;

	std::vector<sglEElementType> types;
	Viewport viewport;
	sglEMatrixMode matrixMode;

	std::vector<Matrix> projection;
	std::vector<Matrix> model_view;

	Context(int width, int height)
	{
		//----------------------//

		//Initialise Drawing
		w			= width;
		h			= height;
		id			= 0;
		//matrixMode	= NULL;

		// ? this shoud be static or const equivalent to NULL ?
		clear	= Color(255,0,0);

		// ? this shoud be static or const equivalent to NULL ?
		color	= Color(0,255,0);

		//----------------------//

		//Initialise Flags
		depth	= false;

		//----------------------//

		size	= 1;

		//this->width = width;
		//this->height = height;
		buffer = (float*) malloc(sizeof(float) * width * height);

		if (!buffer) throw std::exception();

		current_mv = Matrix::identity();
		current_p = Matrix::identity();
	}

	void setVertex2f(float x, float y)
	{
		Vertex v(x, y, 0.0f, 0.0f);
		v = (current_p * current_mv) * v;

		if (viewport.ready)
		{
			viewport.calculateWindowCoordinates(v);
		}
		else
		{
			v.x = x;
			v.y = y;
		}
		vertices.push_back(v);
	}

	void draw()
	{
		if (types.size() == 0) throw std::exception();

		sglEElementType type = types.back();
		types.pop_back();

		switch(type)
		{
			case SGL_POINTS				: drawPoints()		; break;
			case SGL_LINES				: drawLines()		; break;
			case SGL_LINE_STRIP			: drawLineStrip()	; break;
			case SGL_LINE_LOOP			: drawLineLoop()	; break;
			case SGL_TRIANGLES			: drawTriangles()	; break;
			case SGL_POLYGON			: drawPolygon()		; break;
			case SGL_AREA_LIGHT			: 					; break;
			case SGL_LAST_ELEMENT_TYPE	: 					; break;
			default : break;
		}

		vertices.clear();
	}

	void drawPoints()
	{
		if((size)%2==0) size++;
		int size = size<<1;

		if(size<100)
		{
			for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
				setPixel(v_it->x, v_it->y);
		}
		else
		{
			for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			for(int i = -size; i<size-1; i++)
			for(int j = -size; j<size-1; j++)
				setPixel(v_it->x+j, v_it->y+i);
		}
	}

	void drawCricle(float x, float y, float z, float r)
	{
		int p	= 1 - (int)r;
			int x0	= int(x);
			int y0	= int(y);
			int x1	= 0;
			int y1	= int(r);
			int x2	= 1;
			int y2	= -2*r;

			//4 points around center
			setPixel(x0        , y0+int(r));
			setPixel(x0        , y0-int(r));
			setPixel(x0+int(r) , y0);
			setPixel(x0-int(r) , y0);

			while(x1<y1)
			{
				if(p>=0)
				{
					y1--;
					y2 += 2;
					p += y2;
				}

				x1++;

				x2 += 2;
				p += x2;

				setPixel(x0+x1 , y0+y1);
				setPixel(x0-x1 , y0+y1);
				setPixel(x0+x1 , y0-y1);
				setPixel(x0-x1 , y0-y1);

				setPixel(x0+y1 , y0+x1);
				setPixel(x0-y1 , y0+x1);
				setPixel(x0+y1 , y0-x1);
				setPixel(x0-y1 , y0-x1);

			}
	}

	void drawEllipse(float x1, float y1, float z, float x2, float y2)
	{

		float x				= x2;
		float y				= 0;
		float EllipseError	= 0;
		float TwoASquare	= 2 * x2 * x2;
		float TwoBSquare	= 2 * y2 * y2;
		float XChange		= y2 * y2 * (1 - 2 * x2);
		float YChange		= x2 * x2;
		float StoppingX		= TwoBSquare * x2;
		float StoppingY		= 0;

		while( StoppingX >= StoppingY)
		{
			setPixel(x, y);

			y++;
			StoppingY		+= TwoASquare;
			EllipseError	+= YChange;
			YChange			+= TwoASquare;

			if( ( 2 * EllipseError + XChange) > 0 )
			{
				x--;
				StoppingX		-= TwoBSquare;
				EllipseError	+= XChange;
				XChange			+= TwoBSquare;
			}
		}

		x				= 0;
		y				= y2;
		XChange			= y2 * y2;
		YChange			= x2 * x2 * (1 - 2 * y2);
		EllipseError	= 0;
		StoppingX		= 0;
		StoppingY		= TwoASquare * y2;
		while (StoppingX <= StoppingY)
		{
			setPixel(x, y);

			x++;
			StoppingX		+= TwoBSquare;
			EllipseError	+= XChange;
			XChange			+= TwoBSquare;

			if( (2 * EllipseError + YChange) > 0)
			{
				y--;
				StoppingY		-= TwoASquare;
				EllipseError	+= YChange;
				YChange			+= TwoASquare;
			}
		}
	}

	void drawLineStrip()
	{
		for (int i = 0; i < (int)vertices.size()-1; i++)
			drawLine2D(vertices[i], vertices[i+1]);

		//for(unsigned int i = 0; i<vertexVector.size()-1; i++)
		  //          sglDrawLines(vertexVector[i], vertexVector[i+1]);

		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
	}

	void drawLineLoop()
	{
		for (int i = 0; i < (int)vertices.size()-1; i++)
		{
			drawLine2D(vertices[i], vertices[i+1]);

		}
		drawLine2D(vertices[(int)vertices.size()-1], vertices[0]);

		   //for(unsigned int i = 0; i<vertexVector.size()-1; i++){ //draw line strip, where one end point is start for second
		     //       sglDrawLines(vertexVector[i], vertexVector[i+1]);
		       // }
		        //sglDrawLines(vertexVector[vertexVector.size()-1], vertexVector[0]); //line loop where begin point must be connected with last

		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
	}

	void drawTriangles()
	{
		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
	//		setPixel(v_it->x, v_it->y);
	}

	void drawPolygon()
	{
		//if(SGL_LINE)
		//{
			for (int i = 0; i < (int)vertices.size()-1; i++)
			{
				drawLine2D(vertices[i], vertices[i+1]);
			}
		     //           sglDrawLines(vertexVector[i], vertexVector[i+1]);
		       //     }
			drawLine2D(vertices[(int)vertices.size()-1], vertices[0]);

//		            sglDrawLines(vertexVector[vertexVector.size()-1], vertexVector[0]);
		//}

		//if(currContext()->currentAreaMode==SGL_FILL)
		//{
			//sglDrawPolygon();
		//}
		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
	}

	void drawLines()
	{
		for (int i = 0; i < (int)vertices.size(); i += 2)
			drawLine2D(vertices[i], vertices[i+1]);
	}

	//Line
	//Breceanuv algoritmus
	//DDA algoritmus (jednoduzsi)
	//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
	void drawLine2D(Vertex a, Vertex b)
	{

		//obtain the points
		int x1, x2, y1, y2;
		x1 = (int)round(a.x);
		y1 = (int)round(a.y);
		x2 = (int)round(b.x);
		y2 = (int)round(b.y);

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		if (dx > dy)
			if (x1 < x2)
				bresenham_x(x1, y1, x2, y2);
			else
				bresenham_x(x2, y2, x1, y1);
		else
			if (y1 < y2)
				bresenham_y(y1, x1, y2, x2);
			else
				bresenham_y(y2, x2, y1, x1);

/*
		int x1		= int(start.x);
		int x2		= int(end.x);

		if (x1 > x2)
		{
			drawLine2D(end, start);
			return;
		}

		int y1		= int(start.y)		;
		int y2		= int(end.y)		;
		int slope						;
		int dx		= x2 - x1			;
		int dy		= y2 - y1			;

		if (dy < 0)
		{
			slope	= -1				;
			dy		= -dy				;
		}
		else slope	= 1					;

		int incE	= dy << 1			;
		int incNE	= (dy - dx) << 1	;
		int d		= (dy << 1) - dx	;
		int y		= y1				;
		int x							;

		for (x = x1; x <= x2; x++)
		{
			setPixel(x, y);

			if (d > 0)
			{
				d += incNE			;
				y += slope			;
			}
			else	d += incE			;
		}
		*/
	}

	void bresenham_x(int x1, int y1, int x2, int y2)
	{
			int dx = x2 - x1;
			int dy = y2 - y1;

			int sign = 1;
			if (dy < 0)
				sign = -1;
			int c0, c1, p;
			c0 = (dy << 1) * sign;
			c1 = c0 - (dx << 1);
			p = c0 - dx;

			setPixel(x1, y1);
			for (int i = x1 + 1; i <= x2; ++i)
			{
			if (p < 0)
				p += c0;
			else
			{
				p += c1;
				y1 += sign;
			}

			setPixel(i, y1);
		}
	}

	void bresenham_y(int x1, int y1, int x2, int y2)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		int sign = 1;
		if (dy < 0)
			sign = -1;
		int c0, c1, p;
		c0 = (dy << 1) * sign;
		c1 = c0 - (dx << 1);
		p = c0 - dx;

		setPixel(y1, x1);
		for (int i = x1 + 1; i <= x2; ++i)
		{
			if (p < 0)
				p += c0;
			else
			{
				p += c1;
				y1 += sign;
			}

			setPixel(y1, i);
		}
	}

	float round(float number)
	{ return floor(number + 0.5); }

	void setPixel(int x, int y)
	{
		if (x >= 0 && x < w && y >= 0 && y < h)
		{
			int offset = (x+w*y)*3;

			buffer[offset]		= color.r;
			buffer[offset+1]	= color.g;
			buffer[offset+2]	= color.b;
		}
	}



	void setPixel(float x, float y)
	{ setPixel((int)round(x), (int)round(y)); }

	int stackSize()
	{ return types.size(); }

	void multiplyCurrentMatrix(Matrix & m)
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= current_mv * m;
		else								current_p	*= m;
	}

	void setViewport(int width, int height, int x, int y)
	{ viewport.changeViewport(width, height, x, y); }

	std::vector<Matrix> & getCurrentMatrixStack()
	{
		if (matrixMode		== SGL_MODELVIEW)	return model_view;
		else if(matrixMode	== SGL_PROJECTION)	return projection;
		else throw std::exception();
	}

	void pushMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)	getCurrentMatrixStack().push_back(current_mv);
		else 								getCurrentMatrixStack().push_back(current_p);
	}

	void setCurrentMatrix(Matrix matrix)
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= matrix;
		else								current_p	= matrix;
	}

	void setMatrixMode(sglEMatrixMode mode)
	{ matrixMode = mode; }

	bool invalidTypeStack()
	{ return (types.size() > 0); }

	bool stackEmpty()
	{ return (getCurrentMatrixStack().size() == 0); }

	void popMatrix()
	{
		std::vector<Matrix> matrixStack = getCurrentMatrixStack();

		if (matrixMode == SGL_MODELVIEW)	current_mv	= matrixStack.back();
		else								current_p	= matrixStack.back();

		matrixStack.pop_back();
	}

	void pushTypeState(sglEElementType type)
	{ types.push_back(type); }

	void clearBuffer(unsigned what)
	{
		int size = w * h * 3;

		for(int i = 0; i<size; i += 3)
		{
			buffer[i]	= clear.r;
			buffer[i+1]	= clear.g;
			buffer[i+2]	= clear.b;
		}
		return;
	}
};

//Context Manager
struct ContextManager
{
    std::vector<Context*> contexts;
    int current;
};

#endif /* DATA_H_ */
