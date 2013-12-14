/*
 * CircleDrawModel.h
 *
 *  Created on: Nov 15, 2013
 *      Author: jancajthaml
 */

#ifndef CIRCLEDRAWMODEL_H_
#define CIRCLEDRAWMODEL_H_

#include "types.h"

class CircleDrawModel : public Model
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
	CircleDrawModel( Chunk _context, Material _material, uint16 r, Vertex center ) : 	Model( _context, _material)
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
	virtual void rasterize( std::vector<Light> lights, Matrix mpv )
	{
		uint16 x = 0;
		uint16 y = r;
		int32  p = 3 - (r << 1);

		while( x<y )
		{
			setSymPixel(x, y, center.x(), center.y(), context);
			if( p<0 )
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
		if( x==y ) setSymPixel(x, y, center.x(), center.y(), context);
	}

	static inline void setSymPixel(uint32 x, uint32 y, uint32 xs, uint32 ys, Chunk &context)
	{
		uint32 rx  = x + xs;
		uint32 ry  = y + ys;
		uint32 mrx = -x + xs;
		uint32 mry = -y + ys;

		setPixel3D(rx, ry, 0, context);
		setPixel3D(rx, mry, 0, context);
		setPixel3D(mrx, ry, 0, context);
		setPixel3D(mrx, mry, 0, context);
		rx = x + ys;
		ry = y + xs;
		mrx = -x + ys;
		mry = -y + xs;
		setPixel3D(ry, rx, 0, context);
		setPixel3D(ry, mrx, 0, context);
		setPixel3D(mry, rx, 0, context);
		setPixel3D(mry, mrx, 0, context);
	}

};

#endif /* CIRCLEDRAWMODEL_H_ */
