//
//  SphereModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_SphereModel_h
#define libsgl_SphereModel_h

class SphereModel : public Model
{
public:
	float x;
	float y;
	float z;
	float r;
	/**
	 Constructor of Spehre model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	SphereModel(DrawingLibraryBase _g, Chunk _context, const float _x, const float _y, const float _z, const float _r) : Model(_g, _context)
	{
		x = _x;
		y = _y;
		z = _z;
		r = _r;
	}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		throw std::runtime_error( "rasterization of parametric sphere is unsupported." );
	}
};


#endif
