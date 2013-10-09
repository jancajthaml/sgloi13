package struct;

public class Vertex
{

	float x;
	float y;
	float z;
	float w;
	
	public Vertex(float x, float y)
	{
		this.x = x;
		this.y = y;
		this.z = 0.0f;
		this.w = 0.0f;
	}
	
	public Vertex(float x, float y, float z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
		this.w = 0.0f;
	}
	
	public Vertex(float x, float y, float z, float w)
	{
		this.x = x;
		this.y = y;
		this.z = z;
		this.w = w;
	}

}