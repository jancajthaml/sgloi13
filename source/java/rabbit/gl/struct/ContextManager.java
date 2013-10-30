package rabbit.gl.struct;

import java.util.ArrayList;
import rabbit.gl.engine.Context;

public class ContextManager
{

	public static ArrayList<Context>	contexts = new ArrayList<Context>()	;
	public static int					current  = 0						;
	public static Context				working  = null						;

	private ContextManager(){}
	
	public static void setContext(int id)
	{
		
		if ((id >= 0) && (id < contexts.size()))
			current = id;
	}

	public static int create(int width, int height)
	{
		int number_of_contexts = contexts.size();
		working = new Context(width, height);
		
		ContextManager.contexts.add(working);
		
		return number_of_contexts;
	}
	
	public static void deleteContext(int id)
	{
		if (id >= 0 && id < contexts.size())
			contexts.remove(id);
	}
	
	public static void delete()
	{ contexts.clear(); }

}