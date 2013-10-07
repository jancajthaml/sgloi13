package struct;

public class Color
{

	float r;
	float g;
	float b;
	float a;

	public Color()
	{ r = g = b = a = 1.0f; }
	
	public Color(float r, float g, float b)
	{
		this.r = r;
		this.g = g;
		this.b = b;
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
}
