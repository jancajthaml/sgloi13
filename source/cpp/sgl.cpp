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
 * 14.10.2013 ------- FIRST MILESTONE
 * bugs:
 *       - conditioning skip
 *       - viewport wrong calculation
 *       - padding in typedef (Visual studio)
 *
 * ---------------------------------------------------------------------------------------------------------
 *
 * 15.10.2013, Jan Cajthaml - refactor of Viewport coords & matrix calculations
 *                          - conditioning of ERROR CODES by sgl.h specification
 *                          - "if" rewritten to "switch" as needed
 *                          - added current()–>begin() and current()->end() to check sglBegin and sglEnd of Context
 *
 * */

/*
 * useful links:
 * Bresenham's line algorithm - http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 * C++ Code optimalisation    - http://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Code_optimization/Faster_operations
 *
 */

#include "core/CrossReferenceDispatcher.h"
#include "core/Context.h"
#include "core/ContextManager.h"

//---------------------------------------------------------------------------
// Helper functions forward declaration
//---------------------------------------------------------------------------

bool in_range(unsigned number, unsigned low, unsigned high);
Context* current();
//void normalize(float &x, float &y);
//void setPixel(int x, int y);

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

	//manager.contexts.clear();
}

//LongName Function
int sglCreateContext(int width, int height)
{
	int_fast8_t size = manager.contexts.size();

	manager.contexts.push_back(Context(width, height));

	return size;
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

	 manager.deleteContext(id);
}

//LongName Function
void sglSetContext(int id)
{
	 if(!in_range(id,0,manager.contexts.size()-1))
	 {
		 setErrCode(SGL_INVALID_VALUE);
		 return;
	 }

	 manager.setContext(id);
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
float* sglGetColorBufferPointer(void)
{
	return (float*)current()->storage.buffer;
    //return (float *) current()->buffer;
}

//---------------------------------------------------------------------------
// Drawing functions
//---------------------------------------------------------------------------

//Clears buffer with given RGBA color value
void sglClearColor (float r, float g, float b, float a)
{ current()->cacheClear(r,g,b,a); }

//LongName Function
void sglClear(unsigned what)
{
	if (current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	switch(what)
	{
		case SGL_COLOR_BUFFER_BIT :  current()->clearColorBuffer();  break;
		case SGL_DEPTH_BUFFER_BIT :  current()->clearDepthBuffer();  break;
		default                   :  setErrCode(SGL_INVALID_VALUE);  return;
	}

	setErrCode(SGL_NO_ERROR);
}

//LongName Function
void sglBegin(sglEElementType mode)
{
    switch(mode)
    {
    	case SGL_POINTS		: 			/// Points
    	case SGL_LINES		: 			/// Lines
    	case SGL_LINE_STRIP	: 			/// Line strip
    	case SGL_LINE_LOOP	: 			/// Closed line strip
    	case SGL_TRIANGLES	: 			/// Triangle list
    	case SGL_POLYGON	: 			/// General, non-convex polygon
    	case SGL_AREA_LIGHT	: break;	/// Area light - restricted to a quad for simplicity
    	default				: setErrCode(SGL_INVALID_ENUM); break;
    }

    if(current()->BeginBeforeEnd())
    {
    	setErrCode(SGL_INVALID_OPERATION);
    	return;
    }

	current()->begin();
	current()->pushTypeState(mode);
}

//LongName Function
void sglEnd(void)
{
	 if(!current()->BeginBeforeEnd())
	 {
		 setErrCode(SGL_INVALID_OPERATION);
		 return;
	 }

	 current()->end();
	 current()->rasterize();
}

//Vertex with 3 float coords in homogenous coordinates
//[x,y,z,w]
//
void sglVertex4f(float x, float y, float z, float w)
{
	current()->setVertex4f(x, y, z, w);
	//printf("before transform -> vertex4: x=%f, y=%f, z=%f, w=%f\n",x,y,z,w);
}

//Vertex with 3 float coords
//[x,y,z]
void sglVertex3f(float x, float y, float z)
{
	current()->setVertex3f(x, y, z);
	//printf("before transform -> vertex3: x=%f, y=%f, z=%f\n",x,y,z);
}

//Vertex with 2 float coords
//[x,y]
void sglVertex2f(float x, float y)
{
	current()->setVertex2f(x, y);
	//printf("before transform -> vertex2: x=%f, y=%f\n",x,y);
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
	if (r < 0)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}

	//if (current()->BeginBeforeEnd())
	//{
		//setErrCode(SGL_INVALID_OPERATION);
		//return;
	//}

	current()->drawCricle(x, y, z, r);
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
//
//@see http://www.codeproject.com/Messages/2112010/A-Fast-Bresenham-Type-Algorithm-For-Drawing-Ellips.aspx
void sglEllipse(float x, float y, float z, float a, float b)
{
	if (a < 0 || b < 0)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}

	//if (current()->BeginBeforeEnd())
	//{
		//setErrCode(SGL_INVALID_OPERATION);
		//return;
	//}

	current()->drawEllipse(x, y, z, a, b);
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
void sglArc(float x, float y, float z, float radius, float from, float to)
{
	if (radius < 0)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}

	current()->drawArc2D(x, y, z, radius, from, to);
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
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	current()->pushMatrix();
}

//Pop Matrix from transformation Stack
void sglPopMatrix(void)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
	if (current()->stackEmpty())
	{
		setErrCode(SGL_STACK_UNDERFLOW);
		return;
	}

	current()->popMatrix();
}

void sglLoadIdentity(void)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	current()->setCurrentMatrix(MatrixCache::identity());
}

void sglLoadMatrix(const float* matrix)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	current()->setCurrentMatrix(Matrix(matrix));
}

//Multiply two matrices
void sglMultMatrix(const float* matrix)
{
	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	Matrix mat = Matrix(matrix);
	current()->multiplyCurrentMatrix(mat);
}

//Translate coordinates
void sglTranslate(float x, float y, float z)
{
	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	Matrix translate = MatrixCache::translate(x,y,z);
	current()->multiplyCurrentMatrix(translate);
}

//Scale
void sglScale(float scalex, float scaley, float scalez)
{
	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	Matrix scale = MatrixCache::scale(scalex, scaley, scalez);
	current()->multiplyCurrentMatrix(scale);

}

//Rotate **** around the centerx,centery axis with given angle
void sglRotate2D(float angle, float centerx, float centery)
{
	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	sglTranslate(centerx, centery, 0.0f);
	Matrix rotate = MatrixCache::rotate2D(angle, centerx, centery);
	current()->multiplyCurrentMatrix(rotate);
	sglTranslate(-centerx, -centery, 0.0f);
}

// ? rotates what ? Context or scene ?
// ? around what Y axis? Base or context?
void sglRotateY(float angle)
{
	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	Matrix rotate = MatrixCache::rotateY(angle);
	current()->multiplyCurrentMatrix(rotate);
}

// ?
void sglOrtho(float left, float right, float bottom, float top, float near, float far)
{
	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	Matrix ortho = MatrixCache::ortho(left, right, bottom, top, near, far);
	current()->multiplyCurrentMatrix(ortho);
}

// ?
void sglFrustum(float left, float right, float bottom, float top, float near, float far)
{
	if(far < 0 || near < 0) return;

	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	Matrix frustum = MatrixCache::frustum(left, right, bottom, top, near, far);
	current()->multiplyCurrentMatrix(frustum);
}

//Sets scene viewport
void sglViewport(int x, int y, int width, int height)
{

	if(width<0 || height<0)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}
	if(current()->BeginBeforeEnd())
	{
	    setErrCode(SGL_INVALID_OPERATION);
	    return;
	}

	current()->setViewport(x, y, width, height);
}

//---------------------------------------------------------------------------
// Attribute functions
//---------------------------------------------------------------------------

//RGB Color
void sglColor3f(float r, float g, float b)
{
	current()->storage.color = Color(r, g, b);

	//current()->color = Color(r, g, b);
}

// ?
void sglAreaMode(sglEAreaMode mode)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}

	switch(mode)
	{
		case SGL_POINT	: //Draw only vertices (or center for sglCircle, sglEllipse, sglArc)
		case SGL_LINE	: //Draw only borders of graphics elements (lines)
		case SGL_FILL	: //Draw filled elements, default.
			current()->drawType = mode;
		break;
		default:
			setErrCode(SGL_INVALID_ENUM);
		return;
	}
}

//Point "radius/diameter ?"
void sglPointSize(float size)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
    if(size <= 0.0f)
    {
    	setErrCode(SGL_INVALID_VALUE);
    	return;
    }

    current()->storage.size = size;
}

//Enable given flag
void sglEnable(sglEEnableFlags flag)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
	switch(flag)
	{
		case SGL_DEPTH_TEST : current()->enableDepthTest(); break;//depth test is off by default
		default: setErrCode(SGL_INVALID_ENUM); return;
	}
}

//Disable given flag
void sglDisable(sglEEnableFlags flag)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
	switch(flag)
	{
		case SGL_DEPTH_TEST : current()->disableDepthTest(); break;//depth test is off by default

		setErrCode(SGL_INVALID_ENUM);
		return;
	}
}

//---------------------------------------------------------------------------
// RayTracing oriented functions
//---------------------------------------------------------------------------

// ? Begin of what scene ? whole scene or sub scene ?
void sglBeginScene()
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
}

// ? End of what scene ? whole scene or sub scene ?
void sglEndScene()
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
}

//3D Sphere
//
// x	- center.x
// y	- center.y
// z	- center.z
// r	- radius
void sglSphere(const float x, const float y, const float z, const float r)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
}

//Material ******
void sglMaterial(const float r, const float g, const float b, const float kd, const float ks, const float shine, const float T, const float ior)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
}

//Point Light
//
// ? x,y,z base coords r,g,b color and where is the direction ?
void sglPointLight(const float x, const float y, const float z, const float r, const float g, const float b)
{
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
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
	if(current()->BeginBeforeEnd())
	{
		setErrCode(SGL_INVALID_OPERATION);
		return;
	}
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


Context* current()
{ return &manager.contexts[manager.current]; }

