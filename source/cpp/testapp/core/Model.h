//
//  Model.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_Model_h
#define libsgl_Model_h

#include <vector>
#include "./../helpers/Helpers.h"
#include "../struct/Light.h"
#include "../struct/Material.h"
#include "../struct/Vertex.h"
#include "ContextChunk.h"
#include "../struct/Matrix.h"
#include "../struct/Ray.h"

/**
 Base class for models. Models can be various types - line, line strip, polygon, triangle, etc.
 */
class Model
{
protected:
	
	///graphics context
	Chunk context;
	
	///stack holding vertices of this model
	std::vector< Vertex > vertices;
	
	///material of this model
	Material material;
	
public:
	
	Model(){}
	
	virtual ~Model(){};
	
	/**
	 Constructor of model
	 @param _context	graphics context
	 */
	Model( Chunk _context)
	{ this->context = _context; }
	
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 @param _material	material to be used with this model
	 */
	Model( Chunk _context, Material _material)
	{
		this->context = _context;
		this->material = _material;
	}
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{};
	
	virtual bool findIntersection(const Ray &ray, float &t)
	{ return false; }
	
	inline Material &getMaterial()
	{ return material; }
	
	/**
	 calculates normal at intersection point i
	 @param i	intersection point of ray and model
	 */
	virtual Vertex getNormal(const Vertex &i)
	{ return Vertex(0.0f, 0.0f, 0.0f, 1.0f); }
	
	inline void addVertex(Vertex v)
	{ vertices.push_back(v); }
	
	static inline void setPixelChunk( int y, int start, int end, float z, float z_growth, Chunk &context )
	{
		for( int x=start ; x<end ; x++ )
		{
			setPixel3D( x,y,z,context );
			z += z_growth;
		}
	}
	
	static inline void setPixel3D(float x, float y, float z, Chunk &context)
	{
		int index = int(x) + context.w * int(y);

		if( !context.depthTest )
		{
			context.lastSetPixelIndex					= index;
			context.buffer[context.lastSetPixelIndex]	= context.color;
		}
		else if (x >= 0 && x < context.w && y >= 0 && y < context.h && context.depth[index] > z)
		{
			context.lastSetPixelIndex					= index;
			context.depth[index]						= z;
			context.buffer[context.lastSetPixelIndex]	= context.color;
		}
	}
	
	static inline void bresenham_x(int x1, int y1, int z1, int x2, int y2, int z2, Chunk &context)
	{
		int dx	= x2 - x1;
		int dy	= y2 - y1;
		int sign	= 1;

		if( dy<0 ) sign = -1;

		int c0	= (dy << 1) * sign;
		int c1	= c0 - (dx << 1);
		int p	= c0 - dx;

		setPixel3D( x1, y1, z1, context );

		for( size_t i = x1 + 1; i <= x2; ++i )
		{
			if( p<0 )
			{
				p += c0;
				setPixel_x( context );
			}
			else
			{
				p += c1;
				if (sign > 0)	setPixel_xy  ( context );
				else			setPixel_xmy ( context );
			}
		}
	}

	static inline void bresenham_y(int x1, int y1, int z1, int x2, int y2, int z2, Chunk &context)
	{
		int dx	= x2 - x1;
		int dy	= y2 - y1;
		int sign	= 1;

		if( dy<0 )	sign = -1;

		int c0	= (dy << 1) * sign;
		int c1	= c0 - (dx << 1);
		int p	= c0 - dx;

		setPixel3D( y1, x1, z1, context );

		for( size_t i = x1 + 1; i <= x2; ++i )
		{
			if (p < 0)
			{
				p += c0;
				setPixel_y(context);
			}
			else
			{
				p += c1;
				if( sign>0 )	setPixel_xy  (context);
				else			setPixel_mxy (context);
			}
		}
	}

	static inline void setPixel_x( Chunk &context )
	{
		context.lastSetPixelIndex++;

		if( context.lastSetPixelIndex < context.w_h )
			context.buffer[context.lastSetPixelIndex]=context.color;
	}

	static inline void setPixel_y( Chunk &context )
	{
		context.lastSetPixelIndex += context.w;

		if( context.lastSetPixelIndex < context.w_h )
			context.buffer[context.lastSetPixelIndex]=context.color;
	}

	static inline void setPixel_xy( Chunk &context )
	{
		context.lastSetPixelIndex += context.w + 1;

		if( context.lastSetPixelIndex < context.w_h )
			context.buffer[context.lastSetPixelIndex]=context.color;
	}

	static inline void setPixel_mxy( Chunk &context )
	{
		context.lastSetPixelIndex += context.w - 1;

		if( context.lastSetPixelIndex < context.w_h )
			context.buffer[context.lastSetPixelIndex]=context.color;
	}

	static inline void setPixel_xmy( Chunk &context )
	{
		context.lastSetPixelIndex += 1 - context.w;

		if( context.lastSetPixelIndex < context.w_h )
			context.buffer[context.lastSetPixelIndex]=context.color;
	}

	inline void multiplyVerticesWithMVP(Matrix MVP)
	{
		size_t i	= -1				;
		size_t size	= vertices.size()	;
		Vertex v						;
		float w							;

		while( ++i<size )
		{
			v	 = vertices[i];
			w	 = 1.0f/v.w;
			v	 = MVP * v;
			v.x *= w;
			v.y *= w;
			v.z *= w;
		}
	}
	
};

#endif
