/*
 * CircleFillModel.h
 *
 *  Created on: Nov 15, 2013
 *      Author: jancajthaml
 */

#ifndef CIRCLEFILLMODEL_H_
#define CIRCLEFILLMODEL_H_

class CircleFillModel : public Model
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
	CircleFillModel( Chunk _context, Material _material, int r, Vertex center) : 	Model( _context, _material)
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

		while(x <= y)
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

	static inline void fillSymPixel(int x, int y, int center_x, int center_y, Chunk &context)
	{
		int to        = center_x+x;
		int from        = center_x-x;
		int h1        = center_y+y;
		int l1        = center_y-y;

		for( ; from <= to; from++ )
		{
			setPixel3D( from, h1, 0, context );
			setPixel3D( from, l1, 0, context );
		}

		to          = center_x+y;
		from        = center_x-y;
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
