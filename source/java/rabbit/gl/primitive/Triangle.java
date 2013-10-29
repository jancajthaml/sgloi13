package rabbit.gl.primitive;

import rabbit.gl.struct.Vertex;

public class Triangle
{
	public Vertex[] vertices = new Vertex[3];

	public Triangle( Vertex v0, Vertex v1, Vertex v2 )
	{
		vertices[0] = v0;
		vertices[1] = v1;
		vertices[2] = v2;
	}

};
