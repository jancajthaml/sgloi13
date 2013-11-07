//
//  FillTrianglesFanModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_FillTrianglesFanModel_h
#define libsgl_FillTrianglesFanModel_h

class FillTrianglesFanModel : public Model
{
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	FillTrianglesFanModel(DrawingLibraryBase _g, Chunk _context) : Model(_g, _context)
	{
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
		Model::multiplyVerticesWithMVP(mpv);
		uint_fast16_t size = vertices.size();
		if(size < 3) return;
		
		Vertex center	= vertices[0];
		Vertex A		= vertices[1];
		Vertex B		= vertices[2];
		
		for (uint_fast16_t i = 2; i < size; i++)
		{
			g.drawTriangle(center, A, B, context);
			A	= B;
			B	= vertices[i];
		}
		g.drawTriangle(center, A, B, context);
	}
};

#endif
