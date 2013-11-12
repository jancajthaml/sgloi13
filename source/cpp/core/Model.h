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
#include "../struct/Light.h"
#include "../struct/Material.h"
#include "../struct/VertexStack.h"
#include "../struct/Vertex.h"
#include "../helpers/DrawingLibraryBase.h"
#include "ContextChunk.h"
#include "../struct/Matrix.h"
#include "../struct/Ray.h"
#include <vector>
#include <cstdio>
/**
 Base class for models. Models can be various types - line, line strip, polygon, triangle, etc.
 */
class Model
{
protected:
	///drawing library
	DrawingLibraryBase g;
	
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
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	Model(DrawingLibraryBase _g, Chunk _context)
	{
		this->g = _g;
		this->context = _context;
	}
	
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 @param _material	material to be used with this model
	 */
	Model(DrawingLibraryBase _g, Chunk _context, Material _material)
	{
		this->g = _g;
		this->context = _context;
		this->material = _material;
	}
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv){};
	
	
	virtual bool findIntersection(const Ray &ray, float &t){ return false; }
	
	inline Material &getMaterial()
	{
		return material;
	}
	
	
	inline void addVertex(Vertex v)
	{
		vertices.push_back(v);
	}
	
	inline void setPixelChunk( int y, int start, int end, float z, float z_growth, Chunk &context )
	{
		for( int x=start ; x<end ; x++ )
		{
			setPixel3D(x,y,z,context);
			z+=z_growth;
		}
	}
	
	inline void setPixel3D(float x, float y, float z, Chunk &context)
	{
		uint_fast32_t index = uint_fast32_t((int)x + context.w * (int)y);
		if (!context.depthTest)
		{
			context.lastSetPixelIndex	= index;
			context.depth[index]		= z;
			*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}
		else if (x >= 0 && x < context.w && y >= 0 && y < context.h && context.depth[index] > z)
		{
			context.lastSetPixelIndex	= index;
			context.depth[index]		= z;
			*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}
	}
	
	inline void multiplyVerticesWithMVP(Matrix MVP)
	{
		for (std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			(*it) = MVP * (*it);
			(*it).x/=(*it).w;
			(*it).y/=(*it).w;
			(*it).z/=(*it).w;
		}
	}
	
};

#endif
