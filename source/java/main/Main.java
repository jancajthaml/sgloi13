package main;

import static type.sglEErrorCode.SGL_NO_ERROR;
import static type.sglEErrorCode.SGL_INVALID_VALUE;
import static type.sglEErrorCode.SGL_INVALID_OPERATION;
import static type.sglEErrorCode.SGL_INVALID_ENUM;
import static type.sglEErrorCode.SGL_STACK_UNDERFLOW;

import java.awt.image.BufferedImage;

import static type.sglEAreaMode.SGL_FILL;
import static type.sglEAreaMode.SGL_POINT;
import static type.sglEAreaMode.SGL_LINE;

import struct.Color;
import struct.Context;
import struct.ContextManager;
import struct.Matrix;
import struct.Vertex;
import type.sglEAreaMode;
import type.sglEElementType;
import type.sglEEnableFlags;
import type.sglEErrorCode;
import type.sglEMatrixMode;

public class Main
{

	static sglEErrorCode _libStatus = SGL_NO_ERROR;

	// Global error code dispatch
	static void setErrCode(sglEErrorCode c)
	{
		if(_libStatus==SGL_NO_ERROR)
			_libStatus = c;
	}

	//---------------------------------------------------------------------------
	// sglGetError()
	//---------------------------------------------------------------------------
	static sglEErrorCode sglGetError()
	{
		sglEErrorCode ret	= _libStatus;
		_libStatus			= SGL_NO_ERROR;
		return ret;
	}

	//---------------------------------------------------------------------------
	// sglGetErrorString()
	//---------------------------------------------------------------------------
	
	static String errStrigTable[] =
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
	
	static String sglGetErrorString(sglEErrorCode error)
	{

		return "DDD";
		//return (!in_range(error,SGL_NO_ERROR,SGL_OUT_OF_MEMORY))?"Invalid value passed to sglGetErrorString()":errStrigTable[int(error)];
	}

	//---------------------------------------------------------------------------
	// Data attributes
	//---------------------------------------------------------------------------

	//static ContextManager	manager;


	//---------------------------------------------------------------------------
	// Initialization functions
	//---------------------------------------------------------------------------

	//LongName Function "global init ? run init ?"
	static void sglInit()
	{
		//isDrawing = false;
	}

	//LongName Function "gloabl finalization ? run finalization ?"
	static void sglFinish()
	{
		ContextManager.contexts.clear();
	}

	//LongName Function
	static int sglCreateContext(int width, int height)
	{
		int number_of_contexts = ContextManager.contexts.size();

		//Context c = ;

	//	if(c == null)
		//{
			//setErrCode(SGL_OUT_OF_MEMORY);
			//return -1;
		//}

		ContextManager.contexts.push(new Context(width, height));//.push_back(c);

		return number_of_contexts;
	}

	//LongName Function
	// ? destroy or flush or release ?
	static void sglDestroyContext(int id)
	{
		 if(in_range(id,0,ContextManager.contexts.size()-1))
		 {
			 setErrCode(SGL_INVALID_VALUE);
			 return;
		 }

		 ContextManager.contexts.remove(id);
		 
		// delete manager.contexts[id]->buffer;
		 //delete manager.contexts[id];
	}

	//LongName Function
	static void sglSetContext(int id)
	{
		 if(!in_range(id,0,ContextManager.contexts.size()-1))
		 {
			 setErrCode(SGL_INVALID_VALUE);
			 return;
		 }

		 ContextManager.current = id;
	}

	//LongName Function
	static int sglGetContext()
	{
		if(ContextManager.current < 0)
		{
			setErrCode(SGL_INVALID_OPERATION);
			return -1;
		}

		return ContextManager.current;
	}

	//LongName Function
	static BufferedImage sglGetColorBufferPointer()
	{ return current().buffer; }

	//---------------------------------------------------------------------------
	// Drawing functions
	//---------------------------------------------------------------------------

	//Clears buffer with given RGBA color value
	static void sglClearColor (float r, float g, float b, float alpha)
	{
		current().clear.r = r;
	    current().clear.g = g;
	    current().clear.b = b;
	}

	//LongName Function
	static void sglClear(byte ... what)
	{
		if (current().invalidTypeStack())
		{
			setErrCode(SGL_INVALID_OPERATION);
			return;
		}

		/*
		if (((what & SGL_COLOR_BUFFER_BIT) != SGL_COLOR_BUFFER_BIT) && ((what & SGL_DEPTH_BUFFER_BIT) != SGL_DEPTH_BUFFER_BIT))
		{
			setErrCode(SGL_INVALID_VALUE);
			return;
		}

		if ((what & SGL_COLOR_BUFFER_BIT) == SGL_COLOR_BUFFER_BIT)
		{
		*/
			current().clearBuffer(what);
			/*
		}

		if ((what & SGL_DEPTH_BUFFER_BIT) == SGL_DEPTH_BUFFER_BIT)
		{
				//TODO clear with depth buffer bit
		}

*/
		setErrCode(SGL_NO_ERROR);
		

	}

	//LongName Function
	public static void sglBegin(sglEElementType mode)
	{
		switch(mode)
		{
			case SGL_POINTS				:
			case SGL_LINES				:
			case SGL_LINE_STRIP			:
			case SGL_LINE_LOOP			:
			case SGL_TRIANGLES			:
			case SGL_POLYGON			:
			case SGL_AREA_LIGHT			:
			case SGL_LAST_ELEMENT_TYPE	:
				
				current().pushTypeState(mode);
				
			break;
			
			default						:
			{
				setErrCode(SGL_INVALID_ENUM);
				return;	
			}
		}
	}

	//LongName Function
	public static void sglEnd()
	{ current().draw(); }

	//Vertex with 3 float coords in homogenous coordinates
	//[x,y,z,w]
	//
	static void sglVertex4f(float x, float y, float z, float w)
	{
		//normalize(x, y);
		//VERTICES.push_back(Vertex (x, y, z, w));
	}

	//Vertex with 3 float coords
	//[x,y,z]
	static void sglVertex3f(float x, float y, float z)
	{
		//normalize(x, y);
		//VERTICES.push_back(Vertex (x, y, z));
	}

	//Vertex with 2 float coords
	//[x,y]
	public static void sglVertex2f(float x, float y)
	{ current().setVertex2f(x,y); }

	//2D Circle
	//
	// x	- center.x
	// y	- center.y
	// z	- ? IS IT 2D OR 3D CIRCLE ?
	// r	- radius
	//
	// Using "Midpoint circle algorithm" @see http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	static void sglCircle(float x, float y, float z, float r)
	{ current().drawCricle(x,y,z,r); }

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
	static void sglEllipse(float x1, float y1, float z, float x2, float y2)
	{ current().drawEllipse(x1,y1,z,x2,y2); }

	//Line
	//Breceanuv algoritmus
	//DDA algoritmus (jednoduzsi)
	//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
	static void sglDrawLine(Vertex start, Vertex end)
	{ current().drawLine2D(start, end); }

	//Line
	static void sglDrawPolygon(float x1, float y1, float z, float x2, float y2)
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
	static void sglArc(float x, float y, float z, float r, float from, float to)
	{
		current().drawArc2D(x,y,z,r,from,to);
	}

	//---------------------------------------------------------------------------
	// Transform functions
	//---------------------------------------------------------------------------

	// Function name here
	//
	// modes:
//	        SGL_MODELVIEW  - desc
//	        SGL_PROJECTION - desc
	//
	// ? we need something to hold on context ?
	static void sglMatrixMode( sglEMatrixMode mode )
	{
		switch(mode)
		{
			case SGL_MODELVIEW	:
			case SGL_PROJECTION	:
			{
				current().setMatrixMode(mode);
			}
			break;

			default				: setErrCode(SGL_INVALID_ENUM); break;
		}
	}

	//Push Matrix into transformation Stack
	static void sglPushMatrix()
	{
		if(current().invalidTypeStack())
		{
			setErrCode(SGL_INVALID_OPERATION);
			return;
		}

		current().pushMatrix();
	}

	//Pop Matrix from transformation Stack
	static void sglPopMatrix()
	{
		if (current().stackEmpty())
		{
			setErrCode(SGL_STACK_UNDERFLOW);
			return;
		}

		current().popMatrix();
	}

	static void sglLoadIdentity()
	{ current().setCurrentMatrix(Matrix.identity()); }

	static void sglLoadMatrix(float[] matrix)
	{ current().setCurrentMatrix(new Matrix(matrix)); }

	//Multiply two matrices
	static void sglMultMatrix(float[] matrix)
	{ current().multiplyCurrentMatrix(new Matrix(matrix)); }

	//Translate coordinates
	static void sglTranslate(float x, float y, float z)
	{ current().multiplyCurrentMatrix(Matrix.translate(x,y,z)); }

	//Scale
	static void sglScale(float scalex, float scaley, float scalez)
	{ current().multiplyCurrentMatrix(Matrix.scale(scalex, scaley, scalez)); }

	//Rotate **** around the centerx,centery axis with given angle
	static void sglRotate2D(float angle, float centerx, float centery)
	{ current().multiplyCurrentMatrix(Matrix.rotate2D(angle, centerx, centery)); }

	// ? rotates what ? Context or scene ?
	// ? around what Y axis? Base or context?
	static void sglRotateY(float angle)
	{ current().multiplyCurrentMatrix(Matrix.rotateY(angle)); }

	// ?
	static void sglOrtho(float left, float right, float bottom, float top, float near, float far)
	{
		if(current().invalidTypeStack())
		{
			setErrCode(SGL_INVALID_OPERATION);
			return;
		}

		Matrix ortho = new Matrix(2/(right - left), 0, 0, 0, 0, 2/(top-bottom), 0, 0, 0, 0, -2/(far-near), 0, -((right+left)/(right-left)), -((top+bottom)/(top-bottom)), -((far+near)/(far-near)), 1);
		current().multiplyCurrentMatrix(ortho);
	}

	// ?
	static void sglFrustum(float left, float right, float bottom, float top, float near, float far)
	{
		if(current().invalidTypeStack())
		{
			setErrCode(SGL_INVALID_OPERATION);
			return;
		}

		Matrix frustum = new Matrix((2*near)/(right-left), 0, 0, 0, 0, (2*near)/(top-bottom), 0, 0, (right+left)/(right-left), (top+bottom)/(top-bottom), -(far+near)/(far-near), -1.0f, 0, 0, -(2.0f*far*near)/(far-near), 1);
		current().multiplyCurrentMatrix(frustum);
	}

	//Sets scene viewport
	static void sglViewport(int x, int y, int width, int height)
	{
		if(current().invalidTypeStack())
		{
			setErrCode(SGL_INVALID_OPERATION);
			return;
		}

		current().setViewport(width, height, x, y);
	}

	//---------------------------------------------------------------------------
	// Attribute functions
	//---------------------------------------------------------------------------

	//RGB Color
	static void sglColor3f(float r, float g, float b)
	{ current().color = new Color(r, g, b); }

	// ?
	static void sglAreaMode(sglEAreaMode mode)
	{
		if(mode!=SGL_FILL && mode!=SGL_LINE && mode!=SGL_POINT)
		{
			setErrCode(SGL_INVALID_ENUM);
			return;
		}
	}

	//Point "radius/diameter ?"
	static void sglPointSize(float size)
	{
	    if(size <= 0.0f)
	    {
	    	setErrCode(SGL_INVALID_VALUE);
	    	return;
	    }

	    current().size = (int)size;
	}

	//Enable given flag
	static void sglEnable(sglEEnableFlags cap)
	{

	}

	//Disable given flag
	static void sglDisable(sglEEnableFlags cap)
	{

	}

	//---------------------------------------------------------------------------
	// RayTracing oriented functions
	//---------------------------------------------------------------------------

	// ? Begin of what scene ? whole scene or sub scene ?
	static void sglBeginScene()
	{

	}

	// ? End of what scene ? whole scene or sub scene ?
	static void sglEndScene()
	{

	}

	//3D Sphere
	//
	// x	- center.x
	// y	- center.y
	// z	- center.z
	// r	- radius
	static void sglSphere(float x, float y, float z, float r)
	{

	}

	//Material ******
	static void sglMaterial(float r, float g, float b, float kd, float ks, float shine, float T, float ior)
	{

	}

	//Point Light
	//
	// ? x,y,z base coords r,g,b color and where is the direction ?
	static void sglPointLight(float x, float y, float z, float r, float g, float b)
	{

	}

	// ?
	static void sglRayTraceScene()
	{

	}

	// ? fragmentation -> rasterization ? direct rasterization ? evaluation ? bad name for a function !
	static void sglRasterizeScene()
	{

	}

	// ? image or fragment(s) ? or fragment set ?
	static void sglEnvironmentMap(int width, int height, float[] texels)
	{

	}

	// ?
	static void sglEmissiveMaterial(float r, float g, float b, float c0, float c1, float c2)
	{

	}

	//---------------------------------------------------------------------------
	// Helper functions
	//---------------------------------------------------------------------------

	//Check if number is in range
	//@see http://stackoverflow.com/questions/17095324/fastest-way-in-c-to-determine-if-an-integer-is-between-two-integers-inclusive
	static boolean in_range(int number, int low, int high)
	{ return ((int)(number-low) <= (high-low)); }

	//Fast double round (3 times faster than std::round because of absence of EDOM)
	//Type safe
	static double round(double x)
	{ return (double)((x>=0.5)?((int)(x)+1):(int)(x)); }

	static Context current()
	{ return ContextManager.contexts.get(ContextManager.current); }

}
