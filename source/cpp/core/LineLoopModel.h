//
//  LineLoopModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_LineLoopModel_h
#define libsgl_LineLoopModel_h

#include "LineStripModel.h"

class LineLoopModel : public LineStripModel
{
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	LineLoopModel( Chunk _context, Material _material) : 	LineStripModel( _context, _material){}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector< Light* > lights, Matrix mpv)
	{
		LineStripModel::rasterize(lights, mpv);
		drawLine2D(vertices[vertices.size()-1], vertices[0], context);
	}
};


#endif
