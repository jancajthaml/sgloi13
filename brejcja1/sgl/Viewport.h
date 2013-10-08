#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Matrix4.h"
#include "Vertex.h"
#include <cmath>
class Viewport
{
private:
	int width;
	int height;
	float width_2_x;
	float height_2_y;
	int x;
	int y;
	bool readyToUse;
	Matrix4 viewportMatrix;
public:
	bool viewportMatrixChanged;
	Viewport()
	{
		width = 0;
		height = 0;
		width_2_x = 0.0f;
		height_2_y = 0.0f;
		x = 0;
		y = 0;
		readyToUse = false;
		viewportMatrixChanged = true;
	}

	Viewport(int width, int height, int x, int y)
	{
		changeViewport(width, height, x, y);
	}

	Matrix4 getViewportMatrix()
	{
		return viewportMatrix;
	}

	float calculateRatio()
	{
		return sqrt(width * width + height * height) / sqrt(8);
	}

	void calculateWindowCoordinates(Vertex & v)
	{
		if (readyToUse)
		{
			v.v[0] = (v.v[0] + 1) * width_2_x;
			v.v[1] = (v.v[1] + 1) * height_2_y;
		}
	}	

	void changeViewport(int width, int height, int x, int y)
	{
		this->width = width;
		this->height = height;
		this->width_2_x = ((float)width/2) + x;
		this->height_2_y = ((float)height/2) + y;
		this->x = x;
		this->y = y;
		viewportMatrix = Matrix4(width/2, 0.0f, 0.0f, 0.0f, 0.0f, height/2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, width/2 + x, height/2 + x, 0.0f, 1.0f);
		readyToUse = true;
		viewportMatrixChanged = true;
	}

	int getWidth() { return width; }

	int getHeight() {return height; }

	int getX() { return x; }

	int getY() {return y; }

	bool isReadyToUse() { return readyToUse; }
};


#endif
