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
	FillTrianglesFanModel( Chunk _context, Material _material) : Model( _context, _material){}
	
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
		
		size_t i = 1;

		while( ++i<size )
		{
			triangle(center, A, B, context);
			A	= B;
			B	= vertices[i];
		}

		triangle(center, A, B, context);
	}


	void triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, Chunk &context)
	{
		//TODO IMPLEMENT WITH Z!
	}

};

#endif
