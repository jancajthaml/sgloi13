package rabbit.gl.struct;

import java.lang.ref.WeakReference;
import java.util.HashSet;
import java.util.Iterator;
import rabbit.struct.common.GenericStack;

public class MatrixStack implements Iterable<Matrix>
{
	
	private GenericStack<Matrix>	stack		= null					;
	private static HashSet<Matrix>	using		= new HashSet<Matrix>()	;
	private static HashSet<Matrix>	available	= new HashSet<Matrix>()	;

	public MatrixStack()
	{ stack = new GenericStack<Matrix>(); }

    public void push( Matrix id )
    { stack.push(get(id)); }

    public Matrix pop()
    {
    	Matrix v =  stack.pop();
    	release(v);
    	return v;
    }

	public Matrix get(int i)
	{ return stack.peek(i); }

	public int size()
	{ return stack.size(); }

	@Override public Iterator<Matrix> iterator()
	{ return stack.iterator(); }

	public void clear()
	{ stack.clear(); }

	public static Matrix create(float m11, float m21, float m31, float m41, float m12, float m22, float m32, float m42, float m13, float m23, float m33, float m43,  float m14, float m24, float m34, float m44)
	{
		Matrix v					= new Matrix(m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43,  m14, m24, m34, m44);
		WeakReference<Matrix> ref	= new WeakReference<Matrix>(get(v));
		v							= null;
		
		return ref.get();		
	}

	public static Matrix create(float[] matrix)
	{
		Matrix v					= new Matrix(matrix);
		WeakReference<Matrix> ref	= new WeakReference<Matrix>(get(v));
		v							= null;
		
		return ref.get();
	}
	
	private synchronized static Matrix get(Matrix f)
	{
		if( available.size() > 0 )
		{
			for( Matrix t : available )
			{
				available . remove ( t ) ;
				using     . add    ( t ) ;
				return t;
			}
		}
		
		using.add(f);
		
		return f;
	}

	private synchronized static void release(Matrix v)
	{
		using     . remove ( v ) ;
		available . add    ( v ) ;
	}

}