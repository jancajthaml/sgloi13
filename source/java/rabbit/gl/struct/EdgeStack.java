package rabbit.gl.struct;

import java.lang.ref.WeakReference;
import java.util.HashSet;
import java.util.Iterator;
import rabbit.struct.common.GenericStack;

public class EdgeStack implements Iterable<Edge>
{
	private GenericStack<Edge> 		stack		= null					;
	private static HashSet<Edge>	using		= new HashSet<Edge>()	;
	private static HashSet<Edge>	available	= new HashSet<Edge>()	;

	
	public EdgeStack()
	{ stack = new GenericStack<Edge>(); }
	
    public void push( Edge id )
    { stack.push(get(id)); }

    public Edge pop()
    {
    	Edge e = stack.pop();
    	release(e);
    	return e;
    }

	public Edge get(int i)
	{ return stack.peek(i); }

	public int size()
	{ return stack.size();}

	@Override public Iterator<Edge> iterator()
	{ return stack.iterator(); }

	public void clear()
	{ stack.clear(); }

	public static Edge create()
	{
		Edge v						= new Edge();
		WeakReference<Edge> ref		= new WeakReference<Edge>(get(v));
		v							= null;
		
		return ref.get();		
	}
	
	private synchronized static Edge get(Edge f)
	{
		if( available.size() > 0 )
		{
			for( Edge t : available)
			{
				available . remove ( t ) ;
				using     . add    ( t ) ;
				return t;
			}
		}
		
		using.add(f);
		
		return f;
	}

	private synchronized static void release(Edge v)
	{
		using     . remove ( v ) ;
		available . add    ( v ) ;
	}

}