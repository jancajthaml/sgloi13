/*
 * Viewport.h
 *
 *  Created on: 7.10.2013
 *      Author: Jan Brejcha & Jan Cajthaml
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "./../struct/Matrix.h"

//TODO COMMENT !!!!!!!
struct Viewport
{

	int_fast16_t width;		//Maximum 65536
	int_fast16_t height;	//Maximum 65536
	int_fast16_t width_2_x;
	int_fast16_t height_2_y;
	int_fast32_t x;
	int_fast32_t y;
	bool ready;
	Matrix V;
	bool V_changed;

	Viewport()
	{
		width					= 0;
		height					= 0;
		width_2_x				= 0;
		height_2_y				= 0;
		x						= 0;
		y						= 0;
		ready					= false;
		V_changed	= true;
	}

	inline float calculateRatio()
	{
		//Fast square root
		float number = width * width + height * height;
		float y = number;
		long i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (1.5f - ((number * 0.5f)*y*y));
		return (static_cast<int>(1/y + 0.5f))/2.8284271247461903f;
	}

	inline Viewport(int_fast16_t width, int_fast16_t height, int_fast16_t x, int_fast16_t y)
	{ changeViewport(width, height, x, y); }

	inline void calculateWindowCoordinates(Vertex & v)
	{
		if( ready )
		{
			v.x = x + (v.x + 1) * width_2_x;
			v.y = y + (v.y + 1) * height_2_y;
		}
	}

	inline void changeViewport(int_fast16_t x, int_fast16_t y, int_fast16_t width, int_fast16_t height)
	{
		this->width					=  width;
		this->height				=  height;
		this->width_2_x				=  width>>1;
		this->height_2_y			=  height>>1;
		this->x						=  x;
		this->y						=  y;

		this->V		=  Matrix
		(
			width_2_x, 0.0f, 0.0f, 0.0f,
			0.0f, height_2_y, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			width_2_x + x, height_2_y + x, 0.0f, 1.0f
		);
		this->ready		=  true;
		this->V_changed	=  true;
	}

};

#endif /* VIEWPORT_H_ */
