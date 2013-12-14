//
//  SphereModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_SphereModel_h
#define libsgl_SphereModel_h
#include <cstdio>
#include <cmath>
#include "../struct/Material.h"
#include "../struct/Vertex.h"
#include "Model.h"
#include <vector>

class SphereModel : public Model
{
public:
	Vertex position;
	uint16 r;

	/**
	 Constructor of Spehre model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	SphereModel( Chunk _context, Material _material, const float _x, const float _y, const float _z, const uint16 _r ) : Model( _context, _material)
	{
		position = Vertex(_x,_y,_z);
		r		 = _r;
	}
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector< Light* > lights, Matrix mpv)
	{
		uint16 x = 0;
		uint16 y = r;
		int32  p = 3 - (r<<1);

		while( x<=y )
		{
			fillSymPixel( x, y, position.x, position.y, context);

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
			setPixel( from, h1, 0, context );
			setPixel( from, l1, 0, context );
		}

		to         = center_x+y;
		from       = center_x-y;
		h1         = center_y+x;
		l1         = center_y-x;

		for( ; from <= to; from++ )
		{
			setPixel( from, h1 , 0, context );
			setPixel( from, l1 , 0, context );
		}
	}

	virtual bool findIntersection(const Ray &ray, float &t)
	{
		const Vertex dst	= ray.origin - position;
		const float b		= dst * ray.direction;
		const float c		= ( dst*dst ) - r*r;
		const float d		= b*b - c;
		
		if( d>0 )
		{
							t = -b - Helper::q3sqrt(d);
			if( t<0.0f )	t = -b + Helper::q3sqrt(d);
			return true;
		}
		return false;
	}
	
	virtual Vertex getNormal(const Vertex &i)
	{
		Vertex dir = i - position;
		dir.normalise();
		return dir;
	}
	
	virtual const char* getName()
	{ return "SPH\n"; }
	
	virtual inline bool backfaceCull(const Ray &ray, const float &t)
	{ return false; }
	
	inline Vertex getUV(const Vertex vantage_point)	//const ?
	{
		Vertex Vn = Vertex(0,1,0);
		Vertex Ve = Vertex(1,0,0);

	    //FIXME faster acosf is Helper::acos
		float phi = acosf( -1.0f * ( Vn* vantage_point ));

	    float v = phi / PI;

	    //FIXME faster acosf is Helper::acos
	    //FIXME 2*PI should/could be cached

	    float theta = acosf((Ve*vantage_point) / sinf(phi)) / (2.0f * PI);
	    float u;

	    if ( ( Vertex::cross( Vn, Ve ) * vantage_point ) > 0 )
	       u = theta;
	    else
	       u = 1 - theta;

		return Vertex(u,v);
	}

};

#endif
