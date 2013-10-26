package rabbit.gl.helpers;

public class Helper {

	public static int min(float A, float B, float C)
	{ return (A<B)?((A<C)?(int)(A):(int)(C)):(int)(B); }

	public static int max(float A, float B, float C)
	{ return (A>B)?((A>C)?(int)(A):(int)(C)):(int)(B); }

}
