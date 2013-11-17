/*
 * CircleDrawModel.h
 *
 *  Created on: Nov 15, 2013
 *      Author: jancajthaml
 */

#ifndef CIRCLEDRAWMODEL_H_
#define CIRCLEDRAWMODEL_H_

class CircleDrawModel : public Model
{
private:
	int r;
	Vertex center;

public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	CircleDrawModel( Chunk _context, Material _material, int r, Vertex center ) : 	Model( _context, _material)
	{
		this->r = r;
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
		int_fast32_t x = 0;
		int_fast32_t y = r;
		int_fast32_t p = 3 - (int_fast32_t(r) << 1);

		while( x<y )
		{
			setSymPixel(x, y, center.x, center.y, context);
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
		if( x==y ) setSymPixel(x, y, center.x, center.y, context);
	}

	static inline void setSymPixel(signed x, signed y, signed xs, signed ys, Chunk &context)
	{
		int rx = x + xs;
		int ry = y + ys;
		int mrx = -x + xs;
		int mry = -y + ys;
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
