package rabbit.gl.struct;

import java.util.LinkedList;

import rabbit.gl.engine.Context;


public class ContextManager
{

	private ContextManager(){}
	
	public static LinkedList<Context> contexts = new LinkedList<Context>();
	public static int current;

}
