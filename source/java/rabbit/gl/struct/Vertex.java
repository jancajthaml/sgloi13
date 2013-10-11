package rabbit.gl.struct;

public class Vertex
{

	public float x;
	public float y;
	public float z;
	public float w;
	
	public Vertex(float x, float y)
	{
		this.x = x;
		this.y = y;
		this.z = 0.0f;
		this.w = 1.0f;
	}
	
	public Vertex(float x, float y, float z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
		this.w = 1.0f;
	}
	
	public Vertex(float x, float y, float z, float w)
	{
		this.x = x;
		this.y = y;
		this.z = z;
		this.w = w;
	}

}