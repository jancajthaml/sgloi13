package rabbit.gl.struct;

import java.lang.ref.WeakReference;
import java.util.HashSet;
import java.util.Iterator;
import rabbit.struct.common.GenericStack;

public class VertexStack implements Iterable<Vertex>
{
	
	private GenericStack<Vertex>	stack		= null					;
	private static HashSet<Vertex>	using		= new HashSet<Vertex>()	;
	private static HashSet<Vertex>	available	= new HashSet<Vertex>()	;

	public VertexStack()
	{ stack = new GenericStack<Vertex>(); }

    public void push( Vertex id )
    { stack.push(get(id)); }

    public Vertex pop()
    {
    	Vertex v =  stack.pop();
    	release(v);
    	return v;
    }

	public Vertex get(int i)
	{ return stack.peek(i); }

	public int size()
	{ return stack.size(); }

	@Override public Iterator<Vertex> iterator()
	{ return stack.iterator(); }

	public void clear()
	{ stack.clear(); }

	public static Vertex create(float x, float y, float z, float w)
	{
		Vertex v					= new Vertex(x,y,z,w);
		WeakReference<Vertex> ref	= new WeakReference<Vertex>(get(v));
		v							= null;
		
		return ref.get();		
	}
	
	private synchronized static Vertex get(Vertex f)
	{
		if( available.size() > 0 )
		{
			for( Vertex t : available )
			{
				available . remove ( t ) ;
				using     . add    ( t ) ;
				return t;
			}
		}
		
		using.add(f);
		
		return f;
	}

	private synchronized static void release(Vertex v)
	{
		using     . remove ( v ) ;
		available . add    ( v ) ;
	}

}