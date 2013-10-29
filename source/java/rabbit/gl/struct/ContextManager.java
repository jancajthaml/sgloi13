package rabbit.gl.struct;

import java.lang.ref.SoftReference;
import java.util.LinkedList;

import rabbit.gl.engine.Context;

public class ContextManager
{

	public static LinkedList<SoftReference<Context>> contexts = new LinkedList<SoftReference<Context>>();
	public static int current;

	private ContextManager(){}
	
	public static void setContext(int id)
	{
		if ((id >= 0) && (id < contexts.size()))
			current = id;
	}

	public static void deleteContext(int id)
	{
		if (id >= 0 && id < contexts.size())
			contexts.remove(id);
	}
	
	public static void delete()
	{ contexts.clear(); }

}