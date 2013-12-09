/*
 * CircleFillModel.h
 *
 *  Created on: Nov 15, 2013
 *      Author: jancajthaml
 */

#ifndef CIRCLEFILLMODEL_H_
#define CIRCLEFILLMODEL_H_

#include "types.h"

class CircleFillModel : public Model
{
private:
	uint16 r;
	Vertex center;
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	CircleFillModel( Chunk _context, Material _material, uint16 r, Vertex center) : 	Model( _context, _material)
	{
		this->r      = r;
		this->center = center;
	}

	void flush()
	{ rasterize(std::vector<Light>(),MatrixCache::identity()); }

	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		uint16 x = 0;
		uint16 y = r;
		int32  p = 3 - (r<<1);

		while( x<=y )
		{
			fillSymPixel( x, y, center.x, center.y, context);

			if( p<0 )
			{
				p += (x << 1) + 6;
			}
			else
			{
				p += ((x - y) << 1) + 10;
				y -= 1;
			}
			x += 1;
		}
	}

	static inline void fillSymPixel(uint32 x, uint32 y, uint32 center_x, uint32 center_y, Chunk &context)
	{
		uint32 to   = center_x+x;
		uint32 from = center_x-x;
		uint32 h1   = center_y+y;
		uint32 l1   = center_y-y;

		for( ; from <= to; from++ )
		{
			setPixel3D( from, h1, 0, context );
			setPixel3D( from, l1, 0, context );
		}

		to         = center_x+y;
		from       = center_x-y;
		h1         = center_y+x;
		l1         = center_y-x;

		for( ; from <= to; from++ )
		{
			setPixel3D( from, h1 , 0, context );
			setPixel3D( from, l1 , 0, context );
		}
	}

};


#endif /* CIRCLEFILLMODEL_H_ */
