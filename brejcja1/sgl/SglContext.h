#ifndef SGLCONTEXT_H
#define SGLCONTEXT_H

#include "sgl.h"
#include "Color.h"
#include "Vertex.h"
#include "Viewport.h"
#include "Matrix4.h"
#include "VertexStack.h"
#include <cstdlib>
#include <exception>
#include <cstdio>
#include <vector>
#include <cmath>

class SGLContext
{
private:
	int width;
	int height;
	
	Color *framebuffer;
	Color clearColor;
	Color color;
	std::vector<sglEElementType> typeStack;
	Matrix4 PMMatrix;
	Matrix4 PMVMatrix;
	bool projMatChanged;
	bool modelMatChanged;	
	Matrix4 currentModelviewMatrix;
	Matrix4 currentProjectionMatrix;	
	std::vector<Matrix4> projectionStack;
	sglEMatrixMode matrixMode;

	unsigned int pointThickness;
	VertexStack vertices;	
	bool depthTest;

	Viewport viewport;

	int lastSetPixelIndex;
	int w_h;

	float ellipseCos[40];
	float ellipseSin[40];

	bool beginEndCheck(sglEErrorCode * err)
	{
		if (typeStack.size() > 0)
		{
			*err = SGL_INVALID_OPERATION;
			return false;
		}
		return true;
	}
public:
	SGLContext()
	{
		projMatChanged = true;
		modelMatChanged = true;
		lastSetPixelIndex = 0;
		w_h = 0;
		prepareEllipseData();
		//vertices = new VertexStack;
	}
	SGLContext(int width, int height)
	{
		this->width = width;
		this->height = height;
		w_h = width * height;
		framebuffer = (Color *)malloc(sizeof(Color) * width * height);
		if (!framebuffer)
			throw std::exception();
		clearColor = Color(0.0f, 0.0f, 0.0f);
		color = Color(0.0f, 0.0f, 0.0f);
		depthTest = false;
		currentModelviewMatrix = Matrix4::makeIdentity();
		currentProjectionMatrix = Matrix4::makeIdentity();
		pointThickness = 1;
		projMatChanged = true;
		modelMatChanged = true;
		lastSetPixelIndex = 0;
		prepareEllipseData();
		//vertices = new VertexStack;
	}
	/*
	~SGLContext()
	{
		delete vertices;
	}

	SGLContext(const SGLContext& other)
	{

		printf("cc\n");
		
		this->width = other.width;
		this->height = other.height;
		w_h = width * height;
		framebuffer = (Color *)malloc(sizeof(Color) * width * height);
		if (!framebuffer)
			throw std::exception();
		memcpy(framebuffer, other.framebuffer, sizeof(Color) * width * height);
		clearColor = other.clearColor;
		color = other.color;
		depthTest = other.depthTest;
		currentModelviewMatrix = other.currentModelviewMatrix;
		currentProjectionMatrix = other.currentProjectionMatrix;
		pointThickness = other.pointThickness;
		projMatChanged = other.projMatChanged;
		modelMatChanged = other.modelMatChanged;
		lastSetPixelIndex = other.lastSetPixelIndex;
		prepareEllipseData();
		this->vertices = new VertexStack(*(other.vertices)); 
	}

	SGLContext& operator=(const SGLContext& other)
	{
		this->width = other.width;
		this->height = other.height;
		w_h = width * height;
		framebuffer = (Color *)malloc(sizeof(Color) * width * height);
		if (!framebuffer)
			throw std::exception();
		memcpy(framebuffer, other.framebuffer, sizeof(Color) * width * height);
		clearColor = other.clearColor;
		color = other.color;
		depthTest = other.depthTest;
		currentModelviewMatrix = other.currentModelviewMatrix;
		currentProjectionMatrix = other.currentProjectionMatrix;
		pointThickness = other.pointThickness;
		projMatChanged = other.projMatChanged;
		modelMatChanged = other.modelMatChanged;
		lastSetPixelIndex = other.lastSetPixelIndex;
		prepareEllipseData();
		this->vertices = new VertexStack; 
		return *this;
	}*/	

	bool beginEndCheck()
	{
		return typeStack.size() == 0 ? true : false;
	}

	void setPointSize(float t)
	{
		int thickness = (int)floor(t);
		if (thickness % 2 == 1)
			--thickness;
		if (thickness < 1)
			thickness = 1;
		pointThickness = thickness;
	}

	void clear(unsigned what, sglEErrorCode * err)
	{
		if (typeStack.size() > 0)
		{
			*err = SGL_INVALID_OPERATION;
			return;
		}
		if (((what & SGL_COLOR_BUFFER_BIT) != SGL_COLOR_BUFFER_BIT) && ((what & SGL_DEPTH_BUFFER_BIT) != SGL_DEPTH_BUFFER_BIT))
		{
			*err = SGL_INVALID_VALUE;
			return;
		}
		if ((what & SGL_COLOR_BUFFER_BIT) == SGL_COLOR_BUFFER_BIT)
		{
			for (int i = 0; i < width * height; i++)
				framebuffer[i] = clearColor;
		}
		if ((what & SGL_DEPTH_BUFFER_BIT) == SGL_DEPTH_BUFFER_BIT)
		{
			//TODO clear with depth buffer bit
		}
		*err = SGL_NO_ERROR;
	return;
	}

	Color * getFramebuffer()
	{
		return framebuffer;
	}

	void setClearColor(float r, float g, float b)
	{
		this->clearColor.r = r;
		this->clearColor.g = g;
		this->clearColor.b = b;
	}

	Color & getClearColor()
	{
		return clearColor;
	}

	void setColor(Color c)
	{
		color = c;
	}

	void print()
	{
		printf("Context width %d, height %d\n", width, height);
	}

	void pushTypeState(sglEElementType type)
	{
		typeStack.push_back(type);
	}

	void draw()
	{
		//test of parenthesses of sglBegin and sglEnd is correct
		if (typeStack.size() == 0)
			throw std::exception();
		//pop last element
		sglEElementType type = typeStack.back();
		typeStack.pop_back();

		switch(type)
		{
			case SGL_POINTS:
			{
				if (!depthTest)
					drawPoints2D();
				break;
			}
			case SGL_LINES:
			{
				if (!depthTest)
					drawLines2D();
				break;
			}
			case SGL_LINE_STRIP:
			{
				if (!depthTest)
					drawLineStrip2D();
				break;
			}
			case SGL_LINE_LOOP:
			{
				if (!depthTest)
					drawLineLoop2D();
				break;
			}
		}
		vertices.clear();
	}

	void drawLineStrip2D()
	{
		for (int i = 0; i < (int)vertices.size() - 1; i++)
		{
			drawLine2D((vertices)[i], (vertices)[i+1]);
		}
	}

	void drawLineLoop2D()
	{
		drawLineStrip2D();
		drawLine2D((vertices)[vertices.size() - 1], (vertices)[0]);
	}

	void drawPoints2D()
	{
		for (int i = 0; i < vertices.size(); ++i)
		{
			float x = (vertices)[i].v[0];
			float y = (vertices)[i].v[1];
			int offset = pointThickness >> 1;
			for (int i = -offset; i <= offset; ++i)
			{	
				for (int j = -offset; j <= offset; ++j)
				{
					setPixel(x + i, y + j);
				}
			}
		}		

	}

	void drawLines2D()
	{
		for (int i = 0; i < (int)vertices.size(); i += 2)
		{
			drawLine2D((vertices)[i], (vertices)[i+1]);		
		}

	}

	void drawLine2D(Vertex a, Vertex b)
	{
		//obtain the points
		int x1, x2, y1, y2;
		x1 = (int)a.v[0];
		y1 = (int)a.v[1];
		x2 = (int)b.v[0];
		y2 = (int)b.v[1];
		
		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
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

	bool checkPMMatrix()
	{
		if (modelMatChanged || projMatChanged)
		{
			modelMatChanged = false;
			projMatChanged = false;
			PMMatrix = currentProjectionMatrix * currentModelviewMatrix;
			return true;
		}
		return false;
	}

	void checkPMVMatrix()
	{
		bool change = checkPMMatrix();
		bool change2 = viewport.viewportMatrixChanged;
		if (change || change2)
		{
			viewport.viewportMatrixChanged = false;
			PMVMatrix = viewport.getViewportMatrix() * PMMatrix;
		}
	}

	void transform(Vertex & v)
	{
		checkPMVMatrix();
		v = PMVMatrix * v;
	       	//v = viewport.getViewportMatrix() * v;	
		//viewport.calculateWindowCoordinates(v);
	}

	float calculateRadiusScaleFactor()
	{
		checkPMVMatrix(); 
		return sqrt((PMVMatrix.m[0] * PMVMatrix.m[5]) - (PMVMatrix.m[1] * PMVMatrix.m[4]));
	}

	void drawArc(float x, float y, float z, float r, float from, float to)
	{
		float x2, y2;
		float N = 40 * (to - from)/(2 * M_PI);
		float alpha = (to - from) / N;
		pushTypeState(SGL_LINE_STRIP);
		float offset = from / alpha;
		int fromOffset = (int)floor(offset) - 1;
		float x1 = r * cos(fromOffset * alpha);
		float y1 = r * sin(fromOffset * alpha);
		float sa = sin(alpha);
		float ca = cos(alpha);
		for (int i = (int)floor(offset); i <= (int)round(N) + (int)round(offset); i++)
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

		//drawArc(x, y, z, r, r, from, to);
	}
/*
	void drawArc(float x, float y, float z, float a, float b, float from, float to)
	{
		float x1 = a;
		float y1 = 0;
		float x2, y2;
		float N = 40 * (to - from)/(2 * M_PI);
		float alpha = (to - from) / N;
		pushTypeState(SGL_LINE_STRIP);
		float offset = from / alpha;
		//setVertex2f(x1 + x, y1 + y);
		for (int i = (int)floor(offset); i <= (int)round(N) + (int)round(offset); i++)
		{
			float ialpha = i * alpha; 
			x2 = a * cos(ialpha);
			y2 = b * sin(ialpha);
			setVertex2f(x2 + x, y2 + y);
			x1 = x2;
			y1 = y2;
		}
		draw();
	}*/

	void drawCircle(float x, float y, float r)
	{
		Vertex v(x, y, 0.0f, 1.0f);
		transform(v);
		bresenham_circle((int)floor(v.v[0]), (int)floor(v.v[1]), (int)floor(r * calculateRadiusScaleFactor()));
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

	void prepareEllipseData()
	{
		int N = 40;
		float alpha = (2 * M_PI) / N;

		for (int i = 0; i < N; i++)
		{
			float ialpha = i * alpha;
			ellipseCos[i] = cos(ialpha);
			ellipseSin[i] = sin(ialpha);
		}
	}

	void drawEllipse(float x, float y, float z, float a, float b)
	{
		float x1 = a;
		float y1 = 0;
		float x2, y2;
		int N = 40;
		float alpha = (2 * M_PI) / N;
		pushTypeState(SGL_LINE_LOOP);
	       	for (int i = 0; i < N; i++)
		{
			x2 = a * ellipseCos[i];
			y2 = b * ellipseSin[i];
			setVertex2f(x1 + x, y1 + y);
			setVertex2f(x2 + x, y2 + y);
			x1 = x2;
			y1 = y2;
		}
		draw();
	}

	void setPixel(float x, float y)
	{
		setPixel((int)floor(x), (int)floor(y));				
	}

	void setPixel(int x, int y)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			lastSetPixelIndex = x + width * y;
			framebuffer[lastSetPixelIndex] = color;				
		}

	}

	void setPixel_x()
	{
		lastSetPixelIndex += 1;
		if (lastSetPixelIndex < w_h)
			framebuffer[lastSetPixelIndex] = color;
	}

	void setPixel_y()
	{
		lastSetPixelIndex += width;
		if (lastSetPixelIndex < w_h)
			framebuffer[lastSetPixelIndex] = color;
	}

	void setPixel_xy()
	{
		lastSetPixelIndex += width + 1;
		if (lastSetPixelIndex < w_h)
			framebuffer[lastSetPixelIndex] = color;
	}

	void setPixel_mxy()
	{
		lastSetPixelIndex += width - 1;
		if (lastSetPixelIndex < w_h)
			framebuffer[lastSetPixelIndex] = color;
	}

	void setPixel_xmy()
	{
		lastSetPixelIndex += 1 - width;
		if (lastSetPixelIndex < w_h)
			framebuffer[lastSetPixelIndex] = color;
	}


	float round(float number)
	{
		return floor(number + 0.5);
	}

	void setVertex2f(float x, float y)
	{
		Vertex v(x, y, 0.0f, 1.0f);
		transform(v);
		vertices.push_back(v);
	}	

	void setViewport(int width, int height, int x, int y, sglEErrorCode * err)
	{
		if (!beginEndCheck(err))
			return;
		viewport.changeViewport(width, height, x, y);
	}

	void pushMatrix(sglEErrorCode * err)
	{
		if (!beginEndCheck(err))
			return;
		if (matrixMode == SGL_MODELVIEW)
			projectionStack.push_back(currentModelviewMatrix);
		else 
			projectionStack.push_back(currentProjectionMatrix);

	}

	void popMatrix(sglEErrorCode * err)
	{
		if (projectionStack.size() == 0)
		{
			*err = SGL_STACK_UNDERFLOW;
		       	return;
		}	
		if (matrixMode == SGL_MODELVIEW)
		{
			modelMatChanged = true;
			currentModelviewMatrix = projectionStack.back();
		}
		else
		{
			projMatChanged = true;
			currentProjectionMatrix = projectionStack.back();
		}
		projectionStack.pop_back();
	}
		

	void setMatrixMode(sglEMatrixMode mode, sglEErrorCode * err)
	{	
		beginEndCheck(err);
		if ((mode == SGL_MODELVIEW) || (mode == SGL_PROJECTION))
			matrixMode = mode;
		else *err = SGL_INVALID_ENUM;
	}	

	Matrix4 & getCurrentMatrix()
	{
		if (matrixMode == SGL_MODELVIEW)
			return currentModelviewMatrix;
		else return currentProjectionMatrix;
	}


	void setCurrentMatrix(Matrix4 matrix, sglEErrorCode * err)
	{
		beginEndCheck(err);
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

	void multiplyCurrentMatrix(Matrix4 & m, sglEErrorCode * err)
	{
		beginEndCheck(err);
		
		if (matrixMode == SGL_MODELVIEW)
		{
			modelMatChanged = true;
			currentModelviewMatrix = currentModelviewMatrix * m;
		}
		else
		{
			projMatChanged = true;
			currentProjectionMatrix = currentProjectionMatrix * m;
		}
	}
};


#endif
