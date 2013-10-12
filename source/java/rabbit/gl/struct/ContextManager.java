package rabbit.gl.struct;

import java.lang.ref.SoftReference;
import java.util.LinkedList;

import rabbit.gl.engine.Context;


public class ContextManager
{

	//int_fast8_t current;			// Maximum of 256 contexts
	//std::vector<Context> contexts;

	//private ContextManager()
	//{ current = 0; }

	public static void setContext(int id)
	{
		if ((id >= 0) && (id < contexts.size()))
			current = id;
		
		//else throw std::exception();
	}

	public static void deleteContext(int id)
	{
		if (id >= 0 && id < contexts.size())
			contexts.remove(id);
		
			//DELETE
			//contexts.erase(contexts.begin() + id);
	}
	
	private ContextManager(){}
	
	public static LinkedList<SoftReference<Context>> contexts = new LinkedList<SoftReference<Context>>();
	public static int current;
	
	public static void delete()
	{
		contexts.clear();		
	}

}
