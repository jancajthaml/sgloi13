package rabbit.gl.struct;

import rabbit.gl.math.SimpleMath;

public class Color
{

	public float r;
	public float g;
	public float b;
	public float a;

	public Color()
	{ r = g = b = a = 1.0f; }
	
	public Color(float r, float g, float b)
	{
		this.r = SimpleMath.min(1.0f,r);
		this.g = SimpleMath.min(1.0f,g);
		this.b = SimpleMath.min(1.0f,b);
		this.a = 1.0f;
	}
	
	public Color(float r, float g, float b, float a)
	{
		this.r = r;
		this.g = g;
		this.b = b;
		this.a = a;
	}
	
	public void add(Color other)
	{
		this.r = (this.r + other.r) * 0.5f;
		this.g = (this.g + other.g) * 0.5f;
		this.b = (this.b + other.b) * 0.5f;
	}
	
	public void subs(Color other)
	{
		this.r = (this.r - other.r) * 0.5f;
		this.g = (this.g - other.g) * 0.5f;
		this.b = (this.b - other.b) * 0.5f;
	}

	public int getRGB()
	{ return ((int)(a * 255.0f) << 24) | ((int)(r * 255.0f) << 16) | ((int)(g * 255.0f) << 8) | ((int)(b * 255.0f)); }
	 
}