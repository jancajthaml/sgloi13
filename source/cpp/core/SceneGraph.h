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
#include "./../shader/Ward.h"
#include <cmath>
#include <stdio.h>
#include <iostream>

//Adaptive antialiasing and shader types defined there
#define ADAPTIVE_AA
//#define DOF_AA
#define USE_SHADER 1	//0-Flat, 1-Phong, 2-Ward

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
	
		void raytrace()
		{
			//todo raytrace

			const Matrix I = MVP.inverse();
			short y = -1;
			short x = -1;

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
							context.buffer[context.lastSetPixelIndex] = antialiasing(Vertex( x,y ), Vertex( x+1,y+1 ), I, 2);
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
		A.w = 1.0;

		v.z	= 1.0f;
		Vertex B = I * v;//[x y 1 1];

		//SLOW HERE
		w	= 1.0f/B.w;
		B.x = B.x*w;
		B.y = B.y*w;
		B.z = B.z*w;
		B.w = 1.0;

		Vertex D = (B - A);
		D.normalise();

		Ray ray;
		ray.origin		= A;
		ray.direction	= D;
		ray.depth		= 1;

		return ray;
	}

	// Recursive 4-point AA
	Color antialiasing(Vertex s, Vertex e, Matrix I, int depth)
	{
		float dx = e.x - s.x;
		float dy = e.y - s.y;

		float dx_1 = dx*0.25f;
		float dy_1 = dy*0.25f;
		float dx_2 = dx*0.75f;
		float dy_2 = dy*0.75f;

		Vertex p1( s.x + dx_1, s.y + dy_1 );
		Vertex p2( s.x + dx_2, s.y + dy_1 );
		Vertex p3( s.x + dx_1, s.y + dy_2 );
		Vertex p4( s.x + dx_2, s.y + dy_2 );

		Color c1 = castAndShade( createRay(p1,I) );
		Color c2 = castAndShade( createRay(p2,I) );
		Color c3 = castAndShade( createRay(p3,I) );
		Color c4 = castAndShade( createRay(p4,I) );

		if(Helper::areColorsSame(c1,c2,c3,c4)) return c1;
		else if( depth==0 )
		{
			return (c1+c2+c3+c4)*0.25f;
		}
		else
		{
			dx_1 = dx*0.5f;
			dy_1 = dy*0.5f;
			Vertex f(s.x + dx_1, s.y + dy_1);
			p1 = Vertex(s.x + dx_1, s.y);
			p2 = Vertex(s.x, s.y + dy_1);
			p3 = Vertex(s.x + dx_1, e.y);
			p3 = Vertex(e.x, s.y + dy_1);

			depth--;

			c1 = antialiasing( s,  f,  I, depth );
			c2 = antialiasing( p1, p4, I, depth );
			c3 = antialiasing( f,  e,  I, depth );
			c4 = antialiasing( p2, p3, I, depth );

			return (c1+c2+c3+c4)*0.25f;
		}
	}

	Color DOF(const Vertex sample, int depth, Matrix I)
	{

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
			case 1 : return Phong::castAndShade( ray, children, lights ,context);
			case 2 : return Ward::castAndShade( ray, children, lights, *context.clear );
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
	inline SceneNode * getCurrentNode()
	{ return currentNode; }
	
	void setMVP(Matrix mvp)
	{ MVP = mvp; }
	
};

#endif
