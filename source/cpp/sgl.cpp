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
 * 23.9.2013, Jan Cajthaml - Base comment of method function
 * 23.9.2013, Jan Cajthaml - Added data.h
 * 24.9.2013, Jan Cajthaml - sglCircle proof of concept
 * 25.9.2013, Jan Cajthaml - Helper methods:
 *                                           in_range  - checks if(LOW<=x && x<=HIGH) withing one operation
 *                                           round     - round double to a nearest non decimal number
 *                                           current   - current Context pointer
 *
 * */

/*
 * useful links:
 * Bresenham's line algorithm - http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 * C++ Code optimalisation    - http://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Code_optimization/Faster_operations
 *
 */

#include "sgl.h"
#include "data.h"
#include <vector>

//---------------------------------------------------------------------------
// Helper functions forward declaration
//---------------------------------------------------------------------------

bool in_range(unsigned number, unsigned low, unsigned high);
Context* current();

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

	return ((int)error<(int)SGL_NO_ERROR || (int)error>(int)SGL_OUT_OF_MEMORY )?"Invalid value passed to sglGetErrorString()":errStrigTable[(int)error];
}

//---------------------------------------------------------------------------
// Data attributes
//---------------------------------------------------------------------------

static ContextManager	manager;
std::vector<Vertex>		VERTICES;
std::vector<Edge>		EDGES;

//---------------------------------------------------------------------------
// Initialization functions
//---------------------------------------------------------------------------

//LongName Function "global init ? run init ?"
void sglInit(void)
{
}

//LongName Function "gloabl finalization ? run finalization ?"
void sglFinish(void)
{

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
	if(what != SGL_COLOR_BUFFER_BIT)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}

	if(what == SGL_COLOR_BUFFER_BIT)
	{
		int size = current()->w * current()->h * 3;

		for(int i = 0; i<size; i=i+3)
		{
			current()->buffer[i]	= current()->clear.r;
			current()->buffer[i+1]	= current()->clear.g;
			current()->buffer[i+2]	= current()->clear.b;
		}
	}
}

//LongName Function
void sglBegin(sglEElementType mode)
{

}

//LongName Function
void sglEnd(void)
{

}

//Vertex with 4 float coords
//[x,y,z,w]
//
// w	- ? is it weight or another dimension ?
void sglVertex4f(float x, float y, float z, float w)
{

}

//Vertex with 3 float coords
//[x,y,z]
void sglVertex3f(float x, float y, float z)
{

}

//Vertex with 2 float coords
//[x,y]
void sglVertex2f(float x, float y)
{

}

//2D Circle
//
// x	- center.x
// y	- center.y
// z	- ? IS IT 2D OR 3D CIRCLE ?
// r	- radius
//
// Using "Midpoint circle algorithm" @see http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void sglCircle(float x, float y, float z, float r)
{
	int p	= 1 - (int)r;
	int x0	= (int) x;
	int y0	= (int) y;
	int x1	= 0;
	int y1	= (int) r;
	int x2	= 1;
	int y2	= -2*r;

	//Set pixels base color here

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

		//Set pixels color here
	}
}

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
void sglEllipse(float x, float y, float z, float a, float b)
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

// ? What are the matrix modes ?
//
// ? we need something to hold on context ?
void sglMatrixMode( sglEMatrixMode mode )
{

}

//Push Matrix into transformation Stack
void sglPushMatrix(void)
{

}

//Pop Matrix from transformation Stack
void sglPopMatrix(void)
{

}

// ? Load identity matrix ?
void sglLoadIdentity(void)
{

}

// ? Load matrix where ? global ? stack ?
void sglLoadMatrix(const float *matrix)
{

}

//Multiply two matrices
//
// ? Formula for matrix multiplication here ?
void sglMultMatrix(const float *matrix)
{

}

//Translate coordinates
//
// ? fotmula for matrix by vector multiplication in terms of vertex translation here ?
void sglTranslate(float x, float y, float z)
{

}

// ? Scales what? Context or scene ?
void sglScale(float scalex, float scaley, float scalez)
{

}

//Rotate **** around the centerx,centery axis with given angle
//
// ? rotates what ? Context or scene ?
void sglRotate2D(float angle, float centerx, float centery)
{

}

// ? rotates what ? Context or scene ?
// ? around what Y axis? Base or context?
void sglRotateY(float angle)
{

}

// ?
void sglOrtho(float left, float right, float bottom, float top, float near, float far)
{

}

// ?
void sglFrustum(float left, float right, float bottom, float top, float near, float far)
{

}

//Sets scene viewport
void sglViewport(int x, int y, int width, int height)
{

}

//---------------------------------------------------------------------------
// Attribute functions
//---------------------------------------------------------------------------

//RGB Color
void sglColor3f(float r, float g, float b)
{
    current()->color = Color(r, g, b);
}

// ?
void sglAreaMode(sglEAreaMode mode)
{

}

//Point "radius/diameter ?"
void sglPointSize(float size)
{
	//Point size in context or static ?
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
