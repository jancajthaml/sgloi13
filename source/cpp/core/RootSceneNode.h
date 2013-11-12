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
		for (uint_fast16_t y = 0; y < context.w; y++)
		{
			for (uint_fast16_t x = 0; x < context.w; x++)
			{
				e.x = x;
				e.y = y;
				e.z = -1;
				e.w = 1;
				A = I * e;//I * [x y -1 1];
				e.z = 1;
				B = I * e;//[x y 1 1];
				D = (B - A) / (B - A).length();
				Ray ray;
				ray.origin = A;
				ray.direction = D;
				context.buffer[x + context.w * y] = castAndShade(ray);
			}
		}

	}
	
	Color castAndShade(Ray ray)
	{
		return Color(1.0, 0.6, 0.3);
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
	
};


#endif
