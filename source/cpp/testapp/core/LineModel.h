//
//  LineModel.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_LineModel_h
#define libsgl_LineModel_h
#include "Model.h"

class LineModel : public Model
{
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	LineModel( Chunk _context, Material _material) : Model( _context, _material){}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		Model::multiplyVerticesWithMVP(mpv);
		uint16 i = -2;
		const uint16 size = vertices.size();

		while( (i+=2)<size )
			drawLine2D(vertices[i],vertices[i+1],context);
	}

	static inline void drawLine2D(Vertex a, Vertex b, Chunk &context)
	{
		const uint16 dx = Helper::abs(b.x() - a.x());
		const uint16 dy = Helper::abs(b.y() - a.y());

		if( dx>dy )
			if( a.x()<b.x() )	bresenham_x(a.x(), a.y(), a.z(), b.x(), b.y(), b.z(), context);
			else				bresenham_x(b.x(), b.y(), a.z(), a.x(), a.y(), b.z(), context);
		else
			if( a.y()<b.y() )	bresenham_y(a.y(), a.x(), a.z(), b.y(), b.x(), b.z(), context);
			else				bresenham_y(b.y(), b.x(), a.z(), a.y(), a.x(), b.z(), context);
	}

};

#endif
