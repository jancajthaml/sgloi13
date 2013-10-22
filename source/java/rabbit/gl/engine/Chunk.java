package rabbit.gl.engine;

import java.awt.GraphicsEnvironment;
import java.awt.image.MemoryImageSource;

import rabbit.gl.struct.Color;
import rabbit.gl.struct.EdgeStack;
import rabbit.gl.struct.VertexStack;

//A brige between DrawingLibrary.h and Context.h
//C++ does not allow cross referencing and forwarding more than 3 parameters in a method call is evil
public class Chunk
{
	public static final int max_w = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice().getDisplayMode().getWidth();
	public static final int max_h = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice().getDisplayMode().getHeight();
	
	public static int[] _pixels;
	public static MemoryImageSource buffer; // COLOR BUFFER
	
	//Color* buffer					;	// COLOR BUFFER
	//float* depth					;	// DEPTH BUFFER
	//Color* clear					;	// CLEAR COLOR CACHE
	
	static
	{
		_pixels	= new int[max_w*max_h];
		buffer	= new MemoryImageSource(max_w, max_h, _pixels, 0, max_w);
		buffer.setAnimated(true);
	//	buffer = new JPanel().createImage(_mis);
	}

	public int w					;	// CONTEXT WIDTH  (maximum 65536)
	public int h					;	// CONTEXT HEIGHT (maximum 65536)
	public int w_h				;	// PRE-MULTIPLIED WIDTH * HEGIHT, must be 32bit (16bit is too small)
	
	public static int[] clearbuff			= new int[max_w*max_h];
	
	public Color color						;	// CURRENT ACTIVE COLOR

	
	public VertexStack vertices = new VertexStack()			;	//STACK OF VERTICIES
	public EdgeStack edges	= new EdgeStack()				;	//STACK OF EDGES

	public int size				;	//SIZE OF A PIXEL FOR DRAWING POINTS (maximum 256)

	public int lastSetPixelIndex	;	//CACHE OF setPixel(int,int) METHOD

	//bool depth						;	//DEPTH TEST
};