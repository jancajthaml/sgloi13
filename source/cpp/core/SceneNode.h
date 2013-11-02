//
//  SceneNode.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_SceneNode_h
#define libsgl_SceneNode_h

#include <vector>
#include "../struct/Matrix.h"
#include "../struct/Light.h"
#include "Model.h"
/** 
 Basic SceneGraph Node, other elements except Lights extends this one.
 */
class SceneNode
{
protected:
	///model that is held by this SceneNode
	Model * model;
	
	///children nodes of this scene node
	std::vector< SceneNode* > children;
	
	///model view projection matrix for this scene node
	Matrix MVP;
	
public:
	
	SceneNode()
	{
		model = NULL;
	}
	
	~SceneNode()
	{
		if (model != NULL)
			delete model;
	}
	
	//TODO copy assignment operator, to support RULE OF THREE.
	
	/**
	 Constructor of the SceneNode
	 @param _model	model that will be held by this SceneNode
	 @param _mvp	Model View Projection matrix for this model
	 */
	SceneNode(Model * _model, Matrix _mvp)
	{
		this->model = _model;
		this->MVP = _mvp;
	}
	
	/**
	 Rasterizes this node
	 @param lights	lights affecting this node.
	 */
	virtual void rasterize(std::vector< Light > lights)
	{
		model->rasterize(lights, this->MVP);
		for (std::vector< SceneNode* >::iterator it = children.begin(); it != children.end(); ++it)
			(*it)->rasterize(lights);
	}
	
	
	/**
	 Adds child to children
	 @param child	the child to be added
	 */
	inline void addChild(SceneNode * child)
	{
		children.push_back(child);
	}
	
	
	inline void addVertex(Vertex v)
	{
		//TODO multiply or not multiply now?
		Vertex v2 = MVP * v;
		v2.x/=v2.w;
		v2.y/=v2.w;
		v2.z/=v2.w;
		
		//v2.print();
		model->addVertex(v2);
	}
	
	
	
	
};


#endif
