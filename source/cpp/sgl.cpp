//---------------------------------------------------------------------------
// sgl.cpp
//
// Implementation of the SGL (Simple Graphics Library)
// Year: 2013
// Author: Jan Cajthaml, Jan Brejcha
//
//
// based on:
// ## Empty implementation of the SGL (Simple Graphics Library)
// ## Date:  2011/11/1
// ## Author: Jaroslav Krivanek, Jiri Bittner CTU Prague
//---------------------------------------------------------------------------

/*
 * ChangeLog:
 * 23.9.2013, Jan Cajthaml - GNU code style & condition shorthand refactor
 *                         - Base comment of method function
 *                         - Added data.h
 * 24.9.2013, Jan Cajthaml - sglCircle proof of concept
 * 25.9.2013, Jan Cajthaml - Helper methods:
 *                                           in_range  - checks if(LOW<=x && x<=HIGH) withing one operation
 *                                           round     - round double to a nearest non decimal number
 *                                           current   - current Context pointer
 *                                           normalize - normalize coords to viewport
 *                                           setPixel  - sets pixel color
 *
 *                         - sglEllipse proof of concept based on fast Bresenham Type Algorithm
 * 29.9.2013, Jan Cajthaml - added method sglDrawLine()
 *                         - body of sglEnd and sglBegin
 *
 * 30.9.2013, Jan Cajthaml - added sglDrawLine Bresenham´s algorithm draw line implementation
 *
 * */

/*
 * useful links:
 * Bresenham's line algorithm - http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 * C++ Code optimalisation    - http://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Code_optimization/Faster_operations
 *
 */

#include "data.h"
#include <vector>

//---------------------------------------------------------------------------
// Helper functions forward declaration
//---------------------------------------------------------------------------

bool in_range(unsigned number, unsigned low, unsigned high);
Context* current();
//void normalize(float &x, float &y);
//void setPixel(int x, int y);

void sglDrawLine(Vertex start, Vertex end);

//---------------------------------------------------------------------------
// SGL
//---------------------------------------------------------------------------

// Current error code
static sglEErrorCode _libStatus = SGL_NO_ERROR;

// Global error code dispatch
static inline void setErrCode(sglEErrorCode c)
{
	if(_libStatus==SGL_NO_ERROR)
		_libStatus = c;
}

//---------------------------------------------------------------------------
// sglGetError()
//---------------------------------------------------------------------------
sglEErrorCode sglGetError(void)
{
	sglEErrorCode ret	= _libStatus;
	_libStatus			= SGL_NO_ERROR;
	return ret;
}

//---------------------------------------------------------------------------
// sglGetErrorString()
//---------------------------------------------------------------------------
const char* sglGetErrorString(sglEErrorCode error)
{
	static const char *errStrigTable[] =
	{
			"Operation succeeded",
			"Invalid argument(s) to a call",
			"Invalid enumeration argument(s) to a call",
			"Invalid call",
			"Quota of internal resources exceeded",
			"Internal library error",
			"Matrix stack overflow",
			"Matrix stack underflow",
			"Insufficient memory to finish the requested operation"
	};

	return (!in_range(error,SGL_NO_ERROR,SGL_OUT_OF_MEMORY))?"Invalid value passed to sglGetErrorString()":errStrigTable[int(error)];
}

//---------------------------------------------------------------------------
// Data attributes
//---------------------------------------------------------------------------

static ContextManager	manager;
//std::vector<Vertex>		VERTICES;
//std::vector<Edge>		EDGES;
//bool					isDrawing;


//---------------------------------------------------------------------------
// Initialization functions
//---------------------------------------------------------------------------

//LongName Function "global init ? run init ?"
void sglInit(void)
{
	//isDrawing = false;
}

//LongName Function "gloabl finalization ? run finalization ?"
void sglFinish(void)
{
	manager.contexts.clear();
}

//LongName Function
int sglCreateContext(int width, int height)
{
	int number_of_contexts = manager.contexts.size();

	Context *c = new Context(width, height);

	if(c == NULL)
	{
		setErrCode(SGL_OUT_OF_MEMORY);
		return -1;
	}

	manager.contexts.push_back(c);

	return number_of_contexts;
}

//LongName Function
// ? destroy or flush or release ?
void sglDestroyContext(int id)
{
	 if(in_range(id,0,manager.contexts.size()-1))
	 {
		 setErrCode(SGL_INVALID_VALUE);
		 return;
	 }

	 delete manager.contexts[id]->buffer;
	 delete manager.contexts[id];
}

//LongName Function
void sglSetContext(int id)
{
	 if(!in_range(id,0,manager.contexts.size()-1))
	 {
		 setErrCode(SGL_INVALID_VALUE);
		 return;
	 }

	 manager.current = id;
}

//LongName Function
int sglGetContext(void)
{
	if(manager.current < 0)
	{
		setErrCode(SGL_INVALID_OPERATION);
		return -1;
	}

	return manager.current;
}

//LongName Function
float *sglGetColorBufferPointer(void)
{
    return (float *) current()->buffer;
}

//---------------------------------------------------------------------------
// Drawing functions
//---------------------------------------------------------------------------

//Clears buffer with given RGBA color value
void sglClearColor (float r, float g, float b, float alpha)
{
	current()->clear.r = r;
    current()->clear.g = g;
    current()->clear.b = b;
}

//LongName Function
void sglClear(unsigned what)
{
	if (current()->invalidTypeStack())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	if (((what & SGL_COLOR_BUFFER_BIT) != SGL_COLOR_BUFFER_BIT) && ((what & SGL_DEPTH_BUFFER_BIT) != SGL_DEPTH_BUFFER_BIT))
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}

	if ((what & SGL_COLOR_BUFFER_BIT) == SGL_COLOR_BUFFER_BIT)
	{
		current()->clearBuffer(what);
	}

	if ((what & SGL_DEPTH_BUFFER_BIT) == SGL_DEPTH_BUFFER_BIT)
	{
			//TODO clear with depth buffer bit
	}

	setErrCode(SGL_NO_ERROR);

}

//LongName Function
void sglBegin(sglEElementType mode)
{
	if(mode>SGL_LAST_ELEMENT_TYPE)
	{
		setErrCode(SGL_INVALID_ENUM);
		return;
	}

	current()->pushTypeState(mode);
}

//LongName Function
void sglEnd(void)
{ current()->draw(); }

//Vertex with 3 float coords in homogenous coordinates
//[x,y,z,w]
//
void sglVertex4f(float x, float y, float z, float w)
{
	//normalize(x, y);
	//VERTICES.push_back(Vertex (x, y, z, w));
}

//Vertex with 3 float coords
//[x,y,z]
void sglVertex3f(float x, float y, float z)
{
	//normalize(x, y);
	//VERTICES.push_back(Vertex (x, y, z));
}

//Vertex with 2 float coords
//[x,y]
void sglVertex2f(float x, float y)
{ current()->setVertex2f(x,y); }

//2D Circle
//
// x	- center.x
// y	- center.y
// z	- ? IS IT 2D OR 3D CIRCLE ?
// r	- radius
//
// Using "Midpoint circle algorithm" @see http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void sglCircle(float x, float y, float z, float r)
{ current()->drawCricle(x,y,z,r); }

//2D Ellipse
//
// x	- center.x
// y	- center.y
// z	- ? IS IT 2D OR 3D ELLIPSE ?
// a	- ? width ?
// b	- ? height ?
//
// for adaptive fast Ellipse algorithm:
// - Cholesky decomposition
// - Bresenham Algorithm
//
//@see http://www.codeproject.com/Messages/2112010/A-Fast-Bresenham-Type-Algorithm-For-Drawing-Ellips.aspx
void sglEllipse(float x1, float y1, float z, float x2, float y2)
{ current()->drawEllipse(x1,y1,z,x2,y2); }

//Line
//Breceanuv algoritmus
//DDA algoritmus (jednoduzsi)
//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
void sglDrawLine(Vertex start, Vertex end)
{
	current()->drawLine2D(start, end);
}

//Line
void sglDrawPolygon(float x1, float y1, float z, float x2, float y2)
{

}


//2D Arc
//
// x	- center.x
// y	- center.y
// r	- radius
// from	- ?
// to	- ?
//
// ? use circle subdivision ?
void sglArc(float x, float y, float z, float r, float from, float to)
{

}

//---------------------------------------------------------------------------
// Transform functions
//---------------------------------------------------------------------------

// Function name here
//
// modes:
//        SGL_MODELVIEW  - desc
//        SGL_PROJECTION - desc
//
// ? we need something to hold on context ?
void sglMatrixMode( sglEMatrixMode mode )
{
	switch(mode)
	{
		case SGL_MODELVIEW	:
		case SGL_PROJECTION	:
		{
			current()->setMatrixMode(mode);
		}
		break;

		default				: setErrCode(SGL_INVALID_ENUM); break;
	}
}

//Push Matrix into transformation Stack
void sglPushMatrix(void)
{
	if(current()->invalidTypeStack())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	current()->pushMatrix();
}

//Pop Matrix from transformation Stack
void sglPopMatrix(void)
{
	if (current()->stackEmpty())
	{
		setErrCode(SGL_STACK_UNDERFLOW);
		return;
	}

	current()->popMatrix();
}

// ? Load identity matrix ?
void sglLoadIdentity(void)
{ current()->setCurrentMatrix(Matrix::identity()); }

// ? Load matrix where ? global ? stack ?
void sglLoadMatrix(const float* matrix)
{ current()->setCurrentMatrix(Matrix(matrix)); }

//Multiply two matrices
//
// ? Formula for matrix multiplication here ?
//
//  | a b c d |     | 1   2  3  4 |   | a*1+b*5+c*9+d*13  a*2+b*6+c*10+d*14
//  | e f g h |  X  | 5   6  7  8 | = |
//  | i j k l |     | 9  10 11 12 |   |
//  | m n o p |     | 13 14 15 16 |   |
//
void sglMultMatrix(const float* matrix)
{
	Matrix mat = Matrix(matrix);
	current()->multiplyCurrentMatrix(mat);
}

//Translate coordinates
//
// ? fotmula for matrix by vector multiplication in terms of vertex translation here ?
void sglTranslate(float x, float y, float z)
{
//	current()->translate(x,y,z);
	Matrix translate = Matrix::translate(x,y,z);
	current()->multiplyCurrentMatrix(translate);
}

// ? Scales what? Context or scene ?
void sglScale(float scalex, float scaley, float scalez)
{

	Matrix scale = Matrix::scale(scalex, scaley, scalez, 1.0f);
	current()->multiplyCurrentMatrix(scale);
}

//Rotate **** around the centerx,centery axis with given angle
//
// ? rotates what ? Context or scene ?
void sglRotate2D(float angle, float centerx, float centery)
{
	Matrix rotate = Matrix::rotate(angle, centerx, centery);
	current()->multiplyCurrentMatrix(rotate);
}

// ? rotates what ? Context or scene ?
// ? around what Y axis? Base or context?
void sglRotateY(float angle)
{
	Matrix rotate = Matrix::rotateY(angle);
	current()->multiplyCurrentMatrix(rotate);
}

// ?
void sglOrtho(float left, float right, float bottom, float top, float near, float far)
{
	if(current()->invalidTypeStack())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	Matrix ortho(2/(right - left), 0, 0, 0, 0, 2/(top-bottom), 0, 0, 0, 0, -2/(far-near), 0, -((right+left)/(right-left)), -((top+bottom)/(top-bottom)), -((far+near)/(far-near)), 1);
	current()->multiplyCurrentMatrix(ortho);
}

// ?
void sglFrustum(float left, float right, float bottom, float top, float near, float far)
{
	if(current()->invalidTypeStack())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	Matrix frustum((2*near)/(right-left), 0, 0, 0, 0, (2*near)/(top-bottom), 0, 0, (right+left)/(right-left), (top+bottom)/(top-bottom), -(far+near)/(far-near), -1.0f, 0, 0, -(2.0f*far*near)/(far-near), 1);
	current()->multiplyCurrentMatrix(frustum);
}

//Sets scene viewport
void sglViewport(int x, int y, int width, int height)
{
	if(current()->invalidTypeStack())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	current()->setViewport(width, height, x, y);
}

//---------------------------------------------------------------------------
// Attribute functions
//---------------------------------------------------------------------------

//RGB Color
void sglColor3f(float r, float g, float b)
{ current()->color = Color(r, g, b); }

// ?
void sglAreaMode(sglEAreaMode mode)
{
	if(mode>SGL_FILL)
	{
		setErrCode(SGL_INVALID_ENUM);
		return;
	}
}

//Point "radius/diameter ?"
void sglPointSize(float size)
{
    if(size <= 0.0f)
    {
    	setErrCode(SGL_INVALID_VALUE);
    	return;
    }

    current()->size = size;
}

//Enable given flag
void sglEnable(sglEEnableFlags cap)
{

}

//Disable given flag
void sglDisable(sglEEnableFlags cap)
{

}

//---------------------------------------------------------------------------
// RayTracing oriented functions
//---------------------------------------------------------------------------

// ? Begin of what scene ? whole scene or sub scene ?
void sglBeginScene()
{

}

// ? End of what scene ? whole scene or sub scene ?
void sglEndScene()
{

}

//3D Sphere
//
// x	- center.x
// y	- center.y
// z	- center.z
// r	- radius
void sglSphere(const float x, const float y, const float z, const float r)
{

}

//Material ******
void sglMaterial(const float r, const float g, const float b, const float kd, const float ks, const float shine, const float T, const float ior)
{

}

//Point Light
//
// ? x,y,z base coords r,g,b color and where is the direction ?
void sglPointLight(const float x, const float y, const float z, const float r, const float g, const float b)
{

}

// ?
void sglRayTraceScene()
{

}

// ? fragmentation -> rasterization ? direct rasterization ? evaluation ? bad name for a function !
void sglRasterizeScene()
{

}

// ? image or fragment(s) ? or fragment set ?
void sglEnvironmentMap(const int width, const int height, float *texels)
{

}

// ?
void sglEmissiveMaterial(const float r, const float g, const float b, const float c0, const float c1, const float c2)
{

}

//---------------------------------------------------------------------------
// Helper functions
//---------------------------------------------------------------------------

//Check if number is in range
//@see http://stackoverflow.com/questions/17095324/fastest-way-in-c-to-determine-if-an-integer-is-between-two-integers-inclusive
bool in_range(unsigned number, unsigned low, unsigned high)
{ return ((unsigned)(number-low) <= (high-low)); }

//Fast double round (3 times faster than std::round because of absence of EDOM)
//Type safe
double round(double x)
{
	return double((x>=0.5)?(int(x)+1):int(x));
}




Context* current()
{ return manager.contexts[manager.current]; }

