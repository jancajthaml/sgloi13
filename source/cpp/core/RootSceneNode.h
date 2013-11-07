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
		//todo raytrace
		
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
