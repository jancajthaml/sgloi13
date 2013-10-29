package rabbit.gl.primitive;

import rabbit.gl.struct.Vertex;

public class Sphere  implements Primitive
{

	public  Vertex   center ;
	public  float    radius ;
	
	public Sphere(Vertex center, float radius)
	{
		this.center = center;
		this.radius = radius;
	}

}
