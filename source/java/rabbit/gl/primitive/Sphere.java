package rabbit.gl.primitive;

import rabbit.gl.struct.Vertex;

public class Sphere implements Primitive
{
	  public Vertex center  =  new Vertex();
	  public float    radius  =  0.0f;
	  
	  public Sphere()
	  {}
  
	  public Sphere(Vertex c, float r)
	  {
		  this.center = c;
		  this.radius = r;	
	  }
};

