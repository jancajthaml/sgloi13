package tests;

import static rabbit.gl.engine.HUB.sglAreaMode;
import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglVertex3f;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglMaterial;
import static rabbit.gl.engine.HUB.sglLoadIdentity;
import static rabbit.gl.engine.HUB.sglEnable;
import static rabbit.gl.engine.HUB.sglViewport;
import static rabbit.gl.engine.HUB.sglSphere;
import static rabbit.gl.engine.HUB.sglMatrixMode;
import static rabbit.gl.engine.HUB.sglPerspective;
import static rabbit.gl.engine.HUB.sglLookAt;
import static rabbit.gl.type.sglEEnableFlags.SGL_DEPTH_TEST;

import rabbit.gl.engine.HUB;
import rabbit.gl.helpers.Movement;
import rabbit.gl.io.NFFStore;
import rabbit.gl.primitive.Material;
import rabbit.gl.primitive.Sphere;
import rabbit.gl.primitive.Triangle;

import static rabbit.gl.type.sglEAreaMode.SGL_LINE;
import static rabbit.gl.type.sglEElementType.SGL_POLYGON;
import static rabbit.gl.type.sglEMatrixMode.SGL_MODELVIEW;
import static rabbit.gl.type.sglEMatrixMode.SGL_PROJECTION;

public class TestNFF
{

	
	public static void draw(int WIDTH, int HEIGHT, NFFStore nffstore)
	{
		HUB.sglClearColor(nffstore.bg_col.r, nffstore.bg_col.g, nffstore.bg_col.b, 1.0f);
		sglAreaMode(SGL_LINE);
		sglEnable(SGL_DEPTH_TEST);
		
		sglViewport(0, 0, WIDTH, HEIGHT);

		  // projection transformation
		  sglMatrixMode(SGL_PROJECTION);
		  sglLoadIdentity();
		  sglPerspective (nffstore.angle, (float)WIDTH/HEIGHT, 1.0f, 1800.0f);
		  
		  // modelview transformation
		  sglMatrixMode(SGL_MODELVIEW);
		  sglLoadIdentity();
		  sglLookAt(
		    nffstore.from.v.x+Movement.x,
		    nffstore.from.v.y,
		    nffstore.from.v.z+Movement.z,
		    nffstore.at.v.x,
		    nffstore.at.v.y,
		    nffstore.at.v.z,
		    nffstore.up.v.x,
		    nffstore.up.v.y,
		    nffstore.up.v.z
		    );
		  
		  // render the geometry from the NFF file
		  for( NFFStore.MaterialGroup g : nffstore.matgroups)
		  {
			  
			  Material m = g.material;
			  
			  sglMaterial(m.col.r, m.col.g, m.col.b, m.kd, m.ks, m.shine, m.T, m.ior);

			  for( Triangle t : g.geometry )
			  {
				  sglBegin(SGL_POLYGON);
				  //sglBegin(SGL_TRIANGLES);	//<ÐÐ FIXME!!! Triangle fill doesn't work for Spheres
				  							// DRAW IS OK (dependent on drawPolygon)
				  
				  sglVertex3f( t.vertices[0].v.x, t.vertices[0].v.y, t.vertices[0].v.z);
				  sglVertex3f( t.vertices[1].v.x, t.vertices[1].v.y, t.vertices[1].v.z);
				  sglVertex3f( t.vertices[2].v.x, t.vertices[2].v.y, t.vertices[2].v.z);

				  sglEnd();
			  }
			  
			  
			  for( Sphere t : g.spheres )
			  {
				  //POLYGON OR TRIANGLE STRIP?
				  sglBegin(SGL_POLYGON);
				  
				  sglSphere(t);
				  
				  sglEnd();
			  }

		  }
	}

}
