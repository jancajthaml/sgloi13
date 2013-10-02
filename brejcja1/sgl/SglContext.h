#ifndef SGLCONTEXT_H 
#define SGLCONTEXT_H

#include "sgl.h"
#include "Color.h"
#include "Vertex.h"
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

	std::vector<Vertex> vertices;	
	bool depthTest;
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
			}
		}
		vertices.clear();
	}

	void drawPoints2D()
	{
		for (std::vector<Vertex>::iterator v_it = vertices.begin(); v_it != vertices.end(); ++v_it)
		{
			setPixel(v_it->x, v_it->y);		
		}			
	}

	void setPixel(float x, float y)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			framebuffer[(int)round(x) + width * (int)round(y)] = color;				
		}

	}

	float round(float number)
	{
		return floor(number + 0.5);
	}

	void setVertex2f(float x, float y)
	{
		vertices.push_back(Vertex(x, y));
	}	

};


#endif
