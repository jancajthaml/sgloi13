package rabbit.gl.struct;

import java.lang.ref.SoftReference;
import java.util.ConcurrentModificationException;
import java.util.Iterator;

import rabbit.gl.util.ReferenceManager;

public class VertexStack implements Iterable<Vertex>, Iterator<Vertex>
{

	private static int BASIC_STACK_SIZE  =  100;

	private SoftReference<Vertex>[]	stack;
	int					currentSize;	//16bit = 65536
	int					index;			//16bit = 65536
	private int pointer = 0;
	
	@SuppressWarnings("unchecked")
	public VertexStack()
	{
		currentSize	= BASIC_STACK_SIZE;
		index		= 0;
		stack		= new SoftReference[currentSize];
	}

	
    public void push( Vertex id )
    {
    	if (index == currentSize)	realloc();
		stack[index++] = ReferenceManager.get(id);
    }

    public Vertex pop()
    {
    	Vertex v =  stack[index].get();
    	stack[index--].clear();
    	return v;
    }

    @SuppressWarnings("unchecked")
	void realloc()
	{
    	int old_size                   =  currentSize; 
		currentSize                  <<=  1;
		SoftReference<Vertex>[] tmp    =  this.stack;
		this.stack		               =  new SoftReference[currentSize];

		System.arraycopy(tmp, 0, this.stack, 0, old_size);
	}

	public Vertex get(int i)
	{
		try{ return stack[i].get(); }
		catch(java.lang.ArrayIndexOutOfBoundsException e){return null;}
	}

	public int size()
	{
		//System.out.println(index+" "+this.stack.length);
		return index;
	}

	@Override public boolean hasNext()
	{ return pointer<index; }

	@Override public Vertex next()
	{ return stack[pointer++].get(); }

	@Override public void remove()
	{ throw new ConcurrentModificationException(); }

	@Override public Iterator<Vertex> iterator()
	{
		pointer=0;
		return this;
	}

	public void clear()
	{
		index=0;	
	}

}