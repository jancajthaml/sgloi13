/*
 * data.h
 *
 *  Created on: 23.9.2013
 *      Author: jancajthaml
 */

#include <vector>
#include <list>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "sgl.h"
#include "Vertex.h";
#include "Matrix.h";
#include "Viewport.h";
#include "Color.h";
#ifndef DATA_H_
#define DATA_H_


/*
 * Side-notes:
 * Jan Cajthaml -using struct instead of class because of no need of encapsulation
 *
 * */

/*
 * ChangeLog:
 * 23.9.2013, Jan Cajthaml - added struct Vertex
 * 23.9.2013, Jan Cajthaml - added struct Edge
 * 24.9.2013, Jan Cajthaml - added struct Color
 * 24.9.2013, Jan Cajthaml - added struct Matrix
 * 24.9.2013, Jan Cajthaml - added struct Context
 * 25.9.2013, Jan Cajthaml - added ViewPort & depth flag to Context
 * 25.9.2013, Jan Cajthaml - added struct ContextManager
 * 30.9.2013, Jan Cajthaml - added operator overload for aritmetic operations on Matrix
 * 30.9.2013, Jan Cajthaml - added operator overload for aritmetic operations on Color
 * 30.9.2013, Jan Cajthaml - added Model,Projection,Current and MPV (Model-Projection-View pre-calculated) matricies to Context
 * */

//Context
struct Context
{
	int id;

    //Drawing
	float *buffer;
	int w;
	int h;
	Color clear;
	Color color;

	//State
	bool depth;

	//Pixel size
	int size;


	//Transformation matrices

	//Matrix model_view;
	//Matrix projection;

	Matrix current_mv;
	Matrix current_p;

	std::vector<Vertex> vertices;

	std::vector<sglEElementType> types;
	Viewport viewport;
	sglEMatrixMode matrixMode;

	std::vector<Matrix> transformStack;

	Context(int width, int height)
	{
		//----------------------//

		//Initialise Drawing
		w			= width;
		h			= height;
		id			= 0;
		//matrixMode	= NULL;

		// ? this shoud be static or const equivalent to NULL ?
		clear	= Color(255,0,0);

		// ? this shoud be static or const equivalent to NULL ?
		color	= Color(0,255,0);

		//----------------------//

		//Initialise Flags
		depth	= false;

		//----------------------//

		size	= 1;

		//this->width = width;
		//this->height = height;
		buffer = (float*) malloc(sizeof(float) * width * height * 3);

		if (!buffer) throw std::exception();

		current_mv = Matrix::identity();
		current_p = Matrix::identity();
	}

	void setVertex2f(float x, float y)
	{
		Vertex v(x, y);
		v = (current_p * current_mv) * v;

		if (viewport.ready)
		{
			viewport.calculateWindowCoordinates(v);
		}
		else
		{
			v.x = x;
			v.y = y;
		}
		vertices.push_back(v);
	}

	void draw()
	{
		if (types.size() == 0) throw std::exception();

		sglEElementType type = types.back();
		types.pop_back();

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

	void drawPoints()
	{

		if(size==1)
		{
			for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
				setPixel(v_it->x, v_it->y);
		}
		else
		{
			int thickness = size;

			if((thickness%2)==0) thickness++;

			thickness >>= 1;

			for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			for(int i = -thickness; i<size-1; i++)
			for(int j = -thickness; j<size-1; j++)
				setPixel(v_it->x+j, v_it->y+i);
		}
	}

	void transform(Vertex &v)
	{
		v = (current_p * current_mv) * v;
		viewport.calculateWindowCoordinates(v);
	}

	float calculateRadiusScaleFactor()
	{
		Matrix m = current_p * current_mv;
		return sqrt((m.matrix[0] * m.matrix[5]) - (m.matrix[1] * m.matrix[4])) * viewport.calculateRatio();
	}
	void drawCricle(float x, float y, float z, float r)
	{

	//	sglBegin(SGL_POLYGON);
		//float diff = 0.15707963267f;

		//float scaleR = calculateRadiusScaleFactor();
					//axis_x *= scaleR;
					//axis_y *= scaleR;
		//r *= calculateRadiusScaleFactor();


//		for(int i = 0; i < 40; ++i)
	//	{
		//	sglVertex2f(x+(r * sin(i*diff)), y+(r * cos(i*diff)));
	//	}
	//	sglEnd();


		//fix
		Vertex v(x, y, 0.0f, 0.0f);
		transform(v);
		x = v.x; y = v.y; z = v.z;
		//calculate r scale factor
		r = r * calculateRadiusScaleFactor(); 
		int p	= 1 - (int)r;
			int x0	= int(x);
			int y0	= int(y);
			int x1	= 0;
			int y1	= int(r);
			int x2	= 1;
			int y2	= -2*r;

			//4 points around center
			setPixel(x0        , y0+int(r));
			setPixel(x0        , y0-int(r));
			setPixel(x0+int(r) , y0);
			setPixel(x0-int(r) , y0);

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

	void drawEllipse(float center_x, float center_y, float center_z, float axis_x, float axis_y)
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

			bool ellipse_adaptive = false;

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
				sglVertex2f( center_x + (axis_x * 0.1564344693575539	)	, center_y + ( axis_y * 0.987688339911341		));
				sglVertex2f( center_x + (axis_x * 0.3090170026893479	)	, center_y + ( axis_y * 0.9510565135936411		));
				sglVertex2f( center_x + (axis_x * 0.45399051142368685	)	, center_y + ( axis_y * 0.8910065182350011		));
				sglVertex2f( center_x + (axis_x * 0.587785266437776		)	, center_y + ( axis_y * 0.8090169840977831		));
				sglVertex2f( center_x + (axis_x * 0.7071067966408575	)	, center_y + ( axis_y * 0.7071067657322372		));
				sglVertex2f( center_x + (axis_x * 0.8090170097906934	)	, center_y + ( axis_y * 0.5877852310745185		));
				sglVertex2f( center_x + (axis_x * 0.891006511019614		)	, center_y + ( axis_y * 0.4539905255846804		));
				sglVertex2f( center_x + (axis_x * 0.9510565271012029	)	, center_y + ( axis_y * 0.3090169611173454		));
				sglVertex2f( center_x + (axis_x * 0.9876883560735247	)	, center_y + ( axis_y * 0.15643436731351018		));
				sglVertex2f( center_x + (axis_x * 0.999999999999999		)	, center_y - ( axis_y * 4.371139000186241E-8	));
				sglVertex2f( center_x + (axis_x * 0.9876883423975937	)	, center_y - ( axis_y * 0.15643445365997144		));
				sglVertex2f( center_x + (axis_x * 0.9510565000860774	)	, center_y - ( axis_y * 0.30901704426134974		));
				sglVertex2f( center_x + (axis_x * 0.8910064713304968	)	, center_y - ( axis_y * 0.4539906034789449		));
				sglVertex2f( center_x + (axis_x * 0.8090170284743339	)	, center_y - ( axis_y * 0.5877852053586913		));
				sglVertex2f( center_x + (axis_x * 0.7071067769704655	)	, center_y - ( axis_y * 0.7071067854026294		));
				sglVertex2f( center_x + (axis_x * 0.5877851957112599	)	, center_y - ( axis_y * 0.809017035483602		));
				sglVertex2f( center_x + (axis_x * 0.4539903804212881	)	, center_y - ( axis_y * 0.8910065849840472		));
				sglVertex2f( center_x + (axis_x * 0.3090168061705656	)	, center_y - ( axis_y * 0.9510565774464436		));
				sglVertex2f( center_x + (axis_x * 0.15643444188190603	)	, center_y - ( axis_y * 0.9876883442630557		));
				sglVertex2f( center_x - (axis_x * 8.742278000372475E-8	)	, center_y - ( axis_y * 0.9999999999999962		));
				sglVertex2f( center_x - (axis_x * 0.1564346145748253	)	, center_y - ( axis_y * 0.9876883169111731		));
				sglVertex2f( center_x - (axis_x * 0.3090169724585808	)	, center_y - ( axis_y * 0.9510565234162125		));
				sglVertex2f( center_x - (axis_x * 0.4539905362098265	)	, center_y - ( axis_y * 0.8910065056058313		));
				sglVertex2f( center_x - (axis_x * 0.5877853371642876	)	, center_y - ( axis_y * 0.8090169327119581		));
				sglVertex2f( center_x - (axis_x * 0.7071069006049366	)	, center_y - ( axis_y * 0.7071066617681382		));
				sglVertex2f( center_x - (axis_x * 0.8090171312459549	)	, center_y - ( axis_y * 0.5877850639056469		));
				sglVertex2f( center_x - (axis_x * 0.8910066589484567	)	, center_y - ( axis_y * 0.4539902352578838		));
				sglVertex2f( center_x - (axis_x * 0.951056480440929		)	, center_y - ( axis_y * 0.3090171047228822		));
				sglVertex2f( center_x - (axis_x * 0.9876883324525811	)	, center_y - ( axis_y * 0.156434516450301		));
				sglVertex2f( center_x - (axis_x * 0.9999999999999999	)	, center_y + ( axis_y * 1.1924880454806035E-8	));
				sglVertex2f( center_x - (axis_x * 0.9876883287216551	)	, center_y + ( axis_y * 0.15643454000643153		));
				sglVertex2f( center_x - (axis_x * 0.9510564730709448	)	, center_y + ( axis_y * 0.30901712740535175		));
				sglVertex2f( center_x - (axis_x * 0.8910064316413727	)	, center_y + ( axis_y * 0.45399068137320586		));
				sglVertex2f( center_x - (axis_x * 0.8090168369495607	)	, center_y + ( axis_y * 0.5877854689698682		));
				sglVertex2f( center_x - (axis_x * 0.7071065465657921	)	, center_y + ( axis_y * 0.7071070158072251		));
				sglVertex2f( center_x - (axis_x * 0.5877849321000184	)	, center_y + ( axis_y * 0.8090172270082862		));
				sglVertex2f( center_x - (axis_x * 0.45399051495953424	)	, center_y + ( axis_y * 0.8910065164333968		));
				sglVertex2f( center_x - (axis_x * 0.30901694977611		)	, center_y + ( axis_y * 0.9510565307861931		));
				sglVertex2f( center_x - (axis_x * 0.1564343555354446	)	, center_y + ( axis_y * 0.9876883579389858		));

			//	}

				sglEnd();
			}
		}

	void drawLineStrip()
	{
		for (int i = 0; i < (int)vertices.size()-1; i++)
			drawLine2D(vertices[i], vertices[i+1]);

		//for(unsigned int i = 0; i<vertexVector.size()-1; i++)
		  //          sglDrawLines(vertexVector[i], vertexVector[i+1]);

		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
	}

	void drawLineLoop()
	{
		for (int i = 0; i < (int)vertices.size()-1; i++)
		{
			drawLine2D(vertices[i], vertices[i+1]);

		}
		drawLine2D(vertices[(int)vertices.size()-1], vertices[0]);

		   //for(unsigned int i = 0; i<vertexVector.size()-1; i++){ //draw line strip, where one end point is start for second
		     //       sglDrawLines(vertexVector[i], vertexVector[i+1]);
		       // }
		        //sglDrawLines(vertexVector[vertexVector.size()-1], vertexVector[0]); //line loop where begin point must be connected with last

		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
	}

	void drawTriangles()
	{
		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
	//		setPixel(v_it->x, v_it->y);
	}

	void drawPolygon()
	{
		//if(SGL_LINE)
		//{
			for (int i = 0; i < (int)vertices.size()-1; i++)
			{
				drawLine2D(vertices[i], vertices[i+1]);
			}
		     //           sglDrawLines(vertexVector[i], vertexVector[i+1]);
		       //     }
			drawLine2D(vertices[(int)vertices.size()-1], vertices[0]);

//		            sglDrawLines(vertexVector[vertexVector.size()-1], vertexVector[0]);
		//}

		//if(currContext()->currentAreaMode==SGL_FILL)
		//{
			//sglDrawPolygon();
		//}
		//for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
			//setPixel(v_it->x, v_it->y);
	}

	void drawLines()
	{
		for (int i = 0; i < (int)vertices.size(); i += 2)
			drawLine2D(vertices[i], vertices[i+1]);
	}

	//Line
	//Breceanuv algoritmus
	//DDA algoritmus (jednoduzsi)
	//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
	void drawLine2D(Vertex a, Vertex b)
	{
		//bresenham(a.x,a.y,b.x,b.y);
		int dx = abs(b.x - a.x);
		int dy = abs(b.y - a.y);

		if (dx > dy)
			if (a.x < b.x)	bresenham_x(a.x, a.y, b.x, b.y);
			else			bresenham_x(b.x, b.y, a.x, a.y);
		else
			if (a.y < b.y)	bresenham_y(a.y, a.x, b.y, b.x);
			else			bresenham_y(b.y, b.x, a.y, a.x);
	}


	void drawArc2D(float x, float y, float z, float radius, float from, float to)
	{
		float step		= ((to - from) / 40);
		float circleX	= 0;
		float circleY	= 0;

		if (from >= to) return;

		sglBegin(SGL_LINE_STRIP);

		for (float fi = from; fi < to; fi += step)
		{
			circleX = x + radius * cos(fi);
			circleY = y + radius * sin(fi);
			sglVertex2f(circleX, circleY);
		}

		sglVertex2f(x + radius * cos(to), y + radius * sin(to));
		sglEnd();
	}

	void bresenham_x(int x1, int y1, int x2, int y2)
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
	{ return floor(number + 0.5); }

	void setPixel(int x, int y)
	{
		if (x >= 0 && x < w && y >= 0 && y < h)
		{
			int offset = (x+w*y)*3;

			buffer[offset]		= color.r;
			buffer[offset+1]	= color.g;
			buffer[offset+2]	= color.b;
		}
	}

	void setPixel(float x, float y)
	{ setPixel(int(x), int(y)); }

	int stackSize()
	{ return types.size(); }

	void multiplyCurrentMatrix(Matrix & m)
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= current_mv * m;
		else								current_p   = current_p  * m;
	}

	void setViewport(int width, int height, int x, int y)
	{ viewport.changeViewport(width, height, x, y); }


	Matrix & getCurrentMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)
			return current_mv;
	       	return current_p;
	}

	void pushMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)	transformStack.push_back(current_mv);
		else 								transformStack.push_back(current_p);
	}

	void setCurrentMatrix(Matrix matrix)
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= matrix;
		else					current_p	= matrix;
	}

	void setMatrixMode(sglEMatrixMode mode)
	{ matrixMode = mode; }

	bool invalidTypeStack()
	{ return (types.size() > 0); }

	bool stackEmpty()
	{ return (transformStack.size() == 0); }

	void popMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)	current_mv	= transformStack.back();
		else								current_p	= transformStack.back();

		transformStack.pop_back();
	}

	void pushTypeState(sglEElementType type)
	{ types.push_back(type); }

	void clearBuffer(unsigned what)
	{
		int size = w * h * 3;

		for(int i = 0; i<size; i += 3)
		{
			buffer[i]	= clear.r;
			buffer[i+1]	= clear.g;
			buffer[i+2]	= clear.b;
		}
		return;
	}
};

//Context Manager
struct ContextManager
{
    std::vector<Context*> contexts;
    int current;
};

#endif /* DATA_H_ */
