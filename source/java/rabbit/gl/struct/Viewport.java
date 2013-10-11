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
		if (ready)
		{
			v.x = (v.x + 1) * width_2_x;
			v.y = (v.y + 1) * height_2_y;
		}
	}

	public void changeViewport(int width, int height, int x, int y)
	{
		this.width = width;
		this.height = height;
		this.width_2_x = ((float)width/2) + x;
		this.height_2_y = ((float)height/2) + y;
		this.x = x;
		this.y = y;
		viewportMatrix = new Matrix(width/2, 0.0f, 0.0f, 0.0f, 0.0f, height/2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, width/2 + x, height/2 + x, 0.0f, 1.0f);
		ready = true;
		viewportMatrixChanged = true;
	}
	

	public Matrix getViewportMatrix()
	{
		return viewportMatrix;
	}

}