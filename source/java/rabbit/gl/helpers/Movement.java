package rabbit.gl.helpers;

public class Movement
{

	private static int speed = 30;
    public static int x = 0;
    public static int z = 0;
    
	
	public static void rightDown()	{ right(); down(); }
	public static void leftDown()	{ left();  down(); }
	public static void leftUp()		{ left();  up();   }
	public static void rightUp()	{ right(); up();   }
	
	public static void up()			{ z+=speed; }
	public static void down()		{ z-=speed; }
	public static void left()		{ x-=speed; }
	public static void right()		{ x+=speed; }
	
}
