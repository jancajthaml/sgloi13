package rabbit.gl.engine;

import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglVertex2f;
import static rabbit.gl.type.sglEElementType.SGL_LINE_STRIP;
import static rabbit.gl.type.sglEElementType.SGL_POLYGON;
import static rabbit.gl.type.sglEMatrixMode.SGL_MODELVIEW;


import java.awt.GraphicsEnvironment;
import java.awt.image.MemoryImageSource;
import java.util.ArrayDeque;
import java.util.LinkedList;

import rabbit.gl.math.SimpleMath;
import rabbit.gl.struct.Color;
import rabbit.gl.struct.Matrix;
import rabbit.gl.struct.Vertex;
import rabbit.gl.struct.Viewport;
import rabbit.gl.type.sglEElementType;
import rabbit.gl.type.sglEMatrixMode;

public class Context
{
	public int id							= 0;
	

	//std::vector<Matrix> transformStack;
	int lastSetPixelIndex;
	int w_h;

	
	static Vertex vertex_calculation_helper	= new Vertex (0,0,0,1);
	

    //Drawing
	private static final int max_w = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice().getDisplayMode().getWidth();
	private static final int max_h = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice().getDisplayMode().getHeight();
	
//	public static Image buffer;
	static int[] _pixels;
	public static MemoryImageSource buffer;
	
	static
	{
		_pixels	= new int[max_w*max_h];
		buffer	= new MemoryImageSource(max_w, max_h, _pixels, 0, max_w);
		buffer.setAnimated(true);
	//	buffer = new JPanel().createImage(_mis);
	}
	//static public Buffer buffer				= new Buffer(max_w,max_h);
	//public static int[] rgb_buffer			= new int[buffer.getWidth()*buffer.getHeight()];
	public static int[] clearbuff			= new int[max_w*max_h];
	
	
	public int w;
	public int h;
	public int s = w*h;
	
	public Color clear;
	public Color color;

	//State
	public boolean depth;

	//Pixel size
	public int size;



	//std::vector<Vertex> vertices;
	
	boolean projMatChanged;
	boolean modelMatChanged;

	
	
	
	
	public Matrix currentModelviewMatrix;
	public Matrix currentProjectionMatrix;
	
	public Matrix PMMatrix;
	public Matrix PMVMatrix;

	
	public LinkedList <Vertex> vertices = new LinkedList<Vertex>();
	public ArrayDeque <sglEElementType> types = new ArrayDeque<sglEElementType>();
	
	public Viewport viewport = new Viewport();
	public sglEMatrixMode matrixMode;

	public ArrayDeque <Matrix> projectionStack = new ArrayDeque<Matrix>();

	public Context(int width, int height)
	{
		//----------------------//

		//Initialise Drawing
		w			= width;
		h			= height;
		s			= width*height;
		id			= 0;
		//matrixMode	= NULL;

		// ? this shoud be static or const equivalent to NULL ?
		clear	= new Color(255,0,0);

		// ? this shoud be static or const equivalent to NULL ?
		color	= new Color(0,255,0);


		//----------------------//

		//Initialise Flags
		depth	= false;

		//----------------------//

		size	= 1;

		//this->width = width;
		//this->height = height;

		//buffer = new float[width*height*3];

		//if (!buffer) throw std::exception();

		currentModelviewMatrix = Matrix.identity();
		currentProjectionMatrix = Matrix.identity();

		PMMatrix	= Matrix.identity();
		PMVMatrix	= Matrix.identity();
		
		projMatChanged = true;
		modelMatChanged = true;
	}
	
	public void setVertex2f(float x, float y)
	{ vertices.add(transform(new Vertex(x,y))); }

	public void draw()
	{
		if (types.size() == 0);//throw std::exception();

		sglEElementType type = types.peekFirst();//.back();
		//types.pop_back();
		
		if(type==null) return;
		
		switch(type)
		{
			case SGL_POINTS				: drawPoints()		; break;
			case SGL_LINES				: drawLines()		; break;
			case SGL_LINE_STRIP			: drawLineStrip()	; break;
			case SGL_LINE_LOOP			: drawLineLoop()	; break;
			case SGL_TRIANGLES			: drawTriangles()	; break;
			case SGL_POLYGON			: drawPolygon()		; break;
			case SGL_AREA_LIGHT			: 					; break;
			case SGL_LAST_ELEMENT_TYPE	: 					; break;
			default : break;
		}
		
		try
		{
			vertices.clear();
		}catch(Throwable t){/*ignore*/}
	}

	public void drawPoints()
	{
		try
		{
		switch(size)
		{
			case 1 :
			{
				for(Vertex v_it : vertices)
					setPixel(v_it.x, v_it.y);
			} break;
			case 2 :
			{
				for(Vertex v_it : vertices)
				{
					setPixel(v_it.x  , v_it.y-1);
					setPixel(v_it.x  , v_it.y  );
					setPixel(v_it.x-1, v_it.y-1);
					setPixel(v_it.x-1, v_it.y  );
				}
			} break;
			case 3 :
			{
				for(Vertex v_it : vertices)
				{
					setPixel(v_it.x+1, v_it.y-1);
					setPixel(v_it.x+1, v_it.y  );
					setPixel(v_it.x+1, v_it.y+1);
					
					setPixel(v_it.x-1, v_it.y-1);
					setPixel(v_it.x-1, v_it.y  );
					setPixel(v_it.x-1, v_it.y+1);
					
					setPixel(v_it.x  , v_it.y-1);
					setPixel(v_it.x  , v_it.y  );
					setPixel(v_it.x  , v_it.y+1);
				}
			} break;
			default :
			{
				int thickness	= (((size%2)==0)?((size+1)):(size))>>1;
				int s			= size-1;
				
				for(Vertex v_it : vertices)
				for(int i = -thickness; i<s; i++)
				for(int j = -thickness; j<s; j++)
					setPixel(v_it.x+j, v_it.y+i);
			} break;
		
		}
		}catch(Throwable t){/*ignore*/}
	}

	public Vertex transform(Vertex v)
	{
		checkPMVMatrix();
		return PMVMatrix.multiply(v);
	}

	public float calculateRadiusScaleFactor()
	{
		checkPMVMatrix();
		return (float) (Math.sqrt((PMVMatrix.matrix[0] * PMVMatrix.matrix[5]) - (PMVMatrix.matrix[1] * PMVMatrix.matrix[4])));
	}
	
	void bresenham_circle(int xs, int ys, int r)
	{
		int x, y, p;
		x = 0;
		y = r;
		p = 3 - (r << 1);
		while (x < y)
		{
			setSymPixel(x, y, xs, ys);
			if (p < 0)
			{
				p += (x << 2) + 6;
			}
			else
			{
				p += ((x - y) << 2) + 10;
				y -= 1;
			}
			x += 1;
		}
		if (x == y)
			setSymPixel(x, y, xs, ys);
	}
	
	public void drawCricle(float x, float y, float z, float r)
	{
		try
		{
			vertex_calculation_helper.x = x;
			vertex_calculation_helper.y = y;
			Vertex v = transform(vertex_calculation_helper);
			bresenham_circle((int)(v.x), (int)(v.y), (int)SimpleMath.floor(r * calculateRadiusScaleFactor()));
		}catch(Throwable t){/*ignore*/}
	}

	public void drawEllipse(float center_x, float center_y, float center_z, float axis_x, float axis_y)
	{
		boolean ellipse_adaptive = false;

		if(ellipse_adaptive)
		{

		}
		else
		{
			sglBegin(SGL_POLYGON);
				//TRANSLATE!!!

				sglVertex2f( center_x										, center_y + axis_y									 );
				sglVertex2f( center_x + (axis_x * 0.1564344693575539f	)	, center_y + ( axis_y * 0.987688339911341f			));
				sglVertex2f( center_x + (axis_x * 0.3090170026893479f	)	, center_y + ( axis_y * 0.9510565135936411f			));
				sglVertex2f( center_x + (axis_x * 0.45399051142368685f	)	, center_y + ( axis_y * 0.8910065182350011f			));
				sglVertex2f( center_x + (axis_x * 0.587785266437776f		)	, center_y + ( axis_y * 0.8090169840977831f		));
				sglVertex2f( center_x + (axis_x * 0.7071067966408575f	)	, center_y + ( axis_y * 0.7071067657322372f			));
				sglVertex2f( center_x + (axis_x * 0.8090170097906934f	)	, center_y + ( axis_y * 0.5877852310745185f			));
				sglVertex2f( center_x + (axis_x * 0.891006511019614f		)	, center_y + ( axis_y * 0.4539905255846804f		));
				sglVertex2f( center_x + (axis_x * 0.9510565271012029f	)	, center_y + ( axis_y * 0.3090169611173454f			));
				sglVertex2f( center_x + (axis_x * 0.9876883560735247f	)	, center_y + ( axis_y * 0.15643436731351018f		));
				sglVertex2f( center_x + (axis_x * 0.999999999999999f		)	, center_y - ( axis_y * 4.371139000186241E-8f	));
				sglVertex2f( center_x + (axis_x * 0.9876883423975937f	)	, center_y - ( axis_y * 0.15643445365997144f		));
				sglVertex2f( center_x + (axis_x * 0.9510565000860774f	)	, center_y - ( axis_y * 0.30901704426134974f		));
				sglVertex2f( center_x + (axis_x * 0.8910064713304968f	)	, center_y - ( axis_y * 0.4539906034789449f			));
				sglVertex2f( center_x + (axis_x * 0.8090170284743339f	)	, center_y - ( axis_y * 0.5877852053586913f			));
				sglVertex2f( center_x + (axis_x * 0.7071067769704655f	)	, center_y - ( axis_y * 0.7071067854026294f			));
				sglVertex2f( center_x + (axis_x * 0.5877851957112599f	)	, center_y - ( axis_y * 0.809017035483602f			));
				sglVertex2f( center_x + (axis_x * 0.4539903804212881f	)	, center_y - ( axis_y * 0.8910065849840472f			));
				sglVertex2f( center_x + (axis_x * 0.3090168061705656f	)	, center_y - ( axis_y * 0.9510565774464436f			));
				sglVertex2f( center_x + (axis_x * 0.15643444188190603f	)	, center_y - ( axis_y * 0.9876883442630557f			));
				sglVertex2f( center_x - (axis_x * 8.742278000372475E-8f	)	, center_y - ( axis_y * 0.9999999999999962f			));
				sglVertex2f( center_x - (axis_x * 0.1564346145748253f	)	, center_y - ( axis_y * 0.9876883169111731f			));
				sglVertex2f( center_x - (axis_x * 0.3090169724585808f	)	, center_y - ( axis_y * 0.9510565234162125f			));
				sglVertex2f( center_x - (axis_x * 0.4539905362098265f	)	, center_y - ( axis_y * 0.8910065056058313f			));
				sglVertex2f( center_x - (axis_x * 0.5877853371642876f	)	, center_y - ( axis_y * 0.8090169327119581f			));
				sglVertex2f( center_x - (axis_x * 0.7071069006049366f	)	, center_y - ( axis_y * 0.7071066617681382f			));
				sglVertex2f( center_x - (axis_x * 0.8090171312459549f	)	, center_y - ( axis_y * 0.5877850639056469f			));
				sglVertex2f( center_x - (axis_x * 0.8910066589484567f	)	, center_y - ( axis_y * 0.4539902352578838f			));
				sglVertex2f( center_x - (axis_x * 0.951056480440929f	)	, center_y - ( axis_y * 0.3090171047228822f			));
				sglVertex2f( center_x - (axis_x * 0.9876883324525811f	)	, center_y - ( axis_y * 0.156434516450301f			));
				sglVertex2f( center_x - (axis_x * 0.9999999999999999f	)	, center_y + ( axis_y * 1.1924880454806035E-8f		));
				sglVertex2f( center_x - (axis_x * 0.9876883287216551f	)	, center_y + ( axis_y * 0.15643454000643153f		));
				sglVertex2f( center_x - (axis_x * 0.9510564730709448f	)	, center_y + ( axis_y * 0.30901712740535175f		));
				sglVertex2f( center_x - (axis_x * 0.8910064316413727f	)	, center_y + ( axis_y * 0.45399068137320586f		));
				sglVertex2f( center_x - (axis_x * 0.8090168369495607f	)	, center_y + ( axis_y * 0.5877854689698682f			));
				sglVertex2f( center_x - (axis_x * 0.7071065465657921f	)	, center_y + ( axis_y * 0.7071070158072251f			));
				sglVertex2f( center_x - (axis_x * 0.5877849321000184f	)	, center_y + ( axis_y * 0.8090172270082862f			));
				sglVertex2f( center_x - (axis_x * 0.45399051495953424f	)	, center_y + ( axis_y * 0.8910065164333968f			));
				sglVertex2f( center_x - (axis_x * 0.30901694977611f		)	, center_y + ( axis_y * 0.9510565307861931f			));
				sglVertex2f( center_x - (axis_x * 0.1564343555354446f	)	, center_y + ( axis_y * 0.9876883579389858f			));

			//	}

				sglEnd();
			}
		}
	public void drawArc2D(float x, float y, float z, float r, float from, float to)
	{
		try{
		float x2, y2;
		float N = 40 * (to - from)/(2 * SimpleMath.PI);
		float alpha = (to - from) / N;
		pushTypeState(SGL_LINE_STRIP);
		float offset = from / alpha;
		int fromOffset = (int)SimpleMath.floor(offset) - 1;
		float x1 = r * SimpleMath.cos(fromOffset * alpha);
		float y1 = r * SimpleMath.sin(fromOffset * alpha);
		float sa = SimpleMath.sin(alpha);
		float ca = SimpleMath.cos(alpha);
		for (int i = (int)SimpleMath.floor(offset); i <= (int)round(N) + (int)round(offset); i++)
		{
			x2 = ca * x1 - sa*y1;
			y2 = sa * x1 + ca*y1;
			//x2 = r * cos(i * alpha);
			//y2 = r * sin(i * alpha);
			setVertex2f(x2 + x, y2 + y);
			x1 = x2;
			y1 = y2;
		}
		draw();
		}catch(Throwable t){/*ignore*/}
	}
	
	public void drawLineStrip()
	{
		try
		{
			int size = vertices.size()-1;
			
			for (int i = 0; i < size; i++)
				drawLine2D(vertices.get(i), vertices.get(i+1));
			
		}catch(Throwable t){/*ignore*/}
	}

	public void drawLineLoop()
	{
		try
		{
			int size = vertices.size()-1;
		
			for (int i = 0; i < size; i++)
				drawLine2D(vertices.get(i), vertices.get(i+1));
			
			drawLine2D(vertices.get(size), vertices.get(0));
		}catch(Throwable t){/*ignore*/}
	}

	public void drawTriangles()
	{
	}

	public void drawPolygon()
	{
		try
		{
			int size = vertices.size()-1;
		//if(SGL_LINE)
		//{
			for (int i = 0; i < size; i++)
			{
				drawLine2D(vertices.get(i), vertices.get(i+1));
			}
		     //           sglDrawLines(vertexVector[i], vertexVector[i+1]);
		       //     }
			drawLine2D(vertices.get(size), vertices.get(0));

//		            sglDrawLines(vertexVector[vertexVector.size()-1], vertexVector[0]);
		//}

		//if(currContext()->currentAreaMode==SGL_FILL)
		//{
			//sglDrawPolygon();
		//}
		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
		}
		catch(Throwable t)
		{/* ignore */}
	}

	public void drawLines()
	{
		try
		{
			int size =  vertices.size();
			for (int i = 0; i < size; i += 2)
				drawLine2D(vertices.get(i), vertices.get(i+1));
			
		}catch(Throwable t){/*ignore*/}
	}

	//Line
	//Breceanuv algoritmus
	//DDA algoritmus (jednoduzsi)
	//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
	public void drawLine2D(Vertex a, Vertex b)
	{
		if(a == null || b == null) return;
		
		//obtain the points
		int x1, x2, y1, y2;
		x1 = (int)round(a.x);
		y1 = (int)round(a.y);
		x2 = (int)round(b.x);
		y2 = (int)round(b.y);

		int dx = SimpleMath.abs(x2 - x1);
		int dy = SimpleMath.abs(y2 - y1);
		
		if (dx > dy)
			if (x1 < x2)	bresenham_x(x1, y1, x2, y2);
			else			bresenham_x(x2, y2, x1, y1);
		else
			if (y1 < y2)	bresenham_y(y1, x1, y2, x2);
			else			bresenham_y(y2, x2, y1, x1);
	}

	public void bresenham_x(int x1, int y1, int x2, int y2)
	{
			int dx = x2 - x1;
			int dy = y2 - y1;

			int sign = 1;
			if (dy < 0)
				sign = -1;
			int c0, c1, p;
			c0 = (dy << 1) * sign;
			c1 = c0 - (dx << 1);
			p = c0 - dx;

			setPixel(x1, y1);
			for (int i = x1 + 1; i <= x2; ++i)
			{
			if (p < 0)
				p += c0;
			else
			{
				p += c1;
				y1 += sign;
			}

			setPixel(i, y1);
		}
	}

	void bresenham_y(int x1, int y1, int x2, int y2)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		int sign = 1;
		if (dy < 0)
			sign = -1;
		int c0, c1, p;
		c0 = (dy << 1) * sign;
		c1 = c0 - (dx << 1);
		p = c0 - dx;

		setPixel(y1, x1);
		for (int i = x1 + 1; i <= x2; ++i)
		{
			if (p < 0)
				p += c0;
			else
			{
				p += c1;
				y1 += sign;
			}

			setPixel(y1, i);
		}
	}

	float round(float number)
	{ return (float) Math.floor(number + 0.5); }
	
	void setPixel(int x, int y)
	{
		if (x >= 0 && x < w && y >= 0 && y < h)
		{
			lastSetPixelIndex			= (x+max_w*(h-y));
			_pixels[lastSetPixelIndex]	= color.getRGB();
			buffer.newPixels(x, h-y, 1, 1);
		}
	}


	void setPixel_x()
	{
		lastSetPixelIndex += 1;
		if (lastSetPixelIndex < w_h)
		{
			_pixels[lastSetPixelIndex]	= color.getRGB();
		}
			//buffer[lastSetPixelIndex] = color;
	}

	void setPixel_y()
	{
		lastSetPixelIndex += w;
		if (lastSetPixelIndex < w_h)
		{
			_pixels[lastSetPixelIndex]	= color.getRGB();
		}
			//buffer[lastSetPixelIndex] = color;
	}

	void setPixel_xy()
	{
		lastSetPixelIndex += w;
		if (lastSetPixelIndex < w_h)
		{
			_pixels[lastSetPixelIndex]	= color.getRGB();
		}
			//buffer[lastSetPixelIndex] = color;
	}

	void setPixel_mxy()
	{
		lastSetPixelIndex += w;
		if (lastSetPixelIndex < w_h)
		{
			_pixels[lastSetPixelIndex]	= color.getRGB();
		}
			//buffer[lastSetPixelIndex] = color;
	}

	void setPixel_xmy()
	{
		lastSetPixelIndex += 1 - w;
		if (lastSetPixelIndex < w_h)
		{
			//int offset = (x+w*y)*3;

			_pixels[lastSetPixelIndex]	= color.getRGB();
		}
			//buffer[lastSetPixelIndex] = color;
	}

	void setPixel(float x, float y)
	{ setPixel((int)x, (int)y); }

	int stackSize()
	{ return types.size(); }

	public void multiplyCurrentMatrix(Matrix m)
	{
		if (matrixMode == SGL_MODELVIEW)
		{
			modelMatChanged			= true;
			currentModelviewMatrix	= currentModelviewMatrix.multiply(m);
		}
		else
		{
			projMatChanged			= true;
			currentProjectionMatrix	= currentProjectionMatrix.multiply(m);
		}
	}

	public void setViewport(int width, int height, int x, int y)
	{ viewport.changeViewport(width, height, x, y); }

	public Matrix getCurrentMatrix()
	{ return (matrixMode == SGL_MODELVIEW)?currentModelviewMatrix:currentProjectionMatrix; }

	public void pushMatrix()
	{
		//switch is faster than if 
		switch(matrixMode)
		{
			case SGL_MODELVIEW :projectionStack.push(currentModelviewMatrix);  break;
			default : projectionStack.push(currentProjectionMatrix);break;
		}
		//if (matrixMode == SGL_MODELVIEW)	projectionStack.push(currentModelviewMatrix);//push_back
		//else 								projectionStack.push(currentProjectionMatrix);//push_back
	}

	public void setCurrentMatrix(Matrix matrix)
	{
		if (matrixMode == SGL_MODELVIEW)
		{
			modelMatChanged = true;
			currentModelviewMatrix = matrix;
		}
		else
		{
			projMatChanged = true;
			currentProjectionMatrix = matrix;
		}
	}

	public void setMatrixMode(sglEMatrixMode mode)
	{ matrixMode = mode; }

	public boolean invalidTypeStack()
	{ return (types.size() > 0); }

	public boolean stackEmpty()
	{ return (projectionStack.size() == 0); }

	public void popMatrix()
	{
		if (projectionStack.size() == 0)
		{
			//*err = SGL_STACK_UNDERFLOW;
		       	return;
		}
		if (matrixMode == SGL_MODELVIEW)
		{
			modelMatChanged			= true;
			currentModelviewMatrix	= projectionStack.pop();
		}
		else
		{
			projMatChanged			= true;
			currentProjectionMatrix	= projectionStack.pop();
		}
		//projectionStack.pop_back();
		
		//if (matrixMode == SGL_MODELVIEW)	current_mv	= transformStack.peekFirst();//.back();
		//else								current_p	= transformStack.peekFirst();//

		//transformStack.pop_back();
	}

	public void pushTypeState(sglEElementType type)
	{ types.push(type);/*.push_back(type);*/ }

	public void clearBuffer(byte ... what)
	{
		System.arraycopy(clearbuff, 0, _pixels, 0, max_w*max_h);
		buffer.newPixels(0,0, w,h);
		return;
	}
	

	boolean checkPMMatrix()
	{
		if (modelMatChanged || projMatChanged)
		{
			modelMatChanged	= false;
			projMatChanged	= false;
			PMMatrix		= currentProjectionMatrix.multiply(currentModelviewMatrix);
			return true;
		}
		return false;
	}

	void checkPMVMatrix()
	{
		if (checkPMMatrix() || viewport.viewportMatrixChanged)
		{
			viewport.viewportMatrixChanged = false;
			PMVMatrix = viewport.getViewportMatrix().multiply(PMMatrix);
		}
	}

	void setSymPixel(int x, int y, int xs, int ys)
	{
		int rx = x + xs;
		int ry = y + ys;
		int mrx = -x + xs;
		int mry = -y + ys;
		setPixel(rx, ry);
		setPixel(rx, mry);
		setPixel(mrx, ry);
		setPixel(mrx, mry);
		rx = x + ys;
		ry = y + xs;
		mrx = -x + ys;
		mry = -y + xs;
		setPixel(ry, rx);
		setPixel(ry, mrx);
		setPixel(mry, rx);
		setPixel(mry, mrx);
	}
}