package tests;

import static rabbit.gl.engine.HUB.sglArc;
import static rabbit.gl.engine.HUB.sglAreaMode;
import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglCircle;
import static rabbit.gl.engine.HUB.sglClear;
import static rabbit.gl.engine.HUB.sglColor3f;
import static rabbit.gl.engine.HUB.sglVertex3f;
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
import static rabbit.gl.engine.HUB.sglRotateX;
import static rabbit.gl.engine.HUB.sglRotateY;
import static rabbit.gl.engine.HUB.sglRotateZ;
import rabbit.gl.engine.HUB;
import rabbit.gl.math.SimpleMath;

import static rabbit.gl.type.sglEAreaMode.SGL_LINE;
import static rabbit.gl.type.sglEAreaMode.SGL_POINT;
import static rabbit.gl.type.sglEEnableFlags.SGL_DEPTH_TEST;

import static rabbit.gl.type.sglEAreaMode.SGL_FILL;
import static rabbit.gl.type.sglEClearBit.SGL_DEPTH_BUFFER_BIT;
import static rabbit.gl.type.sglEElementType.SGL_LINES;
import static rabbit.gl.type.sglEElementType.SGL_POLYGON;
import static rabbit.gl.type.sglEElementType.SGL_LINE_LOOP;
import static rabbit.gl.type.sglEMatrixMode.SGL_MODELVIEW;
import static rabbit.gl.type.sglEMatrixMode.SGL_PROJECTION;

public class TestAnimation
{

	static float counter = 0.0f;
	static float color = 1.0f;
	public static void DrawAnimation(int WIDTH, int HEIGHT)
	{

		  counter+=.05f;
		  
		  float tx=0, ty=-1, tz=0;
			
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

	}
	
	/// render cube at (x,y,z) rotate by 'rot' radians
		private static void placeCube(float x,float y,float z)
		{
		  sglPushMatrix();
		  sglTranslate(x,y,z);
		  sglRotateX(counter);
		  sglRotateY(counter);
		  //sglRotateZ(counter);
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

	  sglColor3f(.2f,.2f,.2f);
	  poly4(indices[0],vertices);
	  sglColor3f(.3f,.3f,.3f);
	  poly4(indices[1],vertices);
	  sglColor3f(.5f,.5f,.5f);
	  poly4(indices[2],vertices);
	  sglColor3f(.7f,.7f,.7f);
	  poly4(indices[3],vertices);
	  sglColor3f(.8f,.8f,.8f);
	  poly4(indices[4],vertices);
	  sglColor3f(1,1,1);
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
