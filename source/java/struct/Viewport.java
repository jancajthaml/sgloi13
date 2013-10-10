package struct;

public class Viewport
{


	int	width;
	int	height;
	float width_2_x;
	float height_2_y;
	int x;
	int y;
	boolean ready;
	
	public Viewport()
	{
		width = 0;
		height = 0;
		width_2_x = 0.0f;
		height_2_y = 0.0f;
		x = 0;
		y = 0;
		ready = false;
	}

	float calculateRatio()
	{
		return (float) (Math.sqrt(width * width + height * height)/2.824f);
	}

	public Viewport(int width, int height, int x, int y)
	{
		changeViewport(width, height, x, y);
	}

	void calculateWindowCoordinates(Vertex v)
	{
		v.x = (v.x + 1) * width_2_x;
		v.y = (v.y + 1) * height_2_y;
	}

	void changeViewport(int width, int height, int x, int y)
	{
		this.width			= width;
		this.height			= height;
		this.width_2_x		= (float)(width>>1) + x;
		this.height_2_y		= (float)(height>>1) + y;
		this.x				= x;
		this.y				= y;
		ready				= true;
	}

}