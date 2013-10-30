package rabbit.gl.engine;

import java.awt.image.BufferedImage;

import static rabbit.gl.type.sglEErrorCode.SGL_INVALID_ENUM;
import static rabbit.gl.type.sglEErrorCode.SGL_INVALID_OPERATION;
import static rabbit.gl.type.sglEErrorCode.SGL_INVALID_VALUE;
import static rabbit.gl.type.sglEErrorCode.SGL_NO_ERROR;
import static rabbit.gl.type.sglEErrorCode.SGL_STACK_UNDERFLOW;

import rabbit.gl.math.SimpleMath;
import rabbit.gl.primitive.Sphere;
import rabbit.gl.struct.Camera;
import rabbit.gl.struct.CameraManager;
import rabbit.gl.struct.Color;
import rabbit.gl.struct.ContextManager;
import rabbit.gl.struct.Matrix;
import rabbit.gl.struct.MatrixStack;
import rabbit.gl.struct.Vertex;
import rabbit.gl.struct.VertexStack;
import rabbit.gl.type.sglEAreaMode;
import rabbit.gl.type.sglEElementType;
import rabbit.gl.type.sglEEnableFlags;
import rabbit.gl.type.sglEErrorCode;
import rabbit.gl.type.sglEMatrixMode;
import rabbit.gl.type.sglEClearBit;

public class HUB
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
		ContextManager.delete();//.contexts.clear();
	}

	//LongName Function
	public static int sglCreateContext(int width, int height)
	{
		
		return ContextManager.create(width,height);
	}
	
	public static int sglCreateCamera(Camera camera)
	{
		int number_of_cameras = CameraManager.cameras.size();
				
		CameraManager.cameras.push(camera);

		return number_of_cameras;
	}

	//LongName Function
	// ? destroy or flush or release ?
	public static void sglDestroyContext(int id)
	{
		 if(ContextManager.contexts.isEmpty() || in_range(id,0,ContextManager.contexts.size()-1))
		 {
			 setErrCode(SGL_INVALID_VALUE);
			 return;
		 }

		 ContextManager.deleteContext(id);
	}

	public static void sglDestroyCamera(int id)
	{
		 if(CameraManager.cameras.isEmpty() || in_range(id,0,CameraManager.cameras.size()-1))
		 {
			 setErrCode(SGL_INVALID_VALUE);
			 return;
		 }

		 CameraManager.deleteCamera(id);
	}
	//LongName Function
	static void sglSetContext(int id)
	{
		 if(!in_range(id,0,ContextManager.contexts.size()-1))
		 {
			 setErrCode(SGL_INVALID_VALUE);
			 return;
		 }
		 ContextManager.setContext(id);
	}

	static void sglSetCamera(int id)
	{
		 if(!in_range(id,0,CameraManager.cameras.size()-1))
		 {
			 setErrCode(SGL_INVALID_VALUE);
			 return;
		 }
		 CameraManager.setCamera(id);
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

	static int sglGetCamera()
	{
		if(CameraManager.current < 0)
		{
			setErrCode(SGL_INVALID_OPERATION);
			return -1;
		}

		return CameraManager.current;
	}
	
	//LongName Function
	public static BufferedImage sglGetColorBufferPointer()
	{
		return Chunk.buffer;
		//return Context.buffer;
	}

	//---------------------------------------------------------------------------
	// Drawing functions
	//---------------------------------------------------------------------------

	//Clears buffer with given RGBA color value
	public static void sglClearColor (float r, float g, float b, float a)
	{
		//current()->cacheClear(r,g,b,a);
		
		int rgb = new Color(r,g,b).getRGB();
		

		for(int i=0; i<Chunk.clearbuff.length; i++)
		{
			Chunk.clearbuff[i]=rgb;
		}
			
		//System.arraycopy(current, 0, arg2, arg3, arg4)
		//current().clear.r = r;
	    //current().clear.g = g;
	    //current().clear.b = b;
	}

	//LongName Function
	public static void sglClear(sglEClearBit what)
	{
		switch(what)
		{
			case SGL_COLOR_BUFFER_BIT :
				current().clearColorBuffer();
			break;
			
			case SGL_DEPTH_BUFFER_BIT :
				current().clearDepthBuffer();
			break;
			
			default :
			setErrCode(SGL_INVALID_VALUE);
			return;
		}

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
			case SGL_LINE_BEZIER		:
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
	{
		current().rasterize();
	}

	//Vertex with 3 float coords in homogenous coordinates
	//[x,y,z,w]
	//
	static void sglVertex4f(float x, float y, float z, float w)
	{
		//normalize(x, y);
		//VERTICES.push_back(Vertex (x, y, z, w));
		try{ current().setVertex4f(x,y,z,w); }
		catch(Throwable t){/*ignore*/}
	}

	//Vertex with 3 float coords
	//[x,y,z]
	public static void sglVertex3f(float x, float y, float z)
	{
		//System.out.println("adding "+x+","+y+","+z);
		try{ current().setVertex3f(x,y,z); }
		catch(Throwable t){ t.printStackTrace(); }
	}


	//Vertex with 2 float coords
	//[x,y]
	public static void sglVertex2f(float x, float y)
	{
		try{ current().setVertex2f(x,y); }
		catch(Throwable t){/*ignore*/}
	}

	//2D Circle
	//
	// x	- center.x
	// y	- center.y
	// z	- ? IS IT 2D OR 3D CIRCLE ?
	// r	- radius
	//
	// Using "Midpoint circle algorithm" @see http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	public static void sglCircle(float x, float y, float z, float r)
	{
		if (r < 0)
		{
			setErrCode(SGL_INVALID_VALUE);
			return;
		}

		current().drawCricle(x,y,z,r);
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
	public static void sglEllipse(float x, float y, float z, float a, float b)
	{
		if (a < 0 || b < 0)
		{
			setErrCode(SGL_INVALID_VALUE);
			return;
		}
		
		current().drawEllipse(x, y, z, a, b);
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
	public static void sglArc(float x, float y, float z, float r, float from, float to)
	{
		if (r < 0)
		{
			setErrCode(SGL_INVALID_VALUE);
			return;
		}
		
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
	public static void sglMatrixMode( sglEMatrixMode mode )
	{
		switch(mode)
		{
			case SGL_MODELVIEW	:
			case SGL_PROJECTION	:
				current().setMatrixMode(mode);
			break;

			default				: setErrCode(SGL_INVALID_ENUM); break;
		}
	}

	//Push Matrix into transformation Stack
	public static void sglPushMatrix()
	{
		//if(current().invalidTypeStack())
		//{
			//setErrCode(SGL_INVALID_OPERATION);
			//return;
		//}
		//System.out.println("matrix pushed");
		current().pushMatrix();
	}

	//Pop Matrix from transformation Stack
	public static void sglPopMatrix()
	{
		if (current().stackEmpty())
		{
			setErrCode(SGL_STACK_UNDERFLOW);
			return;
		}
		//System.out.println("matrix poped");
		//if (current().stackEmpty())
		//{
			//setErrCode(SGL_STACK_UNDERFLOW);
			//return;
		//}
		
		current().popMatrix();
	}

	public static void sglLoadIdentity()
	{ current().setCurrentMatrix(Matrix.identity()); }

	static void sglLoadMatrix(float[] matrix)
	{ current().setCurrentMatrix(MatrixStack.create(matrix)); }

	//Multiply two matrices
	static void sglMultMatrix(float[] matrix)
	{ current().multiplyCurrentMatrix(MatrixStack.create(matrix)); }

	//Translate coordinates
	public static void sglTranslate(float x, float y, float z)
	{ current().multiplyCurrentMatrix(Matrix.translate(x,y,z)); }

	//Scale
	public static void sglScale(float scalex, float scaley, float scalez)
	{ current().multiplyCurrentMatrix(Matrix.scale(scalex, scaley, scalez)); }

	//Rotate **** around the centerx,centery axis with given angle
	public static void sglRotate2D(float angle, float centerx, float centery)
	{
		sglTranslate(centerx, centery, 0.0f);
		current().multiplyCurrentMatrix(Matrix.rotate2D(angle, centerx, centery));
		sglTranslate(-centerx, -centery, 0.0f);
	}

	public static void sglRotateY(float angle)
	{ current().multiplyCurrentMatrix(Matrix.rotateY(angle)); }
	
	public static void sglRotateX(float angle)
	{ current().multiplyCurrentMatrix(Matrix.rotateX(angle)); }

	public static void sglRotateZ(float angle)
	{ current().multiplyCurrentMatrix(Matrix.rotateZ(angle)); }
	
	public static void sglOrtho(float left, float right, float bottom, float top, float near, float far)
	{ current().multiplyCurrentMatrix(Matrix.ortho(left, right, bottom, top, near, far)); }

	public static void sglFrustum(float left, float right, float bottom, float top, float near, float far)
	{ current().multiplyCurrentMatrix(Matrix.frustum(left, right, bottom, top, near, far)); }
	
	public static void sglPerspective( float fovy, float aspect, float zNear, float zFar )
	{
		fovy *= 0.017453292519943295;
		float h2 = (float)Math.tan(fovy/2)*zNear;
		float w2 = h2*aspect;
		sglFrustum(-w2,w2,-h2,h2,zNear,zFar);
	}

	/// like gluLookAt
	public static void sglLookAt(float eyex   , float eyey   , float eyez,
	                float centerx, float centery, float centerz,
	                float upx    , float upy    , float upz)
	{
	  float    sqrmag;

	  /* Make rotation matrix */

	  /* Z vector */
	  Vertex z = new Vertex(eyex-centerx,eyey-centery,eyez-centerz);
		
	  sqrmag = z.magnitude();
	  z.div(SimpleMath.sqrt(sqrmag));

	  /* Y vector */
	  Vertex y = new Vertex(upx,upy,upz);

	  /* X vector = Y cross Z */
	  Vertex x = Vertex.crossProduct(y,z);

	  sqrmag = x.magnitude();
	  x.div(SimpleMath.sqrt(sqrmag));

	  /* Recompute Y = Z cross X */
	  y = Vertex.crossProduct(z, x);

	  sqrmag = y.magnitude();
	  y.div(SimpleMath.sqrt(sqrmag));

	  float m[] = {
	    x.x, y.x, z.x, 0, // col 1
	    x.y, y.y, z.y, 0, // col 2
	    x.z, y.z, z.z, 0, // col 3
	    - eyex*x.x - eyey*x.y - eyez*x.z , // col 4
	    - eyex*y.x - eyey*y.y - eyez*y.z , // col 4
	    - eyex*z.x - eyey*z.y - eyez*z.z , // col 4
	    1.0f};                              // col 4

		sglMultMatrix(m);
	}
	public static void sglViewport(int x, int y, int width, int height)
	{ current().setViewport(x, y, width, height); }

	//---------------------------------------------------------------------------
	// Attribute functions
	//---------------------------------------------------------------------------

	//RGB Color
	public static void sglColor3f( float r, float g, float b )
	{
		current().storage.color = new Color(r, g, b);
	}
	
	public static void sglColor3f( float r, float g, float b, float a )
	{ current().storage.color = new Color(r, g, b, a); }

	public static void sglAreaMode(sglEAreaMode mode)
	{
		switch(mode)
		{
			case SGL_POINT	: //Draw only vertices (or center for sglCircle, sglEllipse, sglArc)
			case SGL_LINE	: //Draw only borders of graphics elements (lines)
			case SGL_FILL	: //Draw filled elements, default.
				current().drawType = mode;
			break;
			default:
				setErrCode(SGL_INVALID_ENUM);
			return;
		}
	}

	//Point "radius/diameter ?"
	public static void sglPointSize(float size)
	{
	    if(size <= 0.0f)
	    {
	    	setErrCode(SGL_INVALID_VALUE);
	    	return;
	    }

	    current().storage.size = (int)size;
	}

	//Enable given flag
	public static void sglEnable(sglEEnableFlags flag)
	{
		switch( flag )
		{
			case SGL_DEPTH_TEST : current().enableDepthTest(); break;//depth test is off by default
			default				: setErrCode(SGL_INVALID_ENUM); return;
		}
	}

	//Disable given flag
	public static void sglDisable(sglEEnableFlags flag)
	{
		switch( flag )
		{
			case SGL_DEPTH_TEST : current().disableDepthTest(); break;//depth test is off by default
			default				: setErrCode(SGL_INVALID_ENUM); return;
		}
	}

	//---------------------------------------------------------------------------
	// RayTracing oriented functions
	//---------------------------------------------------------------------------

	// ? Begin of what scene ? whole scene or sub scene ?
	static void sglBeginScene()
	{

	}

	// ? End of what scene ? whole scene or sub scene ?
	public static void sglEndScene()
	{

	}

	public static void sglSphere(float x, float y, float z, float r)
	{ sglSphere(new Sphere(VertexStack.create(x, y, z, 1.0f), r)); }
	
	public static void sglSphere(Sphere s)
	{
		//TODO work with primitive
	}

	//Material ******
	public static void sglMaterial(float r, float g, float b, float kd, float ks, float shine, float T, float ior)
	{
		  sglColor3f( r*kd , g*kd , b*kd );
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

	static Context current()
	{ return ContextManager.working; }

}
