package rabbit.gl.util;

import java.lang.ref.SoftReference;
import java.util.HashMap;

public class ReferenceManager
{
	private static final HashMap<Object,SoftReference<?>> cache = new HashMap<Object,SoftReference<?>>();
	
	private ReferenceManager(){}

	@SuppressWarnings("unchecked")
	public static<T extends Object> SoftReference<T> get(T t)
	{
		SoftReference<T> ref = ((SoftReference<T>) cache.get(t));
		if(ref==null)
		{
			ref=create(t);
			cache.put(t, ((SoftReference<?>) ref));
		}
		
		return ref;
	}
	
	public static<T extends Object> SoftReference<T> create(T t)
	{ return new SoftReference<T>(t); }
	
	public static<T> void delete(SoftReference<T> ref)
	{ ref.clear(); }
	
}
