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
 *
 * */
#include "sgl.h"
#include <vector>
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

std::vector<Vertex>	VERTEX;
std::vector<Edge>	EDGE;

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

	return 0;
}

//LongName Function
void sglDestroyContext(int id)
{

}

//LongName Function
void sglSetContext(int id)
{

}

//LongName Function
int sglGetContext(void)
{

	return 0;
}

//LongName Function
float *sglGetColorBufferPointer(void)
{

	return 0;
}

//---------------------------------------------------------------------------
// Drawing functions
//---------------------------------------------------------------------------

//Clears buffer with given RGBA color value
void sglClearColor (float r, float g, float b, float alpha)
{

}

//LongName Function
void sglClear(unsigned what)
{

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
void sglCircle(float x, float y, float z, float r)
{

}

//2D Ellipse
//
// x	- center.x
// y	- center.y
// z	- ? IS IT 2D OR 3D ELLIPSE ?
// a	- ? width ?
// b	- ? height ?
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
void sglArc(float x, float y, float z, float r, float from, float to)
{

}

//---------------------------------------------------------------------------
// Transform functions
//---------------------------------------------------------------------------

// ? What are the matrix modes ?
void sglMatrixMode( sglEMatrixMode mode )
{

}

//Push Matrix into transformation Stack
//
//	|	|	|	|
//	|	|	+-M-+
//	+---+	+---+
void sglPushMatrix(void)
{

}

//Pop Matrix from transformation Stack
//
//	|	|	|	|
//	+-M-+	|   |
//	+---+	+---+
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

}

// ?
void sglAreaMode(sglEAreaMode mode)
{

}

//Point "radius/diameter ?"
void sglPointSize(float size)
{

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
