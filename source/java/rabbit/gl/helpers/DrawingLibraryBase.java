package rabbit.gl.helpers;

import rabbit.gl.engine.Chunk;
import rabbit.gl.struct.Vertex;

public class DrawingLibraryBase implements DrawingLibraryInterface
{
		public DrawingLibraryBase()
		{}

		public void drawLine2D		( Vertex a, Vertex b, Chunk context){ state.drawLine2D(a, b, context); }
		
		public void drawPoints		( Chunk context ){ state.drawPoints( context ); }
		
		public void drawLines		( Chunk context ){ state.drawLines( context ); }
		
		public void drawLineStrip	( Chunk context ){ state.drawLineStrip( context ); }
		
		public void drawLineLoop	( Chunk context ){ state.drawLineLoop(context ); }
		public void drawLineBezier	( Chunk context ){ state.drawLineBezier(context ); }
		
		public void drawPolygon	( Chunk context ){ state.drawPolygon( context ); }
		
		public void drawCircle		( Vertex v,float r, Chunk context){ state.drawCircle( v, r, context); }
		
		public void fillCircle		( Vertex v,float r,Chunk context ){ state.fillCircle( v, r, context ); }
		
		public void fillPolygon	( Chunk context ) { state.fillPolygon( context ); }

		public void fillTrianglesFan	( Chunk context ) {state.fillTrianglesFan(context);}
		public void fillTrianglesStrip	( Chunk context ) {state.fillTrianglesStrip(context);}
		
		public void set (DrawingLibraryInterface newState){ state = newState; }

  	  
    	
    	private DrawingLibraryInterface state;
};
