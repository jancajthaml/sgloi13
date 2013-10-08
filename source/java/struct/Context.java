package struct;

import static type.sglEElementType.SGL_POLYGON;
import static type.sglEElementType.SGL_LINE_STRIP;
import static type.sglEMatrixMode.SGL_MODELVIEW;

import static main.Main.sglBegin;
import static main.Main.sglEnd;
import static main.Main.sglVertex2f;

import java.awt.image.BufferedImage;
import java.util.ArrayDeque;
import java.util.LinkedList;

import type.sglEElementType;
import type.sglEMatrixMode;

public class Context
{

	public int id;

    //Drawing
	public BufferedImage buffer;
	public int w;
	public int h;
	public Color clear;
	public Color color;

	//State
	public boolean depth;

	//Pixel size
	public int size;


	//Transformation matrices

	//Matrix model_view;
	//Matrix projection;

	public Matrix current_mv;
	public Matrix current_p;

	
	public LinkedList <Vertex> vertices = new LinkedList<Vertex>();
	public ArrayDeque <sglEElementType> types = new ArrayDeque<sglEElementType>();
	
	public Viewport viewport = new Viewport();
	public sglEMatrixMode matrixMode;

	public ArrayDeque <Matrix> transformStack = new ArrayDeque();

	public Context(int width, int height)
	{
		//----------------------//

		//Initialise Drawing
		w			= width;
		h			= height;
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
		buffer = new BufferedImage(width,height, BufferedImage.TYPE_INT_RGB);
		//buffer = new float[width*height*3];

		//if (!buffer) throw std::exception();

		current_mv = Matrix.identity();
		current_p = Matrix.identity();
	}

	public void setVertex2f(float x, float y)
	{
		Vertex v = (current_p.multiply(current_mv)).multiply(new Vertex(x, y, 0.0f, 0.0f));

		if (viewport.ready)
		{
			viewport.calculateWindowCoordinates(v);
		}
		else
		{
			v.x = x;
			v.y = y;
		}
		vertices.add(v);
	}

	public void draw()
	{
		if (types.size() == 0);//throw std::exception();

		sglEElementType type = types.peekFirst();//.back();
		//types.pop_back();

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

		vertices.clear();
	}

	public void drawPoints()
	{

		if(size==1)
		{
			for(Vertex v_it : vertices)
				setPixel(v_it.x, v_it.y);
		}
		else
		{
			int thickness = size;

			if((thickness%2)==0) thickness++;

			thickness >>= 1;

			for(Vertex v_it : vertices)
			for(int i = -thickness; i<size-1; i++)
			for(int j = -thickness; j<size-1; j++)
				setPixel(v_it.x+j, v_it.y+i);
		}
	}

	public Vertex transform(Vertex v)
	{
		v = (current_p.multiply(current_mv)).multiply(v);
		viewport.calculateWindowCoordinates(v);
		return v;
	}

	public float calculateRadiusScaleFactor()
	{
		Matrix m = current_p.multiply(current_mv);
		return (float) (Math.sqrt((m.matrix[0] * m.matrix[5]) - (m.matrix[1] * m.matrix[4])) * viewport.calculateRatio());
	}
	
	public void drawCricle(float x, float y, float z, float r)
	{
		//fix
		Vertex v = transform(new Vertex(x, y, 0.0f, 0.0f));
		
		x = v.x;
		y = v.y;
		z = v.z;
		
		//calculate r scale factor
		r = r * calculateRadiusScaleFactor(); 
		int p	= 1 - (int)r;
			int x0	= (int)(x);
			int y0	= (int)(y);
			int x1	= 0;
			int y1	= (int)(r);
			int x2	= 1;
			int y2	= (int)(-2*r);

			//4 points around center
			setPixel(x0        , y0+(int)(r));
			setPixel(x0        , y0-(int)(r));
			setPixel(x0+(int)(r) , y0);
			setPixel(x0-(int)(r) , y0);

			while(x1<y1)
			{
				if(p>=0)
				{
					y1--;
					y2 += 2;
					p += y2;
				}

				x1++;

				x2 += 2;
				p += x2;

				setPixel(x0+x1 , y0+y1);
				setPixel(x0-x1 , y0+y1);
				setPixel(x0+x1 , y0-y1);
				setPixel(x0-x1 , y0-y1);

				setPixel(x0+y1 , y0+x1);
				setPixel(x0-y1 , y0+x1);
				setPixel(x0+y1 , y0-x1);
				setPixel(x0-y1 , y0-x1);

			}
	}

	public void drawEllipse(float center_x, float center_y, float center_z, float axis_x, float axis_y)
		{
		//	Vertex v(center_x, center_y, center_z, 0.0f);
			//transform(v);
			//center_x = v.x;
			//center_y = v.y;
			//center_z = v.z;

			//calculate r scale factor
		//	float scaleR = calculateRadiusScaleFactor();
			//axis_x *= scaleR;
			//axis_y *= scaleR;

			boolean ellipse_adaptive = false;

			if(ellipse_adaptive)
			{

			}
			else
			{
				sglBegin(SGL_POLYGON);

				//float diff = 0.15707963267f;

				//for(int i = 0; i < 40; ++i)
				//{
				//sglVertex2f(center_x+(axis_x * sin(i*diff)), center_y+(axis_y * cos(i*diff)));
				//}

				//TRANSLATE!!!

				sglVertex2f( center_x										, center_y + axis_y								 );
				sglVertex2f( center_x + (axis_x * 0.1564344693575539f	)	, center_y + ( axis_y * 0.987688339911341f		));
				sglVertex2f( center_x + (axis_x * 0.3090170026893479f	)	, center_y + ( axis_y * 0.9510565135936411f		));
				sglVertex2f( center_x + (axis_x * 0.45399051142368685f	)	, center_y + ( axis_y * 0.8910065182350011f		));
				sglVertex2f( center_x + (axis_x * 0.587785266437776f		)	, center_y + ( axis_y * 0.8090169840977831f		));
				sglVertex2f( center_x + (axis_x * 0.7071067966408575f	)	, center_y + ( axis_y * 0.7071067657322372f		));
				sglVertex2f( center_x + (axis_x * 0.8090170097906934f	)	, center_y + ( axis_y * 0.5877852310745185f		));
				sglVertex2f( center_x + (axis_x * 0.891006511019614f		)	, center_y + ( axis_y * 0.4539905255846804f		));
				sglVertex2f( center_x + (axis_x * 0.9510565271012029f	)	, center_y + ( axis_y * 0.3090169611173454f		));
				sglVertex2f( center_x + (axis_x * 0.9876883560735247f	)	, center_y + ( axis_y * 0.15643436731351018f		));
				sglVertex2f( center_x + (axis_x * 0.999999999999999f		)	, center_y - ( axis_y * 4.371139000186241E-8f	));
				sglVertex2f( center_x + (axis_x * 0.9876883423975937f	)	, center_y - ( axis_y * 0.15643445365997144f		));
				sglVertex2f( center_x + (axis_x * 0.9510565000860774f	)	, center_y - ( axis_y * 0.30901704426134974f		));
				sglVertex2f( center_x + (axis_x * 0.8910064713304968f	)	, center_y - ( axis_y * 0.4539906034789449f		));
				sglVertex2f( center_x + (axis_x * 0.8090170284743339f	)	, center_y - ( axis_y * 0.5877852053586913f		));
				sglVertex2f( center_x + (axis_x * 0.7071067769704655f	)	, center_y - ( axis_y * 0.7071067854026294f		));
				sglVertex2f( center_x + (axis_x * 0.5877851957112599f	)	, center_y - ( axis_y * 0.809017035483602f		));
				sglVertex2f( center_x + (axis_x * 0.4539903804212881f	)	, center_y - ( axis_y * 0.8910065849840472f		));
				sglVertex2f( center_x + (axis_x * 0.3090168061705656f	)	, center_y - ( axis_y * 0.9510565774464436f		));
				sglVertex2f( center_x + (axis_x * 0.15643444188190603f	)	, center_y - ( axis_y * 0.9876883442630557f		));
				sglVertex2f( center_x - (axis_x * 8.742278000372475E-8f	)	, center_y - ( axis_y * 0.9999999999999962f		));
				sglVertex2f( center_x - (axis_x * 0.1564346145748253f	)	, center_y - ( axis_y * 0.9876883169111731f		));
				sglVertex2f( center_x - (axis_x * 0.3090169724585808f	)	, center_y - ( axis_y * 0.9510565234162125f		));
				sglVertex2f( center_x - (axis_x * 0.4539905362098265f	)	, center_y - ( axis_y * 0.8910065056058313f		));
				sglVertex2f( center_x - (axis_x * 0.5877853371642876f	)	, center_y - ( axis_y * 0.8090169327119581f		));
				sglVertex2f( center_x - (axis_x * 0.7071069006049366f	)	, center_y - ( axis_y * 0.7071066617681382f		));
				sglVertex2f( center_x - (axis_x * 0.8090171312459549f	)	, center_y - ( axis_y * 0.5877850639056469f		));
				sglVertex2f( center_x - (axis_x * 0.8910066589484567f	)	, center_y - ( axis_y * 0.4539902352578838f		));
				sglVertex2f( center_x - (axis_x * 0.951056480440929f		)	, center_y - ( axis_y * 0.3090171047228822f		));
				sglVertex2f( center_x - (axis_x * 0.9876883324525811f	)	, center_y - ( axis_y * 0.156434516450301f		));
				sglVertex2f( center_x - (axis_x * 0.9999999999999999f	)	, center_y + ( axis_y * 1.1924880454806035E-8f	));
				sglVertex2f( center_x - (axis_x * 0.9876883287216551f	)	, center_y + ( axis_y * 0.15643454000643153f		));
				sglVertex2f( center_x - (axis_x * 0.9510564730709448f	)	, center_y + ( axis_y * 0.30901712740535175f		));
				sglVertex2f( center_x - (axis_x * 0.8910064316413727f	)	, center_y + ( axis_y * 0.45399068137320586f		));
				sglVertex2f( center_x - (axis_x * 0.8090168369495607f	)	, center_y + ( axis_y * 0.5877854689698682f		));
				sglVertex2f( center_x - (axis_x * 0.7071065465657921f	)	, center_y + ( axis_y * 0.7071070158072251f		));
				sglVertex2f( center_x - (axis_x * 0.5877849321000184f	)	, center_y + ( axis_y * 0.8090172270082862f		));
				sglVertex2f( center_x - (axis_x * 0.45399051495953424f	)	, center_y + ( axis_y * 0.8910065164333968f		));
				sglVertex2f( center_x - (axis_x * 0.30901694977611f		)	, center_y + ( axis_y * 0.9510565307861931f		));
				sglVertex2f( center_x - (axis_x * 0.1564343555354446f	)	, center_y + ( axis_y * 0.9876883579389858f		));

			//	}

				sglEnd();
			}
		}
	public void drawArc2D(float x, float y, float z, float radius, float from, float to)
	{
		float step		= ((to - from) / 40);
		float circleX	= 0;
		float circleY	= 0;

		if (from >= to) return;

		sglBegin(SGL_LINE_STRIP);

		for (float fi = from; fi < to; fi += step)
		{
			circleX = (float)(x + radius * Math.cos(fi));
			circleY = (float)(y + radius * Math.sin(fi));
			sglVertex2f(circleX, circleY);
		}

		sglVertex2f((float)(x + radius * Math.cos(to)), (float)(y + radius * Math.sin(to)));
		sglEnd();
	}
	public void drawLineStrip()
	{
		for (int i = 0; i < (int)vertices.size()-1; i++)
			drawLine2D(vertices.get(i), vertices.get(i+1));
	}

	public void drawLineLoop()
	{
		for (int i = 0; i < (int)vertices.size()-1; i++)
			drawLine2D(vertices.get(i), vertices.get(i+1));
		drawLine2D(vertices.get((int)vertices.size()-1), vertices.get(0));
	}

	public void drawTriangles()
	{
	}

	public void drawPolygon()
	{
		//if(SGL_LINE)
		//{
			for (int i = 0; i < (int)vertices.size()-1; i++)
			{
				drawLine2D(vertices.get(i), vertices.get(i+1));
			}
		     //           sglDrawLines(vertexVector[i], vertexVector[i+1]);
		       //     }
			drawLine2D(vertices.get((int)vertices.size()-1), vertices.get(0));

//		            sglDrawLines(vertexVector[vertexVector.size()-1], vertexVector[0]);
		//}

		//if(currContext()->currentAreaMode==SGL_FILL)
		//{
			//sglDrawPolygon();
		//}
		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
	}

	public void drawLines()
	{
		for (int i = 0; i < (int)vertices.size(); i += 2)
			drawLine2D(vertices.get(i), vertices.get(i+1));
	}

	//Line
	//Breceanuv algoritmus
	//DDA algoritmus (jednoduzsi)
	//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
	public void drawLine2D(Vertex a, Vertex b)
	{

		//obtain the points
		int x1, x2, y1, y2;
		x1 = (int)round(a.x);
		y1 = (int)round(a.y);
		x2 = (int)round(b.x);
		y2 = (int)round(b.y);

		int dx = Math.abs(x2 - x1);
		int dy = Math.abs(y2 - y1);
		if (dx > dy)
			if (x1 < x2)
				bresenham_x(x1, y1, x2, y2);
			else
				bresenham_x(x2, y2, x1, y1);
		else
			if (y1 < y2)
				bresenham_y(y1, x1, y2, x2);
			else
				bresenham_y(y2, x2, y1, x1);

/*
		int x1		= int(start.x);
		int x2		= int(end.x);

		if (x1 > x2)
		{
			drawLine2D(end, start);
			return;
		}

		int y1		= int(start.y)		;
		int y2		= int(end.y)		;
		int slope						;
		int dx		= x2 - x1			;
		int dy		= y2 - y1			;

		if (dy < 0)
		{
			slope	= -1				;
			dy		= -dy				;
		}
		else slope	= 1					;

		int incE	= dy << 1			;
		int incNE	= (dy - dx) << 1	;
		int d		= (dy << 1) - dx	;
		int y		= y1				;
		int x							;

		for (x = x1; x <= x2; x++)
		{
			setPixel(x, y);

			if (d > 0)
			{
				d += incNE			;
				y += slope			;
			}
			else	d += incE			;
		}
		*/
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
			//int offset = (x+w*y);

			buffer.setRGB(x, y, color.getRGB());
			//buffer[offset]		= color.r;
			//buffer[offset+1]	= color.g;
			//buffer[offset+2]	= color.b;
		}
	}

	void setPixel(float x, float y)
	{ setPixel((int)x, (int)y); }

	int stackSize()
	{ return types.size(); }

	public void multiplyCurrentMatrix(Matrix m)
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= current_mv.multiply(m);
		else								current_p	= current_p.multiply(m);
	}

	public void setViewport(int width, int height, int x, int y)
	{ viewport.changeViewport(width, height, x, y); }


	public Matrix getCurrentMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)
			return current_mv;
	       	return current_p;
	}

	public void pushMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)	transformStack.push(current_mv);//push_back
		else 								transformStack.push(current_p);//push_back
	}

	public void setCurrentMatrix(Matrix matrix)
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= matrix;
		else					current_p	= matrix;
	}

	public void setMatrixMode(sglEMatrixMode mode)
	{ matrixMode = mode; }

	public boolean invalidTypeStack()
	{ return (types.size() > 0); }

	public boolean stackEmpty()
	{ return (transformStack.size() == 0); }

	public void popMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= transformStack.peekFirst();//.back();
		else								current_p	= transformStack.peekFirst();//

		//transformStack.pop_back();
	}

	public void pushTypeState(sglEElementType type)
	{ types.push(type);/*.push_back(type);*/ }

	public void clearBuffer(byte ... what)
	{
		//int size = w * h * 3;

		for(int i = 0; i<w; i++)
			for(int j = 0; j<h; j++)
			{
				//System.out.println(clear.getRGB()+" "+i+" "+j);
				buffer.setRGB(i, j, clear.getRGB());
			}
		
		return;
	}

}