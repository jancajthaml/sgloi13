//
//  LightNode.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_LightNode_h
#define libsgl_LightNode_h
#include "Color.h"
/** 
 GraphNode base element for Lights.
 */
struct Light
{
	///Position of the light
	Vertex position;
	
	///Color of the light
	Color color;
};

#endif
