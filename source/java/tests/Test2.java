package tests;

import static rabbit.gl.engine.HUB.sglArc;
import static rabbit.gl.engine.HUB.sglAreaMode;
import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglCircle;
import static rabbit.gl.engine.HUB.sglColor3f;
import static rabbit.gl.engine.HUB.sglVertex3f;
import static rabbit.gl.engine.HUB.sglClear;
import static rabbit.gl.engine.HUB.sglEllipse;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglLoadIdentity;
import static rabbit.gl.engine.HUB.sglEnable;
import static rabbit.gl.engine.HUB.sglDisable;
import static rabbit.gl.engine.HUB.sglFrustum;
import static rabbit.gl.engine.HUB.sglViewport;
import static rabbit.gl.engine.HUB.sglMatrixMode;
import static rabbit.gl.engine.HUB.sglOrtho;
import static rabbit.gl.engine.HUB.sglRotate2D;
import static rabbit.gl.engine.HUB.sglTranslate;
import static rabbit.gl.engine.HUB.sglVertex2f;
import static rabbit.gl.engine.HUB.sglPushMatrix;
import static rabbit.gl.engine.HUB.sglPopMatrix;
import static rabbit.gl.engine.HUB.sglRotateY;

import static rabbit.gl.type.sglEAreaMode.SGL_LINE;
import static rabbit.gl.type.sglEAreaMode.SGL_POINT;
import static rabbit.gl.type.sglEEnableFlags.SGL_DEPTH_TEST;

import static rabbit.gl.type.sglEAreaMode.SGL_FILL;
import static rabbit.gl.type.sglEElementType.SGL_LINES;
import static rabbit.gl.type.sglEElementType.SGL_POLYGON;
import static rabbit.gl.type.sglEElementType.SGL_LINE_LOOP;
import static rabbit.gl.type.sglEMatrixMode.SGL_MODELVIEW;
import static rabbit.gl.type.sglEMatrixMode.SGL_PROJECTION;

import static rabbit.gl.type.sglEClearBit.SGL_DEPTH_BUFFER_BIT;

public class Test2
{

	public static void DrawTestScene2A(int WIDTH, int HEIGHT)
	{
		sglDisable(SGL_DEPTH_TEST);

		  // set viewport
		  sglViewport(0, 0, WIDTH, HEIGHT);

		  // set the projection matrix
		  sglMatrixMode(SGL_PROJECTION);
		  sglLoadIdentity();
		  sglOrtho(-8, 8, -6, 6, -1, 1);

		  // set the modelview matrix
		  sglMatrixMode(SGL_MODELVIEW);
		  sglLoadIdentity();

		  sglColor3f(1,0,0);
		  sglAreaMode(SGL_FILL);
		  butterfly();
		  sglColor3f(1,1,0);
		  sglAreaMode(SGL_LINE);
		  butterfly();

		  sglColor3f(0,1,1);
		  sglAreaMode(SGL_FILL);

		  sglTranslate(4,0,0);
		  sglRotate2D(1,0,0);
		  butterfly();
		  sglColor3f(0,1,0);
		  sglAreaMode(SGL_LINE);
		  butterfly();

		  sglLoadIdentity();
		  sglTranslate(-4,-1,0);
		  sglRotate2D(-0.5f,0,0);
		  sglAreaMode(SGL_FILL);
		  butterfly();
		  sglColor3f(1,1,0);
		  sglAreaMode(SGL_LINE);
		  butterfly();
	}
	

	/// render a non-convex polygon
	private static void butterfly()
	{
	  sglBegin(SGL_POLYGON);
	  sglVertex2f(-2,-2);
	  sglVertex2f( 0, 1);
	  sglVertex2f( 2,-2);
	  sglVertex2f( 2, 2);
	  sglVertex2f( 0, -1);
	  sglVertex2f(-2, 2);
	  sglEnd();
	}
	
	public static void DrawTestScene2B(int WIDTH, int HEIGHT) 
	{
		sglDisable(SGL_DEPTH_TEST);

		  // set viewport
		  sglViewport(0, 0, WIDTH, HEIGHT);

		  // set the projection matrix
		  sglMatrixMode(SGL_PROJECTION);
		  sglLoadIdentity();
		  sglOrtho(0, 8, -3, 3, -1, 1);

		  // set the modelview matrix
		  sglMatrixMode(SGL_MODELVIEW);
		  sglLoadIdentity();

		  sglTranslate(1.5f,0,0);

		  float r;
		  // circles
		  for(r=1.2f; r>0.1f; r-=0.1f)
		  {
		    sglColor3f(0,r,r);
		    sglAreaMode(SGL_FILL);
		    sglCircle(0, 0, 0, r);
		    sglColor3f(r,r,0);
		    sglAreaMode(SGL_LINE);
		    sglCircle(0, 0, 0, r);
		    
		    sglColor3f(r,r,r);
		    sglAreaMode(SGL_POINT);
		    sglCircle(0, 0, 0, r);
		  }

		  // ellipses
		  sglTranslate(2.5f,0,0);

		  sglPushMatrix();
		  sglTranslate(0,1.5f,0);
		  sglAreaMode(SGL_FILL);
		  ellipses(1.4f, 1,1,0);
		  sglAreaMode(SGL_LINE);
		  ellipses(1.4f, 0,1,1);
		  sglTranslate(0,-3,0);
		  sglRotate2D((float)(Math.PI*0.25),0,0);
		  sglAreaMode(SGL_FILL);
		  ellipses(1.4f, 1,1,0);
		  sglAreaMode(SGL_LINE);
		  ellipses(1.4f, 0,1,1);
		  sglPopMatrix();

		  // arcs
		  sglTranslate(2.5f,0,0);
		  sglAreaMode(SGL_FILL);
		  arcs(1.3f, 1,0,1);
		  sglAreaMode(SGL_LINE);
		  arcs(1.3f, 0,1,0);
	}

	public static void DrawTestScene2C(int WIDTH, int HEIGHT) 
	{
		float tx=0, ty=0, tz=0;
		
		sglClear(SGL_DEPTH_BUFFER_BIT);
		sglViewport(0, 0, WIDTH, HEIGHT);

		  // set the projection matrix
		  sglMatrixMode(SGL_PROJECTION);
		  sglLoadIdentity();
		  sgluPerspective(45,(float)WIDTH/HEIGHT, 0.1f, 10.0f);

		  // set the modelview matrix
		  sglMatrixMode(SGL_MODELVIEW);
		  sglLoadIdentity();

		  sglTranslate(tx,ty,tz-10);

		  sglAreaMode(SGL_FILL);

		  // upper row with depth test
		  sglEnable(SGL_DEPTH_TEST);
		  placeCube(-1,2,0);
		  placeCube(1,2,0);
		  placeCube(0,1.5f,0);

		  // lower row without depth test
		  sglDisable(SGL_DEPTH_TEST);
		  placeCube(-1,-2,0);
		  placeCube(1,-2,0);
		  placeCube(0,-1.5f,0);
	}
	
	static void box()
	{
	  sglBegin(SGL_LINE_LOOP);
	  sglVertex2f(-1,-1);
	  sglVertex2f(-1,1);
	  sglVertex2f(1,1);
	  sglVertex2f(1,-1);
	  sglEnd();
	  sglBegin(SGL_LINES);
	  sglVertex2f(0,-1);
	  sglVertex2f(0,1);
	  sglEnd();
	}



	/// render a bunch of colorful ellipses
	private static void ellipses(float r, float R, float G, float B)
	{
	  int numInitSegments = 10;
	  for(int i=0; i<numInitSegments; i++) {
	    float c = ((float)i)/numInitSegments;
	    sglColor3f(c*R,c*G,c*B);
	    sglEllipse(0,0,0,r*((float)(i+1))/numInitSegments,r*((float)(numInitSegments-i))/numInitSegments);
	  }
	}
	

	/// render a bunch of colorful arcs
	private static void arcs(float initr, float R, float G, float B)
	{
	  for(float r=initr ; r>0.1; r-=0.2) {
	    float c = r/initr;;
	    sglColor3f(c*R,c*G,c*B);
	    for(float seg=0; seg<Math.PI*2; seg+=Math.PI*2/8)
	    sglArc(0,0,0,r,seg,(float)(seg+Math.PI*2/16));
	  }
	}
	

	/// render cube at (x,y,z) rotate by 'rot' radians
	private static void placeCube(float x,float y,float z)
	{
		float rot=3.14f/3.0f;
	  sglPushMatrix();
	  sglTranslate(x,y,z);
	  sglRotateY(rot);
	  cube();
	  sglPopMatrix();
	}
	
	private static void cube()
	{
	  float[][] vertices =
	  { {0,0,0} ,
	  { 1, 1, 1},
	  { 1, 1,-1},
	  { 1,-1, 1},
	  { 1,-1,-1},
	  {-1, 1, 1},
	  {-1, 1,-1},
	  {-1,-1, 1},
	  {-1,-1,-1}};
	  int[][] indices =
	  {
	    {2,1,3,4},
	    {5,6,8,7},
	    {1,2,6,5},
	    {4,3,7,8},
	    {3,1,5,7},
	    {2,4,8,6}
	  };

	  sglColor3f(0,0,1);
	  poly4(indices[0],vertices);
	  sglColor3f(0,1,0);
	  poly4(indices[1],vertices);
	  sglColor3f(0,1,1);
	  poly4(indices[2],vertices);
	  sglColor3f(1,0,0);
	  poly4(indices[3],vertices);
	  sglColor3f(1,0,1);
	  poly4(indices[4],vertices);
	  sglColor3f(1,1,0);
	  poly4(indices[5],vertices);
	}
	

	/// render a 4-sided polygon
	private static void poly4(int[] idx, float[][] p)
	{
	  sglBegin(SGL_POLYGON);
	  for(int i=0; i<4; i++)
	    sglVertex3f(p[idx[i]][0],p[idx[i]][1],p[idx[i]][2]);
	  sglEnd();
	}
	

	/// like gluPerspective
	private static void sgluPerspective( float fovy, float aspect, float zNear, float zFar )
	{
	  fovy *= (3.1415926535/180);
	  float h2 = (float)Math.tan(fovy/2)*zNear;
	  float w2 = h2*aspect;
	  sglFrustum(-w2,w2,-h2,h2,zNear,zFar);
	}
}
