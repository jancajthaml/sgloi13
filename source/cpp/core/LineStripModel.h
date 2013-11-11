//
//  LineStripModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_LineStripModel_h
#define libsgl_LineStripModel_h

class LineStripModel : public Model
{
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	LineStripModel(DrawingLibraryBase _g, Chunk _context, Material _material) : Model(_g, _context, _material){}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		Model::multiplyVerticesWithMVP(mpv);
		for (uint_fast16_t i = 0; i < vertices.size() - 1; ++i)
			g.drawLine2D(vertices[i], vertices[i+1], context);
	}
};


#endif
