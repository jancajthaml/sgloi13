package rabbit.gl.util;

import java.lang.ref.SoftReference;
import java.util.Enumeration;
import java.util.HashSet;

public class ReferenceManager
{	
	private ReferenceManager(){}
	
	public static<T extends Object> SoftReference<T> create(T t)
	{ return new SoftReference<T>(t); }
	

	private static HashSet<SoftReference<?>> locked	= new HashSet<SoftReference<?>>();
	private static HashSet<SoftReference<?>> unlocked	= new HashSet<SoftReference<?>>();

	@SuppressWarnings("unchecked")
	public synchronized static<T extends Object> SoftReference<T> get(T f)
	{
		SoftReference<T> t	= null;
		
		if (unlocked.size() > 0)
		{
			Enumeration<T> e = (Enumeration<T>) unlocked.iterator();
			while (e.hasMoreElements())
			{
				t = (SoftReference<T>) e.nextElement();
				unlocked . remove(t);
				locked   . add(t);
				return t;
			}
		}
		
		t = create(f);
		
		locked.add(t);
		return t;
	}

	public synchronized static <T extends Object> void release(SoftReference<T> object)
	{
		locked.remove ( object );
		unlocked.add  ( object );
	}

}