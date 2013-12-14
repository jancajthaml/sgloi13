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

struct Ray
{
	///Origin of the ray
	Vertex origin;
	
	///Direction of the ray
	Vertex direction;
	
	///Depth of a ray
	int8 depth;	//FIXME UINT
	int8 type;	//FIXME UINT

	inline Vertex extrapolate( const float t ) const
	{ return origin + (direction * t); }

	Ray()
	{
		reset();
	}

	void reset()
	{
		depth  		= 0;
		type		= RAY_PRIMARY;
	}
};


#endif
