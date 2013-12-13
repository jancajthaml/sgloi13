//
//  LightNode.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_LightNode_h
#define libsgl_LightNode_h
#include "./../Color.h"
#include "./../Vertex.h"
#include "./../Ray.h"
#include "../../core/SceneNode.h"
/** 
 GraphNode base element for Lights.
 */

//TODO COMMENT
class Light : public SceneNode
{
	public:
		Vertex position;
		Color color;

		Light(){}
		virtual ~Light(){};

		virtual void Sample( const Vertex& point, Ray& ray, Color& contribution, const float u = 0, const float v = 0)
		{}

		virtual bool isPoint()
		{ return false; }
};

#endif
