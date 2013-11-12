//
//  SphereModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_SphereModel_h
#define libsgl_SphereModel_h
#include <cstdio>
#include "../struct/Material.h"

class SphereModel : public Model
{
public:
	Vertex position;
	float r;
	/**
	 Constructor of Spehre model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	SphereModel(DrawingLibraryBase _g, Chunk _context, Material _material, const float _x, const float _y, const float _z, const float _r) : Model(_g, _context, _material)
	{
		position.x = _x;
		position.y = _y;
		position.z = _z;
		position.w = 1.0f;
		position.print();
		r = _r/2.0;
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
	
	virtual bool findIntersection(const Ray &ray, float &t)
	{
		//printf("finding intersect of sphere\n");
		//position.print();
		const Vertex dst = ray.origin - position;
		//ray.origin.print();
		const float b = dst * ray.direction;
		const float c = (dst * dst) - r*r;
		const float d = b*b - c;
		
		if(d > 0) {
			t = -b - sqrtf(d);
			if (t < 0.0f)
				t = -b + sqrtf(d);
				return true;
		}
		return false;
	}
};


#endif
