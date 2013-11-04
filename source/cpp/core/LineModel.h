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
	LineModel(DrawingLibraryBase _g, Chunk _context)
	{
		Model(_g, _context);
		this->g = _g;
		this->context = _context;
	}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		for (uint_fast32_t i = 0; i < vertices.size(); i += 2)
			g.drawLine2D(vertices[i], vertices[i+1], context);
	}
};

#endif
