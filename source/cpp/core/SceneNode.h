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
#include "Model.h"

class Light;

/** 
 Basic SceneGraph Node, other elements except Lights extends this one.
 */
class SceneNode
{
protected:
	///model that is held by this SceneNode
	Model * model;
	
	///children nodes of this scene node
	//FIXME std::vector really slow
	std::vector< SceneNode* > children;
	
	///model view projection matrix for this scene node
	

public:
	Matrix MVP;

	SceneNode()
	{ model = NULL; }
	
	virtual ~SceneNode()
	{
		if( model!=NULL ) delete model;

		uint8 i				= -1;
		const uint8 size	= children.size();

		while( ++i<size )
		   delete children[i];

	}
	
	//TODO copy assignment operator, to support RULE OF THREE.
	
	/**
	 Constructor of the SceneNode
	 @param _model	model that will be held by this SceneNode
	 @param _mvp	Model View Projection matrix for this model
	 */
	SceneNode( Model* _model, Matrix _mvp )
	{
		this->model = _model;
		this->MVP   = _mvp;
	}
	
	/**
	 Rasterizes this node
	 @param lights	lights affecting this node.
	 */
	virtual void rasterize( std::vector< Light* > lights )
	{
		model->rasterize(lights, this->MVP);

		for( std::vector< SceneNode* >::iterator iter = children.begin(); iter != children.end(); ++iter )
			(*iter)->rasterize(lights);

		 children.clear();
	}

	/**
	 Adds child to children
	 @param child	the child to be added
	 */
	void addChild(SceneNode * child)
	{ children.push_back(child); }
	
	virtual void addVertex(Vertex v)
	{ model->addVertex(v); }
	
	Model* getModel()
	{ return model; }

};

#endif
