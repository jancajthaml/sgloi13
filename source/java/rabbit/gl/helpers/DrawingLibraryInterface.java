package rabbit.gl.helpers;

import rabbit.gl.engine.Chunk;
import rabbit.gl.struct.Vertex;

public interface DrawingLibraryInterface
{
		public void drawLine2D		( Vertex a, Vertex b, Chunk context);
		public void drawPoints		( Chunk context );
		public void drawLines		( Chunk context );
		public void drawLineStrip	( Chunk context );
		public void drawLineLoop	( Chunk context );
		public void drawPolygon		( Chunk context );
		public void drawCircle		( Vertex v,float r, Chunk context );
		public void fillCircle		( Vertex v,float r,Chunk context );
		public void fillPolygon		( Chunk context );
		public void fillTrianglesFan	( Chunk context );
		public void fillTrianglesStrip	( Chunk context );
		
	//protected:
	//	protected DrawingLibraryInterface()	{};
    	//~DrawingLibraryInterface()	{};

};
