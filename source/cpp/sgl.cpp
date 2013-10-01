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

#include "sgl.h"
#include "data.h"
#include <vector>

//---------------------------------------------------------------------------
// Helper functions forward declaration
//---------------------------------------------------------------------------

bool in_range(unsigned number, unsigned low, unsigned high);
Context* current();
void normalize(float &x, float &y);
void setPixel(int x, int y);

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
std::vector<Vertex>		VERTICES;
std::vector<Edge>		EDGES;
bool					isDrawing;
sglEElementType			current_primitive_drawing;

//---------------------------------------------------------------------------
// Initialization functions
//---------------------------------------------------------------------------

//LongName Function "global init ? run init ?"
void sglInit(void)
{
	isDrawing = false;
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
	if(mode>SGL_LAST_ELEMENT_TYPE)
	{
		setErrCode(SGL_INVALID_ENUM);
		return;
	}

	//Musime vedet zde co jsme vykreslovali, nastaven enum zde

	isDrawing					= false;
	current_primitive_drawing	= mode;
	//? Context here ?

    VERTICES.clear();

}

//LongName Function
void sglEnd(void)
{
	if(isDrawing)
	{
		//throw exception did not begun
		return;
	}

	//Podle promenne nastavene v sglBegin vykreslime

	isDrawing = true;

	switch(current_primitive_drawing)
	{
		case SGL_POINTS :
		{
			int size = VERTICES.size();

			for( int i=0; i<size; i++ )
				setPixel(VERTICES[i].x,VERTICES[i].y);
		}
		break;

		case SGL_LINES :
		{
			int size = VERTICES.size();

			for( int i=0; i<size; i+=2 )
				sglDrawLine(VERTICES[i],VERTICES[i+1]);
		}
		break;

	}
}

//Vertex with 3 float coords in homogenous coordinates
//[x,y,z,w]
//
void sglVertex4f(float x, float y, float z, float w)
{
	normalize(x, y);
	VERTICES.push_back(Vertex (x, y, z, w));
}

//Vertex with 3 float coords
//[x,y,z]
void sglVertex3f(float x, float y, float z)
{
	normalize(x, y);
	VERTICES.push_back(Vertex (x, y, z));
}

//Vertex with 2 float coords
//[x,y]
void sglVertex2f(float x, float y)
{
	normalize(x, y);
	VERTICES.push_back(Vertex (x, y));
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
	//Prepsat na Bresehnamm

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

//Line
//Breceanuv algoritmus
//DDA algoritmus (jednoduzsi)
//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
void sglDrawLine(Vertex start, Vertex end)
{
	int x1		= int(start.x);
	int x2		= int(end.x);

    if (x1 > x2)
    {
    	sglDrawLine(end, start);
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
	//if(mode == SGL_MODELVIEW || mode == SGL_PROJECTION)
	//{
		switch(mode)
		{
			case SGL_MODELVIEW	:
			{

			}
			break;

			case SGL_PROJECTION	:
			{

			}
			break;

			default				: setErrCode(SGL_INVALID_ENUM); break;
		}
		//Set matrix mode
	//}
	//else
	//{
		//setErrCode(SGL_INVALID_ENUM);
	//}
}

//Push Matrix into transformation Stack
void sglPushMatrix(void)
{
	//Create stack of matrices
}

//Pop Matrix from transformation Stack
void sglPopMatrix(void)
{
	//Create stack of matrices
}

// ? Load identity matrix ?
void sglLoadIdentity(void)
{
	//?
}

// ? Load matrix where ? global ? stack ?
void sglLoadMatrix(const float *matrix)
{
	//?
}

//Multiply two matrices
//
// ? Formula for matrix multiplication here ?
//
//  | a b c d |     | 1   2  3  4 |   | a*1+b*5+c*9+d*13  a*2+b*6+c*10+d*14
//  | e f g h |  X  | 5   6  7  8 | = |
//  | i j k l |     | 9  10 11 12 |   |
//  | m n o p |     | 13 14 15 16 |   |
//
void sglMultMatrix(const float *matrix)
{



	//multiply two matices 4x4
}

//Translate coordinates
//
// ? fotmula for matrix by vector multiplication in terms of vertex translation here ?
void sglTranslate(float x, float y, float z)
{
	//aplikuj transformaci posunuti na aktualni matici

	//1 vytvorit novou matici popisujici posunuti
	//starou matici vynasobit zprava novou
	//nahradit starou matici novou matici
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

	//2D rotace -> rotace okolo osy z

}

// ? rotates what ? Context or scene ?
// ? around what Y axis? Base or context?
void sglRotateY(float angle)
{
	// rotace okolo osy y

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

//Normalize coords in context of Viewport
//@see http://www.opengl.org/sdk/docs/man/xhtml/glViewport.xml
void normalize(float &x, float &y)
{
    x = (x+1)*(current()->w2)+current()->x;
    y = (y+1)*(current()->h2)+current()->y;
}


void setPixel(int x, int y)
{
	Context* c	= current();
	c->x		= x;
	c->y		= y;

	int offset = (c->x+c->w*c->y)*3;

	c->buffer[offset]	= c->color.r;
	c->buffer[offset+1]	= c->color.g;
	c->buffer[offset+2]	= c->color.b;
}

Context* current()
{ return manager.contexts[manager.current]; }

