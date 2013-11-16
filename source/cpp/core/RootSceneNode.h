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
		Vertex A, B, D, e;
		Matrix I = MVP.inverse();
		float w=0.0f;

		int_fast16_t y = -1;
		int_fast16_t x = -1;

		while( ++y<context.h )
		{
			x=-1;
			while( ++x<context.w )
			{
				e.x = x;
				e.y = y;
				e.z = -1;
				e.w = 1;
				A	= I * e;//I * [x y -1 1];
				w	= 1.0f/A.w;
				A.x = A.x*w;
				A.y = A.y*w;
				A.z = A.z*w;
				A.w = 1.0;
				
				e.z	= 1;
				B	= I * e;//[x y 1 1];
				w	= 1.0f/B.w;
				B.x = B.x*w;
				B.y = B.y*w;
				B.z = B.z*w;
				B.w = 1.0;
				
				D = (B - A) / (B - A).length();
				Ray ray;
				ray.origin = A;
				ray.direction = D;

				context.lastSetPixelIndex					= ( x+context.w*y );
				context.buffer[context.lastSetPixelIndex]	= castAndShade(ray);
			}
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

			//this function returns "nan" -- cause of a bug
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

			//---------------------------------------------------------------------------------------ambient part

			//const float Ia_R = material.color.r ;
			//const float Ia_G = material.color.g ;
			//const float Ia_B = material.color.b ;

			//---------------------------------------------------------------------------------------difusion part

			float _LN	= L*normal;
			float LN	= _LN;

			const float Id_R = light->color.r * material.color.r * material.kd * LN;
			const float Id_G = light->color.g * material.color.g * material.kd * LN;
			const float Id_B = light->color.b * material.color.b * material.kd * LN;

			//----------------------------------------------------------------------------------------mirror part

			//light is always white ... for now at least

			Vertex pointR;
			const float double_scalar = 2.0f * _LN;

			pointR.x = double_scalar * normal.x - L.x ;
			pointR.y = double_scalar * normal.y - L.y ;
			pointR.z = double_scalar * normal.z - L.z ;

			Vertex R(pointR.x,pointR.y,pointR.z);
			R.normalise();

			float COS_BETA = L*R;

			if( COS_BETA<0 ) COS_BETA = 0;

			float ER = powf(COS_BETA, material.shine);

			//----------------------------------------------------------------------------------------complete all

			const float Is_R = material.ks * ER;
			const float Is_G = material.ks * ER;
			const float Is_B = material.ks * ER;

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
