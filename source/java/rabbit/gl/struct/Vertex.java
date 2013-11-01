package rabbit.gl.struct;

import rabbit.gl.math.SimpleMath;

public class Vertex
{
    public Geometric	v  =  new Geometric();
    public Texture		t  =  new Texture();
    public Normal		n  =  new Normal();
    
    public static class Geometric
    {

        public float x = 0.0f;
        public float y = 0.0f;
        public float z = 0.0f;
        public float w = 1.0f;

    	public Geometric()
    	{
    		
    	}
    	
    	public Geometric( float x, float y )
    	{
    		this.x  =  x;
    		this.y  =  y;
    	}
    	
    	public Geometric( float x, float y, float z )
    	{
    		this.x  =  x;
    		this.y  =  y;
    		this.z  =  z;
    	}
    	
    	public Geometric( float x, float y, float z, float w )
    	{
    		this.x  =  x;
    		this.y  =  y;
    		this.z  =  z;
    		this.w  =  w;
    	}
    }
    
    public static class Normal
    {
    	
    	public float x = 0;
        public float y = 0;
        public float z = 0;

        public Normal()
        {
        	
        }
        
        public void add(float x, float y, float z)
        {
        	this.x += x;
        	this.y += y;
        	this.z += z;
        }

        public Normal(float x, float y, float z)
        {
        	this.x = x;
        	this.y = y;
        	this.z = z;
        }
    }
    
    public static class Texture
    {

        public float u = 0;
        public float v = 0;

        public Texture()
        {
        	
        }
        
        public Texture(float u, float v)
        {
            this.u = u;
            this.v = v;
        }
    }

    
	

	public float sqrdist( Vertex w )
	{ return sqr(v.x-w.v.x) + sqr(v.y-w.v.y) + sqr(v.z-w.v.z); }
	  
	public Vertex plus( Vertex w )
	{
		Vertex r = new Vertex();
		r.v.x = v.x+w.v.x;
		r.v.y = v.y+w.v.y;
		r.v.z = v.z+w.v.z;
		return r;
	}
	  
	public Vertex minus( Vertex w )
	{
		Vertex r = new Vertex();
		r.v.x = v.x-w.v.x;
		r.v.y = v.y-w.v.y;
		r.v.z = v.z-w.v.z;
		return r;
	}
	  
	public Vertex times( float w )
	{
		Vertex r = new Vertex();
		r.v.x=v.x*w;
		r.v.y=v.y*w;
		r.v.z=v.z*w;
		return r;
	}
	
	public void div(float A)
	{
		float a = 1.0f/A;
		v.x*=a;
		v.y*=a;
		v.z*=a;
	}
	
	public Vertex divide(float A)
	{ return times(1.0f/A); }

	public float sqrlen()
	{ return sqr(v.x) + sqr(v.y) + sqr(v.z); }
	  
	public float length()
	{ return SimpleMath.sqrt( sqrlen() ); }
	  
	public Vertex normalize()
	{ return divide( length() ); }

	private static float sqr(float a)
	{ return a*a; }
	
	public float magnitude() { return v.x*v.x + v.y*v.y + v.z*v.z; }

	public static Vertex crossProduct(Vertex A, Vertex B)
	{
		Vertex v = new Vertex();
		v.v.x = A.v.y * B.v.z - A.v.z * B.v.y;
		v.v.y = A.v.z * B.v.x - A.v.x * B.v.z;
		v.v.z = A.v.x * B.v.y - A.v.y * B.v.x;
		return v;
	}

}