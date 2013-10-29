package rabbit.gl.struct;

import java.lang.ref.SoftReference;
import java.util.LinkedList;

public class CameraManager
{

	public static LinkedList<SoftReference<Camera>> cameras = new LinkedList<SoftReference<Camera>>();
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