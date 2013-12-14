/*
 * Color.h
 *
 *  Created on: 4.12.2013
 *      Author: jancajthaml
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "./../core/types.h"

struct Bitmap
{

	int32 texture_width;
	int32 texture_height;
	float *texture;

	Bitmap()
	{
		this->texture			= 0;
		this->texture_width		= 0;
		this->texture_height	= 0;
	}

	Bitmap(int32 w, int32 h, float* texels)
	{
		  this->texture			= texels;
		  this->texture_width	= w;
		  this->texture_height	= h;
	}

	Color getColor(const float U, const float V) const
	{
		int sx = 3 * int((1-U) * texture_width);
		int sy = 3 * int(V * texture_height);
		//printf("sx: %d, sy: %d\n", sx, sy);
		int offset = (sx + sy * texture_width);

		Color c = Color
		(
			texture[offset],
			texture[offset+1],
			texture[offset+2]
		);

		return c;
	}

};

#endif /* BITMAP_H_ */
