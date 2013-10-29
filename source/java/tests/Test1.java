package tests;

import static rabbit.gl.engine.HUB.sglArc;
import static rabbit.gl.engine.HUB.sglAreaMode;
import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglCircle;
import static rabbit.gl.engine.HUB.sglColor3f;
import static rabbit.gl.engine.HUB.sglEllipse;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglLoadIdentity;
import static rabbit.gl.engine.HUB.sglMatrixMode;
import static rabbit.gl.engine.HUB.sglOrtho;
import static rabbit.gl.engine.HUB.sglPointSize;
import static rabbit.gl.engine.HUB.sglRotate2D;
import static rabbit.gl.engine.HUB.sglScale;
import static rabbit.gl.engine.HUB.sglTranslate;
import static rabbit.gl.engine.HUB.sglVertex2f;
import static rabbit.gl.engine.HUB.sglPushMatrix;
import static rabbit.gl.engine.HUB.sglPopMatrix;
import static rabbit.gl.math.SimpleMath.PI;
import static rabbit.gl.math.SimpleMath.cos;
import static rabbit.gl.math.SimpleMath.sin;
import static rabbit.gl.type.sglEAreaMode.SGL_LINE;
import static rabbit.gl.type.sglEElementType.SGL_LINES;
import static rabbit.gl.type.sglEElementType.SGL_LINE_LOOP;
import static rabbit.gl.type.sglEElementType.SGL_LINE_STRIP;
import static rabbit.gl.type.sglEElementType.SGL_POINTS;
import static rabbit.gl.type.sglEMatrixMode.SGL_MODELVIEW;
import static rabbit.gl.type.sglEMatrixMode.SGL_PROJECTION;
import rabbit.gl.engine.HUB;
import rabbit.gl.type.sglEClearBit;

public class Test1
{

	public static void DrawTestScene1A(int WIDTH, int HEIGHT) 
	{
		HUB.sglClear(sglEClearBit.SGL_COLOR_BUFFER_BIT);
	//  sglDisable(SGL_DEPTH_TEST);

	  // set viewport

	  // set the projection matrix
	  sglMatrixMode(SGL_PROJECTION);
	  sglLoadIdentity();
	  sglOrtho(-10*WIDTH/HEIGHT, 10*WIDTH/HEIGHT, -10, 10, -1, 1);

	  // set the modelview matrix
	  sglMatrixMode(SGL_MODELVIEW);
	  sglLoadIdentity();

	  float offsetx, offsety = -6;
	  int numInitSegments = 8, i;
	  float segment = (2.0f*PI/(float)numInitSegments);

	  float r;
	  for(r=0.2f; r<=2.5; r+=0.2) {

	    offsetx = -7.25f;

	    // point test
	    sglColor3f(r/3,r/3,r/3);
	    sglPointSize(3);
	    sglBegin(SGL_POINTS);
	    {
	      for(i=0; i<numInitSegments; i++) {
	        float angle = segment*(float)i;
	        sglVertex2f(offsetx + r*cos(angle),offsety + r*sin(angle));
	      }
	    }
	    sglEnd();

	    offsetx += 5;

	    // line test
	    sglColor3f(1,0,0);
	    sglBegin(SGL_LINES);
	    for(i=0; i<numInitSegments; i++) {
	      float angle = segment*(float)i;
	      sglVertex2f(offsetx+r*cos(angle),offsety+r*sin(angle));
	    }
	    sglEnd();

	    offsetx += 5;

	    // line strip test
	    sglColor3f(0,1,0);
	    sglBegin(SGL_LINE_STRIP);
	    for(i=0; i<numInitSegments; i++) {
	      float angle = segment*(float)i;
	      sglVertex2f(offsetx+r*cos(angle),offsety+r*sin(angle));
	    }
	    sglEnd();

	    offsetx += 5;


	    // line loop test
	    sglColor3f(0,0,1);
	    sglBegin(SGL_LINE_LOOP);
	    for(i=0; i<numInitSegments; i++) {
	      float angle = segment*(float)i;
	      sglVertex2f(offsetx+r*cos(angle),offsety+r*sin(angle));
	    }
	    sglEnd();

	  } // for r...

	  offsety += 2.5*r;

	  sglAreaMode(SGL_LINE);

	  r=2;

	  // ellipse drawing
	  sglColor3f(1,0,1);
	  for(i=0; i<numInitSegments; i++) {
	    sglEllipse(0,0,0,(i+1)*2.5f*r/numInitSegments,(numInitSegments-i)*2.5f*r/numInitSegments);
	  }

	  sglColor3f(0,1,1);

	  sglEllipse(-7,-1,0,2.9f,0.2f);
	  sglEllipse(7,0,0,0.2f,2.9f);

	  offsetx = -7.25f;
	  offsety = 6;

	  sglColor3f(1,1,0);

	  for(float rr=r/5; rr<=r; rr+=r/5) {
	    sglCircle(offsetx-1,offsety,0,0.5f*rr);
	    sglCircle(offsetx+3,offsety,0,1.5f*rr);
	  }
	  
	  offsetx = r;

	  sglColor3f(1,1,1);

	  for(float rr=0.4f; rr<=4; rr+=0.4) {
	    sglArc(offsetx,offsety,  0,rr,0,PI/2);
	    sglArc(offsetx+4,offsety,0,rr,PI*2/2,PI*3/2);
	  }

	}

	public static void DrawTestScene1B() 
	{
		HUB.sglClear(sglEClearBit.SGL_COLOR_BUFFER_BIT);
	  //sglDisable(SGL_DEPTH_TEST);
	  sglAreaMode(SGL_LINE);

	  // set viewport
	  //sglViewport(0, 0, WIDTH, HEIGHT);

	  // set the projection matrix
	  sglMatrixMode(SGL_PROJECTION);
	  sglLoadIdentity();
	  sglOrtho(-8, 8, -6, 6, -1, 1);

	  // set the modelview matrix
	  sglMatrixMode(SGL_MODELVIEW);
	  sglLoadIdentity();

	  // draw coord axes
	  sglColor3f(0.5f,0.5f,0.5f);
	  sglBegin(SGL_LINES);
	  sglVertex2f(-7,0);
	  sglVertex2f(7,0);
	  sglVertex2f(0,-5);
	  sglVertex2f(0,5);
	  sglEnd();

	  // BOX
	  sglColor3f(1,1,1);
	  box();

	  sglColor3f(0,1,0);
	  sglTranslate(-3,0,0);
	  box();

	  sglColor3f(1,0,0);
	  sglTranslate(-3,0,0);
	  sglRotate2D(PI*1/8,0,0);
	  box();

	  sglColor3f(0,0,1);
	  sglLoadIdentity();
	  sglTranslate(3,0,0);
	  sglScale(0.5f,2,1f);
	  box();

	  sglColor3f(0,1,1);
	  sglLoadIdentity();
	  sglTranslate(6,0,0);
	  sglRotate2D(PI*1/8,0,0);
	  sglScale(0.5f,2,1);
	  box();

	  // ARC
	  sglLoadIdentity(); sglTranslate(0,4,0);

	  sglColor3f(1,1,1);
	  sglArc(0,0,0,1,0,4);

	  sglColor3f(0,1,0);
	  sglTranslate(-3,0,0);
	  sglArc(0,0,0,1,0,4);

	  sglColor3f(1,0,0);
	  sglTranslate(-3,0,0);
	  sglRotate2D(PI*1/8,0,0);
	  sglArc(0,0,0,1,0,4);

	  sglColor3f(0,0,1);
	  sglLoadIdentity(); sglTranslate(0,4,0);
	  sglTranslate(3,0,0);
	  sglScale(0.5f,2,1);
	  sglArc(0,0,0,1,0,4);

	  sglColor3f(0,1,1);
	  sglLoadIdentity(); sglTranslate(0,4,0);
	  sglTranslate(6,0,0);
	  sglRotate2D(PI*1/8,0,0);
	  sglScale(0.5f,2,1);
	  sglArc(0,0,0,1,0,4);

	  // ELLIPSE
	  sglLoadIdentity(); sglTranslate(0,-4,0);

	  sglColor3f(1,1,1);
	  sglEllipse(0,0,0,1.5f,0.7f);

	  sglColor3f(0,1,0);
	  sglTranslate(-3,0,0);
	  sglEllipse(0,0,0,1.5f,0.7f);

	  sglColor3f(1,0,0);
	  sglTranslate(-3,0,0);
	  sglRotate2D(PI*1/8,0,0);
	  sglEllipse(0,0,0,1.5f,0.7f);

	  sglColor3f(0,0,1);
	  sglLoadIdentity(); sglTranslate(0,-4,0);
	  sglTranslate(3,0,0);
	  sglScale(0.5f,2,1);
	  sglEllipse(0,0,0,1.5f,0.7f);

	  sglColor3f(0,1,1);
	  sglLoadIdentity(); sglTranslate(0,-4,0);
	  sglTranslate(6,0,0);
	  sglRotate2D(PI*1/8,0,0);
	  sglScale(0.5f,2,1);
	  sglEllipse(0,0,0,1.5f,0.7f);
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

	public static void DrawTestScene1C() 
	{
		HUB.sglClear(sglEClearBit.SGL_COLOR_BUFFER_BIT);
		//sglDisable(SGL_DEPTH_TEST);

		// set viewport
		//sglViewport(0, 0, WIDTH, HEIGHT);

		// set the projection matrix
		sglMatrixMode(SGL_PROJECTION);
		sglLoadIdentity();
		sglOrtho(-8, 8, -1, 12, -1, 1);

		// set the modelview matrix
		sglMatrixMode(SGL_MODELVIEW);
		sglLoadIdentity();

		// draw coord axes
		sglColor3f(0.5f,0.5f,0.5f);
		sglBegin(SGL_LINES);
		sglVertex2f(-7,0);
		sglVertex2f( 7,0);
		sglVertex2f( 0,0);
		sglVertex2f(0,10);
		sglEnd();

		sglColor3f(1,1,1);

		branch(0,1);
	}

	/// render a branch of a tree
	private static void branch(int depth,float rot)
	{
	  sglPushMatrix();
	  for(int i=0; i<3; i++)
	  {
	    sglScale(0.87f,0.87f,1);
	    sglRotate2D(rot*0.15f,0,1);
	    sglTranslate(0,2,0);
	    box();
	  }
	  if(depth<3)
	  {
	    branch(depth+1,1);
	    sglColor3f(0,1,0);
	    branch(depth+1,-1);
	  }
	  sglPopMatrix();
	}
}
