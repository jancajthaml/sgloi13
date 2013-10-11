package tests;

import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglColor3f;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglPointSize;
import static rabbit.gl.engine.HUB.sglVertex2f;
import static rabbit.gl.math.SimpleMath.PI;
import static rabbit.gl.math.SimpleMath.cos;
import static rabbit.gl.math.SimpleMath.sin;
import static rabbit.gl.type.sglEElementType.SGL_LINES;
import static rabbit.gl.type.sglEElementType.SGL_POINTS;

public class Test0
{

	public static void DrawTestScene0A(int HEIGHT) 
	{
	  float centerX=0;
	  float centerY=0;

	  int numSegments = 32;
	  float angleStep = ((0.5f*PI)/(float)numSegments);
	  
	  int numCircles = 64;
	  float radiusStep = (HEIGHT)/numCircles;
	  float r = radiusStep;
	  
	  for(int j=0; j < numCircles; j++, r+=radiusStep)
	  {
		  sglColor3f(j/(float)numCircles,j/(float)numCircles,j/(float)numCircles);
		  sglPointSize(1);
		  
		  sglBegin(SGL_POINTS);
		  {
			  for(int i=0; i<numSegments; i++)
			  {
				  float angle = angleStep*(float)i;
				  sglVertex2f(centerX + r*cos(angle), centerY + r*sin(angle));
			  }
		  }
	    sglEnd();
	  }
	}

	public static void DrawTestScene0B(int HEIGHT) 
	{
	  float centerX=0, centerY=0;
	  int numSegments = 64;
	  float angleStep = (0.25f*PI/(float)numSegments);
	  float r=HEIGHT;
	  sglPointSize(1);
	  
	  for(int i=0; i<numSegments; i++)
	  {
		  sglColor3f(i/(float)numSegments,i/(float)numSegments,i/(float)numSegments);
		  float angle = angleStep*(float)i;
		  sglBegin(SGL_LINES);
		  sglVertex2f(centerX, centerY );
		  sglVertex2f(centerX + r*cos(angle), centerY + r*sin(angle));
		  sglEnd();
	  }
	}

}
