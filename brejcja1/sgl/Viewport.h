/*
 * Viewport.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_


struct Viewport
{

	int width;
	int height;
	float width_2_x;
	float height_2_y;
	int x;
	int y;
	bool ready;

	Matrix viewportMatrix;
	bool viewportMatrixChanged;

	Viewport()
	{
		width = 0;
		height = 0;
		width_2_x = 0.0f;
		height_2_y = 0.0f;
		x = 0;
		y = 0;
		ready = false;
		viewportMatrixChanged = true;
	}

	float calculateRatio()
	{
		return sqrt(width * width + height * height) / sqrt(8);
	}

	Viewport(int width, int height, int x, int y)
	{
		changeViewport(width, height, x, y);
	}

	void calculateWindowCoordinates(Vertex & v)
	{
		if (ready)
		{
			v.x = (v.x + 1) * width_2_x;
			v.y = (v.y + 1) * height_2_y;
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
		viewportMatrix = Matrix(width/2, 0.0f, 0.0f, 0.0f, 0.0f, height/2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, width/2 + x, height/2 + x, 0.0f, 1.0f);
		ready = true;
		viewportMatrixChanged = true;
	}

	Matrix getViewportMatrix()
	{
		return viewportMatrix;
	}

};



#endif /* VIEWPORT_H_ */
