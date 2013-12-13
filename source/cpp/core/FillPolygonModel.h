//
//  FillPolygonModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_FillPolygonModel_h
#define libsgl_FillPolygonModel_h

#include "../helpers/Helpers.h"

#define USE_TRIANGLE_NORMAL

class FillPolygonModel : public Model
{
private:
	Vertex normal;
	bool cached;
	Vertex cache_10;
	Vertex cache_20;
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	FillPolygonModel( Chunk _context, Material _material) : Model( _context, _material)
	{
		normal.w = -1;
		cached   = false;
	}
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 @author Jan Cajthaml
	 */
	virtual void rasterize(std::vector< Light* > lights, Matrix mpv)
	{
		Model::multiplyVerticesWithMVP(mpv);
		
		const uint16 size   =  vertices.size()		;
		Edge * edges  =  new Edge[size]         ;
		float  delta  =  0.0f                   ;
		
		int16* x     =  new int16[size]          ;
		int16* y     =  new int16[size]          ;
		
		x[0] = int16(vertices[0].x);
		y[0] = int16(vertices[0].y);
		
		int16    min_y  =  y[0];
		int16    max_y  =  y[0];
		
		uint16 i = -1;

		while( ++i<size )
		{
			x[i] = int16(vertices[i].x);
			y[i] = int16(vertices[i].y);
			
			if( y[i]<min_y )  min_y = y[i];
			if( y[i]>max_y )  max_y = y[i];
			
			if( y[i] < y[i-1] )
			{
				edges[i].min_y  =  y[i]-1 ;
				edges[i].max_y  =  y[i-1]   ;
				edges[i].x      =  x[i];
				edges[i].z      =  vertices[i].z   ;
			}
			else
			{
				edges[i].min_y  =  y[i-1]-1 ;
				edges[i].max_y  =  y[i]  ;
				edges[i].x      =  x[i-1]   ;
				edges[i].z      =  vertices[i-1].z   ;
			}
			
			delta            = float(y[i]-y[i-1]);
			edges[i].delta_x = float(x[i]-x[i-1]) / delta;
			edges[i].delta_z = (vertices[i].z - vertices[i-1].z) / delta;
		}
		
		if( y[0] < y[size-1] )
		{
			edges[0].min_y  =  y[0]-1 ;
			edges[0].max_y  =  y[size-1]  ;
			edges[0].x      =  x[0] ;
			edges[0].z      =  vertices[0] . z   ;
		}
		else
		{
			edges[0].min_y  =  y[size-1]-1 ;
			edges[0].max_y  =  y[0]   ;
			edges[0].x      =  x[size-1]  ;
			edges[0].z      =  vertices[size-1] . z   ;
		}
		
		
		delta             =  float(y[0]-y[size-1]);
		edges[0].delta_x  =  float(x[0]-x[size-1])/ delta;
		edges[0].delta_z  =  (vertices[0].z-vertices[size-1].z) / delta;
		
		float * draw   =  new float[size] ;
		float * drawZ  =  new float[size] ;
		uint16  count  =  0;
		
		for( uint16 y=min_y ; y<max_y ; y++ )
		{
			count = 0;
			for( uint16 v=0 ; v<size ; v++ )
			{
				if( (edges[v].min_y<y) & (edges[v].max_y>y) )
				{
					edges[v].x  += edges[v].delta_x;
					edges[v].z  += edges[v].delta_z;
					draw[count]  = edges[v].x;
					drawZ[count] = edges[v].z;
					
					count++;
				}
			}
			
			Helper::sort(draw,drawZ,count);
			
			for( uint16 i=0 ; i<count ; i=i+2 )
				setPixelChunk( y+1, draw[i], draw[i+1], drawZ[i], (drawZ[i+1]-drawZ[i])/(draw[i+1]-draw[i]), context );
		}
		// BUBBLE SORT END
		
		delete[] drawZ             ;
		delete[] draw              ;
		vertices.clear()		   ;
		delete[] edges             ;
	}

	virtual bool findIntersection(const Ray &ray, float &t)
	{
		if( !cached )
		{
			cached   = true;
			cache_20 = vertices[2] - vertices[0];
			cache_10 = vertices[1] - vertices[0];
		}

		Vertex s1		= Vertex::cross(ray.direction,cache_20);
		float divisor	= s1*cache_10;

		if( divisor==0.0f )  return false;

		float inverse = 1.0f/divisor;

		Vertex d	= ray.origin - vertices[0];
		float b1	= ( d*s1) * inverse;

		if( b1 < 0.0f || b1 > 1.0f ) return false;

		Vertex s2	= Vertex::cross(d,cache_10);
		float b2	= (ray.direction*s2) * inverse;

		if( b2 < 0.0f || b1 + b2 > 1.0f ) return false;

		float hit = (cache_20*s2) * inverse;
		if( hit < 0.0f || hit > 1073741824.0f ) return false;

		t = hit;
		return true;
	}

	virtual Vertex getNormal(const Vertex &i)
	{
		#ifdef USE_TRIANGLE_NORMAL
		if(normal.w==-1)
		{
			if(!cached)
			{
				cached   = true;
				cache_20 = vertices[2] - vertices[0];
				cache_10 = vertices[1] - vertices[0];
			}
			normal = Vertex::cross(cache_10,(vertices[2] - vertices[0]));
			normal.normalise();
		}
		return normal;
		#endif

		Vertex n = Vertex::cross((i - vertices[0]),(i - vertices[1]));
		n.normalise();

		return n;
	}
	
	virtual const char * getName()
	{ return "TRI\n"; }
	
	virtual inline bool backfaceCull(const Ray &ray, const float &t)
	{
		#ifdef USE_TRIANGLE_NORMAL
			return ray.direction * getNormal(normal) >= 0.0f;
		#endif

		return ray.direction * getNormal(ray.extrapolate(t)) >= 0.0f;
	}

};

#endif
