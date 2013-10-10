/*
 * Viewport.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_
#include <stdint.h>

struct Viewport
{

	int_fast16_t width;		//Maximum 65536
	int_fast16_t height;	//Maximum 65536
	float width_2_x;
	float height_2_y;
	int_fast32_t x;
	int_fast32_t y;
	bool ready;
	Matrix viewportMatrix;
	bool viewportMatrixChanged;

	Viewport()
	{
		width					= 0;
		height					= 0;
		width_2_x				= 0.0f;
		height_2_y				= 0.0f;
		x						= 0;
		y						= 0;
		ready					= false;
		viewportMatrixChanged	= true;
	}

	inline float calculateRatio()
	{
		float number = width * width + height * height;
		float x2 = number * 0.5F;
		float y = number;
		long i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (1.5F - (x2*y*y));
		y = y * (1.5F - (x2*y*y));
		return (static_cast<int>(1/y + 0.5f))/ 2.8284271247461903;

	}

	inline Viewport(int_fast16_t width, int_fast16_t height, int_fast16_t x, int_fast16_t y)
	{ changeViewport(width, height, x, y); }

	inline void calculateWindowCoordinates(Vertex & v)
	{
		if (ready)
		{
			v.x = (v.x + 1) * width_2_x;
			v.y = (v.y + 1) * height_2_y;
		}
	}

	inline void changeViewport(int_fast16_t width, int_fast16_t height, int_fast16_t x, int_fast16_t y)
	{
		this->width				= width;
		this->height			= height;
		this->width_2_x			= ((float)(width>>1)) + x;
		this->height_2_y		= ((float)(height>>1)) + y;
		this->x					= x;
		this->y					= y;
		viewportMatrix			= Matrix((width>>1), 0.0f, 0.0f, 0.0f, 0.0f, (height>>1), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, (width>>1) + x, (height>>1) + x, 0.0f, 1.0f);
		ready					= true;
		viewportMatrixChanged	= true;
	}

};

#endif /* VIEWPORT_H_ */
