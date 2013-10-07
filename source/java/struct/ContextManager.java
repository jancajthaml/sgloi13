package struct;

import java.util.LinkedList;

public class ContextManager
{

	private ContextManager(){}
	
	public static LinkedList<Context> contexts = new LinkedList<Context>();
	public static int current;

}
