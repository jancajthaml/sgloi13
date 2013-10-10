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
#include <stdio.h>
#include <stdint.h>
#include "sgl.h"
#include "Vertex.h"
#include "VertexStack.h"
#include "MatrixCache.h"
#include "Matrix.h"
#include "Viewport.h"
#include "Color.h"
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

typedef struct { float f1; float f2; float f3;} __attribute__((packed)) __color;

//Context
struct Context
{
	int_fast8_t id;	//Maximum 256 contexts


    //Drawing
	Color *buffer;
	Color* clear;

	int_fast16_t w;	//maximum 65536
	int_fast16_t h;	//maximum 65536



	//Color clear;
	Color color;

	//State
	bool depth;

	//Pixel size
	int_fast8_t size;	// maximum 256

	//Transformation matrices

	Matrix currentModelviewMatrix;
	Matrix currentProjectionMatrix;
	std::vector<Matrix> projectionStack;

	VertexStack vertices;
	Matrix PMMatrix;
	Matrix PMVMatrix;
	bool projMatChanged;
	bool modelMatChanged;

	std::vector<sglEElementType> types;
	Viewport viewport;
	sglEMatrixMode matrixMode;

	uint_fast32_t lastSetPixelIndex;	//	must be 32bit (16bit is too small)
	uint_fast32_t w_h;					//	must be 32bit (16bit is too small)

	Context(uint_fast16_t width, uint_fast16_t height)
	{
		//----------------------//

		//Initialise Drawing
		w			= width;
		h			= height;
		id			= 0;
		w_h			= width * height;
		//matrixMode	= NULL;

		// ? this shoud be static or const equivalent to NULL ?
		//clear	= Color(255,0,0);

		// ? this shoud be static or const equivalent to NULL ?
		color	= Color(0,255,0);

		//----------------------//

		//Initialise Flags
		depth	= false;

		//----------------------//

		size	= 1;

		//this->width = width;
		//this->height = height;
		buffer	= (Color*) malloc(sizeof(Color) * w_h);
		clear	= (Color*) malloc(sizeof(Color) * w_h);

		if (!buffer) throw std::exception();

		currentModelviewMatrix	= MatrixCache::identity();
		currentProjectionMatrix	= MatrixCache::identity();

		projMatChanged	= true;
		modelMatChanged	= true;
	}

	inline void setVertex2f(float x, float y)
	{
		Vertex v(x, y, 0.0f, 1.0f);
		transform(v);
		vertices.push_back(v);
	}

	inline void draw()
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

		vertices.index = 0;
	}

	inline void drawPoints()
	{
		int_fast32_t s = int_fast32_t(vertices.index);

		if(size==1)
		{
			for (int_fast32_t i = 0; i < s; i++)
				setPixel((vertices)[i].x, (vertices)[i].y);
		}
		else
		{
			int_fast8_t thickness = size;

			if((thickness%2)==0) thickness++;

			thickness >>= 1;

			for (int_fast32_t i = 0; i < s; i++)
			{
				Vertex v = (vertices)[i];
				for(int_fast8_t i = -thickness; i<size-1; i++)
				for(int_fast8_t j = -thickness; j<size-1; j++)
					setPixel(v.x+j, v.y+i);
			}
		}
	}

	inline void transform(Vertex & v)
	{
		checkPMVMatrix();
		v = PMVMatrix * v;
	}

	inline float calculateRadiusScaleFactor()
	{
		checkPMVMatrix();
		return sqrt((PMVMatrix.matrix[0] * PMVMatrix.matrix[5]) - (PMVMatrix.matrix[1] * PMVMatrix.matrix[4]));
	}


	inline void bresenham_circle(int_fast32_t xs, int_fast32_t ys, int_fast32_t r)
	{
		int_fast32_t x, y, p;
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

	inline void drawCricle(float x, float y, float z, float r)
	{
		Vertex v(x, y, 0.0f, 1.0f);
		transform(v);
		bresenham_circle(int_fast32_t(v.x), (int_fast32_t)(v.y), int_fast32_t(r * calculateRadiusScaleFactor()));
	}

	inline void drawEllipse(float center_x, float center_y, float center_z, float axis_x, float axis_y)
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

				//Translate center_x,center_y

				//float diff = 0.15707963267f;

				//for(int_fast32_t i = 0; i < 40; ++i)
				//{
				//sglVertex2f(center_x+(axis_x * sin(i*diff)), center_y+(axis_y * cos(i*diff)));
				//}


				// //Translate -center_x,-center_y
				//TRANSLATE is slower than calculation

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

	inline void drawLineStrip()
	{
		uint_fast16_t size = uint_fast16_t(vertices.index-1);

		for (uint_fast16_t i = 0; i < size; i++)
			drawLine2D(vertices[i], vertices[i+1]);
	}

	void drawLineLoop()
	{
		uint_fast16_t size = uint_fast16_t(vertices.index-1);

		for (uint_fast16_t i = 0; i < size; i++)
			drawLine2D(vertices[i], vertices[i+1]);

		drawLine2D(vertices[size], vertices[0]);
	}

	inline void drawTriangles()
	{
	}

	inline void drawPolygon()
	{
		uint_fast32_t size = uint_fast32_t(vertices.index-1);

		for (uint_fast32_t i = 0; i < size; i++)
			drawLine2D(vertices[i], vertices[i+1]);

		drawLine2D(vertices[size], vertices[0]);
	}

	inline void drawLines()
	{
		uint_fast32_t size = uint_fast16_t(vertices.index-1);

		for (uint_fast32_t i = 0; i < size; i += 2)
			drawLine2D(vertices[i], vertices[i+1]);
	}

	//Line
	//Breceanuv algoritmus
	//DDA algoritmus (jednoduzsi)
	//@see https://www.google.cz/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDwQFjAB&url=http%3A%2F%2Fwww.cs.toronto.edu%2F~smalik%2F418%2Ftutorial2_bresenham.pdf&ei=m9ZJUselBqTm7AbmpICgAg&usg=AFQjCNF6Bfg6OxtgTUATu1aTlDUmTy0aYw&bvm=bv.53217764,d.ZGU
	inline void drawLine2D(Vertex a, Vertex b)
	{
		//bresenham(a.x,a.y,b.x,b.y);
		int_fast32_t dx = abs(b.x - a.x);
		int_fast32_t dy = abs(b.y - a.y);

		if (dx > dy)
			if (a.x < b.x)	bresenham_x(a.x, a.y, b.x, b.y);
			else			bresenham_x(b.x, b.y, a.x, a.y);
		else
			if (a.y < b.y)	bresenham_y(a.y, a.x, b.y, b.x);
			else			bresenham_y(b.y, b.x, a.y, a.x);
	}


	inline void drawArc2D(float x, float y, float z, float r, float from, float to)
	{
		float x2, y2;
		float N		= 40 * (to - from)/6.283185307179586f;
		float alpha	= (to - from) / N;

		pushTypeState(SGL_LINE_STRIP);

		int_fast32_t offset = from / alpha;
		float fromOffset	= (offset - 1)*alpha;
		float x1			= r * cosf(fromOffset);
		float y1			= r * sinf(fromOffset);
		float sa			= sinf(alpha);
		float ca			= cosf(alpha);

		int_fast32_t RR = __round(N)+offset;

		for (int_fast32_t i = offset; i <= RR; i++)
		{
			x2 = ca * x1 - sa * y1;
			y2 = sa * x1 + ca * y1;
			setVertex2f(x2 + x, y2 + y);
			x1 = x2;
			y1 = y2;
		}
		draw();
	}

	inline void bresenham_x(signed x1, signed y1, signed x2, signed y2)
	{
		signed dx = x2 - x1;
		signed dy = y2 - y1;
		signed sign = 1;
		if (dy < 0) sign = -1;
		signed c0 = (dy << 1) * sign;
		signed c1 = c0 - (dx << 1);
		signed p = c0 - dx;

		setPixel(x1, y1);
		for (signed i = x1 + 1; i <= x2; ++i)
		{
			if (p < 0)
			{
				p += c0;
				setPixel_x();
			}
			else
			{
				p += c1;
				if (sign > 0)
					setPixel_xy();
				else
					setPixel_xmy();
			}
		}
	}

	inline void bresenham_y(signed x1, signed y1, signed x2, signed y2)
	{
		signed dx = x2 - x1;
		signed dy = y2 - y1;
		signed sign = 1;
		if (dy < 0)
			sign = -1;
		signed c0, c1, p;
		c0 = (dy << 1) * sign;
		c1 = c0 - (dx << 1);
		p = c0 - dx;

		setPixel(y1, x1);
		for (signed i = x1 + 1; i <= x2; ++i)
		{
			if (p < 0)
			{
				p += c0;
				setPixel_y();
			}
			else
			{
				p += c1;
				if (sign > 0)
					setPixel_xy();
				else
					setPixel_mxy();
			}
		}
	}

	inline void setPixel(signed x, signed y)
	{
		//Condition not needed for now
	//	if (x >= 0 && x < w && y >= 0 && y < h)
		//{
			lastSetPixelIndex = (x + w * y);
			*((__color*) (buffer + lastSetPixelIndex))	= *((__color*) &(color));
		//}
	}

	inline void setPixel_x()
	{
		lastSetPixelIndex += 1;

		//Safety off
		//if (lastSetPixelIndex < w_h)
			*((__color*) (buffer + lastSetPixelIndex))	= *((__color*) &(color));
	}

	inline void setPixel_y()
	{
		lastSetPixelIndex += w;

		//Safety off
//		if (lastSetPixelIndex < w_h)
			*((__color*) (buffer + lastSetPixelIndex))	= *((__color*) &(color));
	}

	inline void setPixel_xy()
	{
		lastSetPixelIndex += (w + 1);

		//Safety off
		//if (lastSetPixelIndex < w_h)
			*((__color*) (buffer + lastSetPixelIndex))	= *((__color*) &(color));
	}

	inline void setPixel_mxy()
	{
		lastSetPixelIndex += (w - 1);

		//Safety off
		//if (lastSetPixelIndex < w_h)
			*((__color*) (buffer + lastSetPixelIndex))		= *((__color*) &(color));
	}

	inline void setPixel_xmy()
	{
		lastSetPixelIndex += (1 - w);
		//if (lastSetPixelIndex < w_h)
			*((__color*) (buffer + lastSetPixelIndex))	= *((__color*) &(color));
	}

	inline void setSymPixel(signed x, signed y, signed xs, signed ys)
	{
		signed rx = x + xs;
		signed ry = y + ys;
		signed mrx = -x + xs;
		signed mry = -y + ys;
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

	inline int_fast16_t stackSize()
	{ return types.size(); }

	inline void multiplyCurrentMatrix(Matrix & m)
	{

		switch(matrixMode)
		{
			case SGL_MODELVIEW :
				modelMatChanged			= true;
				currentModelviewMatrix	= currentModelviewMatrix * m;
			break;

			default:
				projMatChanged			= true;
				currentProjectionMatrix	= currentProjectionMatrix * m;
			break;
		}
	}

	inline void setViewport(signed width, signed height, signed x, signed y)
	{ viewport.changeViewport(width, height, x, y); }

	inline Matrix & getCurrentMatrix()
	{
		switch(matrixMode)
		{
			case SGL_MODELVIEW	: return currentModelviewMatrix;
			default				: return currentProjectionMatrix;
		}
	}

	inline void pushMatrix()
	{
		switch(matrixMode)
		{
			case SGL_MODELVIEW	: projectionStack.push_back(currentModelviewMatrix); break;
			default				: projectionStack.push_back(currentProjectionMatrix); break;
		}
	}

	inline void setCurrentMatrix(Matrix matrix)
	{
		switch(matrixMode)
		{
			case SGL_MODELVIEW	:
				modelMatChanged			= true;
				currentModelviewMatrix	= matrix;
			break;

			default				:
				projMatChanged			= true;
				currentProjectionMatrix	= matrix;
			break;
		}
	}

	inline void setMatrixMode(sglEMatrixMode mode)
	{ matrixMode = mode; }

	inline bool invalidTypeStack()
	{ return (types.size() > 0); }

	inline bool stackEmpty()
	{ return (projectionStack.size() == 0); }

	inline void popMatrix()
	{
		if (projectionStack.size() == 0)
		{
			//*err = SGL_STACK_UNDERFLOW;
		       	return;
		}

		switch(matrixMode)
		{
			case SGL_MODELVIEW	:
				modelMatChanged = true;
				currentModelviewMatrix = projectionStack.back();
			break;

			default				:
				projMatChanged = true;
				currentProjectionMatrix = projectionStack.back();
			break;
		}

		projectionStack.pop_back();
	}

	inline void pushTypeState(sglEElementType type)
	{ types.push_back(type); }

	inline void clearBuffer(unsigned what)
	{
		memcpy(buffer, clear, w_h);
		return;
	}

	inline bool checkPMMatrix()
	{
		if (modelMatChanged || projMatChanged)
		{
			modelMatChanged	= false;
			projMatChanged	= false;
			PMMatrix		= currentProjectionMatrix * currentModelviewMatrix;
			return true;
		}
		return false;
	}

	inline void checkPMVMatrix()
	{
		if (checkPMMatrix() || viewport.viewportMatrixChanged)
		{
			viewport.viewportMatrixChanged = false;
			PMVMatrix = viewport.viewportMatrix * PMMatrix;
		}
	}


	inline void cacheClear(float r, float g, float b, float a)
	{
		Color c = Color(r,g,b);
		//1-6 = RGB RGB
		*((__color*) (clear))	= *((__color*) &c);	//1-3 RGB
		*((__color*) (clear+2))	= *((__color*) &c);	//4-7 RGB

		uint_fast32_t l = 2;
		int_fast8_t s = sizeof(Color);

		for(uint_fast32_t offset=l ; offset < w_h; offset <<= 1)
		{
			memcpy(&clear[offset], &clear[l], s);
			l=offset;
		}
	}

	inline int_fast32_t __round(float x)
	{ return ((x>=0.5f)?(int_fast32_t(x)+1):int_fast32_t(x)); }
};


#endif /* DATA_H_ */
