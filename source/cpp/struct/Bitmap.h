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
		int offset = (int(U * texture_width) + int((1.0f - V) * texture_height) * texture_width) * 3;
		float f = 1.0f/255;

		Color c = Color
		(
			texture[offset] * f,
			texture[offset+1] * f,
			texture[offset+2] * f
		);

		return c;
	}

};

#endif /* BITMAP_H_ */
