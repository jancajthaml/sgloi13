package rabbit.gl.struct;

import rabbit.gl.math.SimpleMath;

public class Vertex
{

	public float x  =  0.0f;
	public float y  =  0.0f;
	public float z  =  0.0f;
	public float w  =  1.0f;
	
	public Vertex()
	{
		
	}
	
	public Vertex( float x, float y )
	{
		this.x  =  x;
		this.y  =  y;
	}
	
	public Vertex( float x, float y, float z )
	{
		this.x  =  x;
		this.y  =  y;
		this.z  =  z;
	}
	
	public Vertex( float x, float y, float z, float w )
	{
		this.x  =  x;
		this.y  =  y;
		this.z  =  z;
		this.w  =  w;
	}
	
	public float sqrdist( Vertex v )
	{ return sqr(x-v.x) + sqr(y-v.y) + sqr(z-v.z); }
	  
	public Vertex plus( Vertex v )
	{ return new Vertex( x+v.x , y+v.y , z+v.z ); }
	  
	public Vertex minus( Vertex v )
	{ return new Vertex( x-v.x , y-v.y , z-v.z ); }
	  
	public Vertex times( float w )
	{ return new Vertex( x*w , y*w , z*w ); }
	
	public void div(float A)
	{
		float a = 1.0f/A;
		x*=a;
		y*=a;
		z*=a;
	}
	
	public Vertex divide(float A)
	{ return times(1.0f/A); }

	public float sqrlen()
	{ return sqr(x) + sqr(y) + sqr(z); }
	  
	public float length()
	{ return SimpleMath.sqrt( sqrlen() ); }
	  
	public Vertex normalize()
	{ return divide( length() ); }

	private static float sqr(float a)
	{ return a*a; }
	
	public float magnitude() { return x*x + y*y + z*z; }

	public static Vertex crossProduct(Vertex A, Vertex B)
	{ return new Vertex(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); }
}