package rabbit.gl.io;

import static rabbit.gl.engine.HUB.sglAreaMode;
import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglColor3f;
import static rabbit.gl.engine.HUB.sglVertex3f;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglLoadIdentity;
import static rabbit.gl.engine.HUB.sglEnable;
import static rabbit.gl.engine.HUB.sglViewport;
import static rabbit.gl.engine.HUB.sglMatrixMode;
import static rabbit.gl.engine.HUB.sglPerspective;
import static rabbit.gl.engine.HUB.sglLookAt;
import static rabbit.gl.type.sglEEnableFlags.SGL_DEPTH_TEST;
import java.io.File;
import java.io.FileNotFoundException;
import rabbit.gl.engine.HUB;
import rabbit.gl.helpers.Movement;
import rabbit.gl.primitive.Material;
import rabbit.gl.primitive.Triangle;
import rabbit.struct.common.GenericStack;

import static rabbit.gl.type.sglEAreaMode.SGL_FILL;
import static rabbit.gl.type.sglEElementType.SGL_POLYGON;
import static rabbit.gl.type.sglEMatrixMode.SGL_MODELVIEW;
import static rabbit.gl.type.sglEMatrixMode.SGL_PROJECTION;

public class TestNFF
{

	public static NFFStore load(String filename)
	{
		NFFStore store = new NFFStore(true);
		try
		{ NFFRead.read(new File(filename),store); }
		catch (FileNotFoundException e) { e.printStackTrace(); }
		return store;
	}
	
	public static void draw(int WIDTH, int HEIGHT, NFFStore nffstore)
	{
		HUB.sglClearColor(nffstore.bg_col.r, nffstore.bg_col.g, nffstore.bg_col.b, 1.0f);
		sglAreaMode(SGL_FILL);
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
		    nffstore.from.x+Movement.x,
		    nffstore.from.y,
		    nffstore.from.z+Movement.z,
		    nffstore.at.x,
		    nffstore.at.y,
		    nffstore.at.z,
		    nffstore.up.x,
		    nffstore.up.y,
		    nffstore.up.z
		    );
		  
		  // render the geometry from the NFF file
		  for( NFFStore.MaterialGroup g : nffstore.matgroups)
		  {
			  Material m = g.material;
			  sglColor3f(m.col.r*m.kd, m.col.g*m.kd, m.col.b*m.kd );
			  GenericStack<Triangle> tlist = g.geometry;

			  for( Triangle t : tlist )
			  {
				  sglBegin(SGL_POLYGON);
				  
				  sglVertex3f( t.vertices[0].x, t.vertices[0].y, t.vertices[0].z);
				  sglVertex3f( t.vertices[1].x, t.vertices[1].y, t.vertices[1].z);
				  sglVertex3f( t.vertices[2].x, t.vertices[2].y, t.vertices[2].z);

				  sglEnd();
			  }

		  }
	}

}
