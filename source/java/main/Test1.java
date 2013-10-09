package main;

import static main.Main.sglBegin;
import static main.Main.sglVertex2f;
import static main.Main.sglEnd;
import static main.Main.sglColor3f;
import static main.Main.sglPointSize;
import static type.sglEElementType.SGL_LINES;
import static type.sglEElementType.SGL_POINTS;
import static type.sglEElementType.SGL_LINE_STRIP;
import static main.Main.sglScale;
import static main.Main.sglEllipse;
import static main.Main.sglRotate2D;
import static main.Main.sglTranslate;
import static main.Main.sglCircle;
import static main.Main.sglMatrixMode;
import static main.Main.sglLoadIdentity;
import static main.Main.sglOrtho;
import static main.Main.sglArc;
import static main.Main.sglAreaMode;
import static type.sglEMatrixMode.SGL_PROJECTION;
import static type.sglEMatrixMode.SGL_MODELVIEW;
import static type.sglEAreaMode.SGL_LINE;
import static type.sglEElementType.SGL_LINE_LOOP;
import static math.SimpleMath.sin;
import static math.SimpleMath.cos;
import static math.SimpleMath.PI;

public class Test1
{

	public static void DrawTestScene1A(int WIDTH, int HEIGHT) 
	{
	//  sglDisable(SGL_DEPTH_TEST);

	  // set viewport
	  //sglViewport(0, 0, WIDTH, HEIGHT);

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
}
