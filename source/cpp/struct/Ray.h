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
struct Ray
{
	///Origin of the ray
	Vertex origin;
	
	///Direction of the ray
	Vertex direction;
	
	
	inline Vertex extrapolate(const float t)const
	{
		return origin + (direction * t);
	}
};


#endif
