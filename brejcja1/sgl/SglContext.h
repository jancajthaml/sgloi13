#ifndef SGLCONTEXT_H 
#define SGLCONTEXT_H

#include "sgl.h"
#include "Color.h"
#include "Vertex.h"
#include "Viewport.h"
#include "Matrix4.h"
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
	
	Matrix4 currentModelviewMatrix;
	Matrix4 currentProjectionMatrix;	
	std::vector<Matrix4> projectionStack;
	std::vector<Matrix4> modelviewStack;
	sglEMatrixMode matrixMode;

	std::vector<Vertex> vertices;	
	bool depthTest;

	Viewport viewport;

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
	SGLContext(){};
	SGLContext(int width, int height)
	{
		this->width = width;
		this->height = height;
		framebuffer = (Color *)malloc(sizeof(float) * width * height);
		if (!framebuffer)
			throw std::exception();
		clearColor = Color(0.0f, 0.0f, 0.0f);
		color = Color(0.0f, 0.0f, 0.0f);
		depthTest = false;
		currentModelviewMatrix = Matrix4::makeIdentity();
		currentProjectionMatrix = Matrix4::makeIdentity();
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
		}
		vertices.clear();
	}

	void drawPoints2D()
	{
		for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
		{
			setPixel(v_it->v[0], v_it->v[1]);		
		}			
	}

	void drawLines2D()
	{
		for (int i = 0; i < (int)vertices.size(); i += 2)
		{
			drawLine2D(vertices[i], vertices[i+1]);		
		}

	}

	void drawLine2D(Vertex a, Vertex b)
	{
		//obtain the points
		int x1, x2, y1, y2;
		x1 = (int)round(a.v[0]);
		y1 = (int)round(a.v[1]);
		x2 = (int)round(b.v[0]);
		y2 = (int)round(b.v[1]);
	
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

	void setPixel(float x, float y)
	{
		setPixel((int)round(x), (int)round(y));				
	}

	void setPixel(int x, int y)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			framebuffer[x + width * y] = color;				
		}

	}



	float round(float number)
	{
		return floor(number + 0.5);
	}

	void setVertex2f(float x, float y)
	{
		Vertex v(x, y, 0.0f, 0.0f);
		v = currentProjectionMatrix * currentModelviewMatrix * v;	
		//currentProjectionMatrix.print();
		//currentModelviewMatrix.print();
		if (viewport.isReadyToUse())
		{
			viewport.calculateWindowCoordinates(v);
			//v.print();
		}
		else
		{
			v.v[0] = x;
			v.v[1] = y;
		}
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
			getCurrentMatrixStack().push_back(currentModelviewMatrix);
		else 
			getCurrentMatrixStack().push_back(currentProjectionMatrix);

	}

	void popMatrix(sglEErrorCode * err)
	{
		std::vector<Matrix4> matrixStack = getCurrentMatrixStack();
		if (matrixStack.size() == 0)
		{
			//TODO ask course teacher why in the docs is written
			//that underflow should be generated in case one matrix is on the stack
			//Now the error is generated when 0 matrices are on the stack - I think it is correct that way.
			*err = SGL_STACK_UNDERFLOW;
		       	return;
		}	
		if (matrixMode = SGL_MODELVIEW)
			currentModelviewMatrix = matrixStack.back();
		else
			currentProjectionMatrix = matrixStack.back();
		matrixStack.pop_back();
	}
		

	std::vector<Matrix4> & getCurrentMatrixStack()
	{
		if (matrixMode == SGL_MODELVIEW)
			return modelviewStack;
		else if(matrixMode == SGL_PROJECTION)
			return projectionStack;
		else throw std::exception();
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
			currentModelviewMatrix = matrix;
		else currentProjectionMatrix = matrix;
	}

	void multiplyCurrentMatrix(Matrix4 & m, sglEErrorCode * err)
	{
		beginEndCheck(err);
		if (matrixMode == SGL_MODELVIEW)
			currentModelviewMatrix = currentModelviewMatrix * m;
		else
			currentProjectionMatrix = currentProjectionMatrix * m;
	}
};


#endif
