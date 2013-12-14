//
//  Ray.h
//  libsgl
//
//  Created by Jan Brejcha on 12.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_Ray_h
#define libsgl_Ray_h
#include "Vertex.h"
#include "../core/types.h"

#define RAY_PRIMARY 0
#define RAY_SECONDARY 1
#define RAY_GI 2
#define RAY_SHADOW 3

struct Ray
{
	///Origin of the ray
	Vertex origin;
	
	///Direction of the ray
	Vertex direction;
	
	///Depth of a ray
	int8 depth;	//FIXME UINT
	int8 type;	//FIXME UINT

	float environment;	 ///< environment conastant;
	float tMax;  ///< No intersection after tMax distance
	float tMin;  ///< No intersection before tMin distance

	inline Vertex extrapolate( const float t ) const
	{ return origin + (direction * t); }

	Ray()
	{
		reset();
	}

	void reset()
	{
		depth  		= 0;
		tMax   		= FLOAT_MAX;
		tMin   		= 0.f;
		environment	= 1;
		type		= RAY_PRIMARY;
	}
};


#endif
