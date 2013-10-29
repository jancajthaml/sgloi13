package rabbit.gl.struct;

public class Viewport
{
	int	width;
	int	height;
	float width_2_x;
	float height_2_y;
	int x;
	int y;
	public boolean ready;

	Matrix viewportMatrix;
	public boolean viewportMatrixChanged;

	public Viewport()
	{
		width = 0;
		height = 0;
		width_2_x = 0.0f;
		height_2_y = 0.0f;
		x = 0;
		y = 0;
		ready = false;
		viewportMatrixChanged = true;
	}

	public float calculateRatio()
	{
		return (float) (Math.sqrt(width * width + height * height)/2.824f);
	}

	public Viewport(int width, int height, int x, int y)
	{
		changeViewport(width, height, x, y);
	}

	
	
	public void calculateWindowCoordinates(Vertex v)
	{
		if( ready )
		{
			v.x = x + (v.x + 1) * width_2_x;
			v.y = y + (v.y + 1) * height_2_y;
		}
	}

	public void changeViewport(int x, int y, int width, int height)
	{
		this.width					=  width;
		this.height				=  height;
		this.width_2_x				=  width>>1;
		this.height_2_y			=  height>>1;
		this.x						=  x;
		this.y						=  y;

		this.viewportMatrix		=  new Matrix
		(
			width_2_x, 0.0f, 0.0f, 0.0f,
			0.0f, height_2_y, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			width_2_x + x, height_2_y + x, 0.0f, 1.0f
		);
		this.ready		=  true;
		this.viewportMatrixChanged	=  true;
	}
	
	public Matrix getViewportMatrix()
	{
		return viewportMatrix;
	}

}