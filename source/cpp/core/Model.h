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
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		printf("drawing model\n");
	};
	
	
	inline void addVertex(Vertex v)
	{
		printf("adding vertex\n");
		vertices.push_back(v);
	}
	
};

#endif
