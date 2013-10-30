package rabbit.gl.struct;

import java.util.LinkedList;

public class CameraManager
{

	public static LinkedList<Camera> cameras = new LinkedList<Camera>();
	public static int current;

	private CameraManager(){}
		
	public static void setCamera(int id)
	{
		if ((id >= 0) && (id < cameras.size()))
			current = id;
	}

	public static void deleteCamera(int id)
	{
		if (id >= 0 && id < cameras.size())
			cameras.remove(id);
	}

	public static void delete()
	{ cameras.clear(); }

}