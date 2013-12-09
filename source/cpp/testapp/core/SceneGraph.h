//
//  RootSceneNode.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_RootSceneNode_h
#define libsgl_RootSceneNode_h

#include <iterator>
#include <vector>
#include "SceneNode.h"
#include "ContextChunk.h"
#include "../struct/Ray.h"
#include "./../shader/Flat.h"
#include "./../shader/Phong.h"
#include "./../shader/Toon.h"
#include "./../shader/Rim.h"
#include "./../shader/Ward.h"
#include <cmath>
#include <stdio.h>
#include <iostream>

//Adaptive antialiasing and shader types defined there
#define ADAPTIVE_AA
//#define DOF_AA
#define USE_SHADER 1	//0-Flat, 1-Phong, 2-Ward, 3-Toon, 4-Rim

const int RAY_RECURSION_DEPTH = 8;

class RootSceneNode : public SceneNode
{
private:
	///lights on scene
	std::vector< Light > lights;

	///Pointer to node currently being changed
	/// - i.e. when the node is being created using sglVertexnf().
	///when not null, this node is not part of child nodes.
	///Once commitCurrentNode() is called, the node is added to child nodes
	///and this pointer becomes NULL.
	SceneNode *currentNode;

public:
	Chunk context;

	RootSceneNode()
	{ currentNode = NULL; }

	/**
	 Constructor of root scene node
	 */
	RootSceneNode(Chunk _context)
	{
		this->context	= _context;
		currentNode		= NULL;
	}

	/**
	 Adds light to the scene
	 @param _light	the light to be added
	 */
	virtual void addLight(Light _light)
	{ lights.push_back(_light); }

	/**
	 Rasterizes this node
	 @param _lights additional lights to be added before rasterization
				effective when all lights are known so far and were not 
				added one by one to the RootSceneNode.
	 */
	virtual void rasterize(std::vector< Light > _lights)
	{
		this->lights.insert(this->lights.end(), _lights.begin(), _lights.end());
		this->rasterize();
	}

	void rasterize()
	{
		for( std::vector< SceneNode* >::iterator iter = children.begin(); iter != children.end(); ++iter )
			(*iter)->rasterize(lights);

		//after rasterization delete all children to avoid redrawing.
		children.clear();
	}

	void reset()
	{
		children.clear();
		lights.clear();
	}

	void raytrace()
	{
		//todo raytrace

		const Matrix I = MVP.inverse();
		uint16 y = -1;
		uint16 x = -1;

		while( ++x<context.w )
		{
			y=-1;
			while( ++y<context.h )
			{
				context.lastSetPixelIndex					= ( x+context.w*y );

				#ifdef DOF_AA
					context.buffer[context.lastSetPixelIndex] = DOF(Vertex(x,y),0,I);
				#else
				{
					#ifdef ADAPTIVE_AA
						context.buffer[context.lastSetPixelIndex] = castAndShadeAntialiased(createRay(Vertex( x,y ), I),x,y);
					#else
						context.buffer[context.lastSetPixelIndex] = castAndShade(createRay(Vertex( x,y ), I));
					#endif
				}
				#endif
			}
		}
	}

	//This method is horribly slow.... !!!!
	//FIXME
	Ray createRay(Vertex v, Matrix I)
	{
		float w=0.0f;

		v.z	= -1.0f;
		v.w	= 1.0f;

		Vertex A	= I * v;//I * [x y -1 1];

		//SLOW HERE
		w	= 1.0f/A.w;
		A.x = A.x*w;
		A.y = A.y*w;
		A.z = A.z*w;
		A.w = 1.0f;

		v.z	= 1.0f;
		Vertex B = I * v;//[x y 1 1];

		//SLOW HERE
		w	= 1.0f/B.w;
		B.x = B.x*w;
		B.y = B.y*w;
		B.z = B.z*w;
		B.w = 1.0f;

		Ray ray;

		ray.origin	= A;
		ray.direction = (B - A);
		ray.depth		= RAY_RECURSION_DEPTH-1;

		ray.direction.normalise();

		return ray;
	}

	Color DOF(const Vertex sample, int8 depth, Matrix I)
	{
		//FIXME THIS DOESNT WORK
		Ray     ray;
		Color   color;
		Color   color1;
		Color   color2;
		Color   color3;
		Color   color4;

		float shift = powf(0.5f,float(depth));

		Vertex sample1 = Vertex(sample.x+shift, sample.y+shift, 1.f);
		Vertex sample2 = Vertex(sample.x+shift, sample.y-shift, 1.f);
		Vertex sample3 = Vertex(sample.x-shift, sample.y+shift, 1.f);
		Vertex sample4 = Vertex(sample.x-shift, sample.y-shift, 1.f);

		ray = createRay(sample1,I);

		color1	= castAndShade(ray);
		ray		= createRay(sample2, I);
		color2	= castAndShade(ray);
		ray		= createRay(sample3, I);
		color3	= castAndShade(ray);
		ray		= createRay(sample4, I);
		color4	= castAndShade(ray);

		if((Helper::areColorsSimilar(color1,color2) && Helper::areColorsSimilar(color3,color4) && Helper::areColorsSimilar(color1,color3)) || depth > 4)
		{
			color = (color1+color2+color3+color4)*0.25f;
		}
		else
		{
			depth++;
			shift*=0.5f;
			color = (DOF(Vertex(sample.x+shift, sample.y+shift, 1.f), depth,I)+
					DOF(Vertex(sample.x+shift, sample.y-shift, 1.f), depth,I)+
					DOF(Vertex(sample.x-shift, sample.y+shift, 1.f), depth,I)+
					DOF(Vertex(sample.x-shift, sample.y-shift, 1.f), depth,I))*0.25f;
		}

		return color;
	}

	///
	Color castAndShade(const Ray &ray)
	{
		//FIXME punk switch... needs some abstraction and setters
		switch( USE_SHADER )
		{
			//case 0 : return Flat()  . calculateColor(ray, model, i, normal, lights);
			case 1  : return Phong::castAndShade ( ray, children, lights ,context);
			case 2  : return Ward::castAndShade  ( ray, children, lights, *context.clear );
			case 3  : return Toon::castAndShade  ( ray, children, lights ,context );
			case 4  : return Rim::castAndShade   ( ray, children, lights ,context );
			default : return *context.clear;
		}
	}

	///
	Color castAndShadeAntialiased(const Ray &ray,const uint16 x, const uint16 y)
	{
		//FIXME punk switch... needs some abstraction and setters
		switch( USE_SHADER )
		{
			//case 0 : return Flat()  . calculateColor(ray, model, i, normal, lights);
			case 1 : return Phong::castAndShade ( ray, children, lights ,context);
			case 2 : return Ward::castAndShade  ( ray, children, lights, *context.clear );
			case 3 : return Toon::castAndShade  ( ray, children, lights ,context );
			case 4 : return Rim::castAndShade   ( ray, children, lights ,context );
			default: return *context.clear;
		}
	}

	/**
	 Sets currentNode to node
	 @param node	pointer node to be added
	 */
	void beginNewNode(SceneNode *node)
	{ this->currentNode = node; }

	/**
	 Adds currentNode to children and sets currentNode to NULL.
	 As the currentNode was pointer to allocated memory (using new), it will be freed using delete.
	 */
	void commitCurrentNode()
	{
		if( currentNode!=NULL )  children.push_back(currentNode);
		currentNode = NULL;
	}

	/**
	 Get the current node
	 */
	SceneNode * getCurrentNode()
	{ return currentNode; }

	void setMVP( Matrix mvp )
	{ MVP = mvp; }
	
};

#endif
