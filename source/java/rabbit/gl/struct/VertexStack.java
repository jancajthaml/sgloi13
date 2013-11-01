package rabbit.gl.struct;

import java.util.ArrayList;
import java.util.Iterator;

import rabbit.struct.common.GenericStack;

public class VertexStack implements Iterable<Vertex>
{

	private GenericStack<Vertex>	stack		= null					;
	private static ArrayList<Vertex> cache = new ArrayList<Vertex>();
	private static int cache_index = 0;

	public VertexStack()
	{ stack = new GenericStack<Vertex>(); }

    public void push(float x, float y, float z, float w)
    { stack.push(create(x,y,z,w)); }

    public void push(Vertex v)
    { push( v.v.x , v.v.y , v.v.z , v.v.w ); }
    

    public Vertex pop()
    {
    	return stack.pop();
    }

	public Vertex get(int i)
	{
		return stack.peek(i);
	}

	public int size()
	{ return stack.size(); }

	@Override public Iterator<Vertex> iterator()
	{ return stack.iterator(); }

	public void clear()
	{
		VertexStack.cache_index=0;
		stack.clear();
	}

	public static Vertex create(float x, float y, float z, float w)
	{
		//System.out.println(cache.size());

		try
		{
			Vertex v = cache.get(VertexStack.cache_index);
			
			v.v.x = x;
			v.v.y = y;
			v.v.z = z;
			v.v.w = w;
			VertexStack.cache_index++;
			return v;
		}
		catch(Throwable t)
		{
			Vertex f = new Vertex();
			f.v.x = x;
			f.v.y = y;
			f.v.z = z;
			f.v.w = w;
			
			cache.add(VertexStack.cache_index++,f);
			return f;
		}
		
		
	}
	

}