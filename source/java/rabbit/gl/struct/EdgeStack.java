package rabbit.gl.struct;

import java.lang.ref.SoftReference;
import java.util.ConcurrentModificationException;
import java.util.Iterator;

import rabbit.gl.util.ReferenceManager;

public class EdgeStack implements Iterable<Edge>, Iterator<Edge>
{

	private static int BASIC_STACK_SIZE  =  500;

	private SoftReference<Edge>[]	stack;
	int					currentSize;	//16bit = 65536
	int					index;			//16bit = 65536
	private int pointer = 0;
	
	@SuppressWarnings("unchecked")
	public EdgeStack()
	{
		currentSize	= BASIC_STACK_SIZE;
		index		= 0;
		stack		= new SoftReference[currentSize];;
	}

	
    public void push( Edge id )
    {
    	if (index == currentSize)	realloc();
		stack[index++] = ReferenceManager.get(id);
    }

    public Edge pop()
    {
    	return stack[index--].get();
    }

    @SuppressWarnings("unchecked")
	void realloc()
	{
    	int old_size =currentSize; 
		currentSize <<= 1;
		SoftReference<Edge>[] tmp = this.stack;
		this.stack		= new SoftReference[currentSize];

		System.arraycopy(tmp, 0, this.stack, 0, old_size);
	}

	public Edge get(int i)
	{
		try{return stack[i].get();}
		catch(java.lang.ArrayIndexOutOfBoundsException e){return null;}
	}

	public int size() { return index;}

	@Override public boolean hasNext()
	{ return pointer<index; }

	@Override public Edge next()
	{ return stack[pointer++].get(); }

	@Override public void remove()
	{ throw new ConcurrentModificationException(); }

	@Override public Iterator<Edge> iterator()
	{
		pointer=0;
		return this;
	}

	public void clear()
	{
		index=0;	
	}

}