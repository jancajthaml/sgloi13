package rabbit.gl.struct;

public class Edge {


	public float min_y;
	public float max_y;
	public float delta_x;
	public float delta_z;
	public float x;
	public float z;
	
	public Edge()
	{
		min_y      = 0.0f;
		max_y      = 0.0f;
		delta_x    = 0.0f;
		delta_z    = 0.0f;
		x   	   = 0.0f;
		z          = 0.0f;
	}

	public boolean equals(Object other)
	{
		if(!(other instanceof Edge)) return false;
		
		Edge v = (Edge)other;

		return min_y==v.min_y && max_y==v.max_y && delta_x==v.delta_x && delta_z==v.delta_z && x==v.x && z==v.z;		
	}

}