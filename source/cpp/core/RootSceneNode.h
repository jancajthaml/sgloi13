//
//  RootSceneNode.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_RootSceneNode_h
#define libsgl_RootSceneNode_h
#include "SceneNode.h"
#include "ContextChunk.h"
#include "../struct/Ray.h"
#include <vector>
#include <cfloat>
#include <cmath>

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
	
	RootSceneNode(){}
	
	/**
	 Constructor of root scene node
	 */
	RootSceneNode(Chunk _context){ this->context = _context; }
	
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
		//MVP.print();
		for (uint_fast16_t y = 0; y < context.w; y++)
		{
			for (uint_fast16_t x = 0; x < context.w; x++)
			{
				e.x = x;
				e.y = y;
				e.z = -1;
				e.w = 1;
				A = I * e;//I * [x y -1 1];
				A.x = A.x/A.w;
				A.y = A.y/A.w;
				A.z = A.z/A.w;
				A.w = 1.0;
				
				e.z = 1;
				B = I * e;//[x y 1 1];
				B.x = B.x/B.w;
				B.y = B.y/B.w;
				B.z = B.z/B.w;
				B.w = 1.0;
				
				D = (B - A) / (B - A).length();
				Ray ray;
				ray.origin = A;
				ray.direction = D;
				context.buffer[x + context.w * y] = castAndShade(ray);
			}
		}

	}
	
	Color castAndShade(const Ray &ray)
	{
		float tmin = FLT_MAX;
		Model *model;
		float t;
		for (std::vector<SceneNode *>::iterator it = children.begin(); it != children.end(); ++it)
		{
			//find nearest object
			if ((*it)->getModel()->findIntersection(ray, t))
			{
				if (t < tmin)
				{
					tmin = t;
					model = (*it)->getModel();
				}
			}
		}
		if (tmin != FLT_MAX)
		{
			Vertex i = ray.extrapolate(tmin);
			Vertex normal = model->getNormal(i);
			return phongModel(ray, model, i, normal);
			//return model->getMaterial().color;
		}
		return *(context.clear);
	}
	
	/**
	 Calculation of phong lighting model
	 @param ray		ray that caused the intersection
	 @param model	model with which the ray intersected
	 @param i		intersection point of the ray and the model
	 @param n		normal in the intersection point
	 */
	Color phongModel(const Ray &ray, Model *model, const Vertex &i, const Vertex &n)const
	{
		Color Ia;
		Color Id;
		Color Is;
		Material material = model->getMaterial();
		Vertex E = ray.origin + ray.direction;
		E = E*(-1);
		E = E / E.length();
		for (std::vector< Light >::const_iterator light = lights.begin(); light != lights.end(); ++light)
		{
			//ambient
			/*
			there is no material.ka !!! - we do not obtain it on sglMaterial call
			so I am skipping ambient.
			Ia.r += light->color.r * material.color.r;
			Ia.g += light->color.g * material.color.g;
			Ia.b += light->color.b * material.color.b;
			 */
			
			//diffuse
			Vertex L = light->position - i;
			L = L / L.length();
			float LN = (L * n);
			Id.r += light->color.r * material.kd * material.color.r * LN;
			Id.g += light->color.g * material.kd * material.color.g * LN;
			Id.b += light->color.b * material.kd * material.color.b * LN;
			
			//specular
			Vertex R = n*2*LN - L;
			//R = R / R.length();
			float ER = pow(E*R, material.shine);
			Is.r += light->color.r * material.ks * ER;
			Is.g += light->color.g * material.ks * ER;
			Is.b += light->color.b * material.ks * ER;
		}
		return Id + Is;
		//return model->getMaterial().color;
	}
	
	/**
	 Sets currentNode to node
	 @param node	pointer node to be added
	 */
	void beginNewNode(SceneNode *node)
	{
		this->currentNode = node;
	}
	
	/**
	 Adds currentNode to children and sets currentNode to NULL.
	 As the currentNode was pointer to allocated memory (using new), it will be freed using delete.
	 */
	void commitCurrentNode()
	{
		if (currentNode != NULL)
			children.push_back(currentNode);
		currentNode = NULL;
	}
	
	/**
	 Get the current node
	 */
	inline SceneNode * getCurrentNode()
	{
		return currentNode;
	}
	
	void setMVP(Matrix mvp)
	{
		MVP = mvp;
	}
	
};


#endif
