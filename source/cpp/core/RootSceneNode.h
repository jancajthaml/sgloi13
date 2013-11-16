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
#include <limits>

#include <cmath>
#include <stdio.h>
#include <iostream>

#define ADAPTIVE_AA


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
	{
		lights.push_back(_light);
	}
	
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
		//FIXME
		for (std::vector< SceneNode* >::iterator it = children.begin(); it != children.end(); ++it)
			(*it)->rasterize(this->lights);
		
		//after rasterization delete all children to avoid redrawing.
		children.clear();
	}
	
	void raytrace()
	{
		printf("raytrace\n");

		//todo raytrace

		Matrix I = MVP.inverse();

		int_fast16_t y = -1;
		int_fast16_t x = -1;

		while( ++x<context.w )
		{
			y=-1;
			while( ++y<context.h )
			{
				context.lastSetPixelIndex					= ( x+context.w*y );

				#ifdef ADAPTIVE_AA
					context.buffer[context.lastSetPixelIndex] = antialiasing(Vertex((float)x, (float)y), Vertex((float) (x + 1), (float) (y + 1)), I, 0);
				#else
					context.buffer[context.lastSetPixelIndex] = castAndShade(createRay(Vertex((float)x, (float)y), I));
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

		return ray;
	}

	// Recursive 4-point AA
	Color antialiasing(Vertex s, Vertex e, Matrix I, int depth)
	{
		float dx = e.x - s.x;
		float dy = e.y - s.y;

		Vertex p1( s.x + 0.25f * dx, s.y + 0.25f * dy );
		Vertex p2( s.x + 0.75f * dx, s.y + 0.25f * dy );
		Vertex p3( s.x + 0.25f * dx, s.y + 0.75f * dy );
		Vertex p4( s.x + 0.75f * dx, s.y + 0.75f * dy );

		Color c1 = castAndShade( createRay(p1,I) );
		Color c2 = castAndShade( createRay(p2,I) );
		Color c3 = castAndShade( createRay(p3,I) );
		Color c4 = castAndShade( createRay(p4,I) );

		if( c1==c2 && c2==c3 && c3==c4 )
		{
			return c1;
		}
		else if( depth==0 )
		{
			return Color
			(
				( c1.r + c2.r + c3.r + c4.r ) * 0.25f,
				( c1.g + c2.g + c3.g + c4.g ) * 0.25f,
				( c1.b + c2.b + c3.b + c4.b ) * 0.25f
			);
		}
		else
		{
			Vertex f(s.x + 0.5f * dx, s.y + 0.5f * dy);
			p1 = Vertex(s.x + 0.5f * dx, s.y);
			p2 = Vertex(s.x, s.y + 0.5f * dy);
			p3 = Vertex(s.x + 0.5f * dx, e.y);
			p3 = Vertex(e.x, s.y + 0.5f * dy);

			depth--;
			c1 = antialiasing( s,  f,  I, depth );
			c2 = antialiasing( p1, p4, I, depth );
			c3 = antialiasing( f,  e,  I, depth );
			c4 = antialiasing( p2, p3, I, depth );

			return Color
			(
				( c1.r + c2.r + c3.r + c4.r ) * 0.25f,
				( c1.g + c2.g + c3.g + c4.g ) * 0.25f,
				( c1.b + c2.b + c3.b + c4.b ) * 0.25f
			);
		}
	}
	Color castAndShade(const Ray &ray)
	{

		float tmin = std::numeric_limits<float>::max();
		Model *model;
		float t	 = std::numeric_limits<float>::max();

		//FIXME
		for (std::vector<SceneNode *>::iterator it = children.begin(); it != children.end(); ++it)
		{
			//find nearest object
			if ((*it)->getModel()->findIntersection(ray, t))
			{
				//there is a bug also
				if( t<tmin )
				{
					tmin  = t;
					model = (*it)->getModel();
				}
			}
		}
		if( tmin<std::numeric_limits<float>::max() )
		{
			Vertex i		= ray.extrapolate(tmin);
			Vertex normal	= model->getNormal(i);

			return phongModel(ray, model, i, normal);
		}

		return *context.clear;
	}

	/**
	 Calculation of phong lighting model
	 @param ray		ray that caused the intersection
	 @param model	model with which the ray intersected
	 @param i		intersection point of the ray and the model
	 @param n		normal in the intersection point
	 */
	Color phongModel( const Ray &ray, Model *model, const Vertex &i, const Vertex &normal ) const
	{
		//const Vertex &n causes nan
		Color color;
		Material material = model->getMaterial();
		Vertex E = ray.origin + ray.direction;
		E = E*(-1);
		E = E / E.length();

		//FIXME
		for( std::vector< Light >::const_iterator light = lights.begin(); light != lights.end(); ++light )
		{

			Vertex L = light->position - i;
			L.normalise();

			//-----AMBIENT


			//-----DIFFUSION

			float _LN	= L*normal;
			float LN	= _LN;

			const float Id_R = light->color.r * material.color.r * material.kd * LN;
			const float Id_G = light->color.g * material.color.g * material.kd * LN;
			const float Id_B = light->color.b * material.color.b * material.kd * LN;

			//-----SPECULAR

			//light is always white ... for now at least

			Vertex pointR;
			const float double_scalar = 2.0f * _LN;

			pointR.x = double_scalar * normal.x - L.x ;
			pointR.y = double_scalar * normal.y - L.y ;
			pointR.z = double_scalar * normal.z - L.z ;

			Vertex R(pointR.x,pointR.y,pointR.z);
			R.normalise();

			float COS_BETA = L*R;

			float Is_R = 0;
			float Is_G = 0;
			float Is_B = 0;
			if( COS_BETA<0 ) COS_BETA = 0;
			else
			{
				float ER	= powf(COS_BETA, material.shine);
				Is_R		= material.ks * ER;
				Is_G		= material.ks * ER;
				Is_B		= material.ks * ER;
			}

			//-----Resulting color


			color.r += Id_R + Is_R;
			color.g += Id_G + Is_G;
			color.b += Id_B + Is_B;
		}

		return color;
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
