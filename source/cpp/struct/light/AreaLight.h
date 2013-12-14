/*
 * AreaLight.h
 *
 *  Created on: Dec 9, 2013
 *      Author: jancajthaml
 *	
 *	Changelog: 
 *		14. 12. 2013 - Jan Brejcha:
 *			- added method addVertex()
 *			- some calculations needed only once moved to prepare() method
 *			- revisited method Sample, corrected some calculation mistakes.
 *	
 */

#ifndef libsgl_AreaLightNode_h
#define libsgl_AreaLightNode_h

#include "Light.h"
#include "./../helpers/Helpers.h"
#include "../../core/FillPolygonModel.h"
#include "../../core/ContextChunk.h"

class AreaLight : public Light
{

private:
	Vertex atten;		// utlum
	Vertex normal;
	Vertex u,v;
	Vertex edge1, edge2;
	float area;
	int vertexIndex;

public:

	Vertex v0,v1,v2;			// vrcholy svetla

	AreaLight(Chunk context, const Vertex atten,const Color color )
	{
		this->model			= new FillPolygonModel(context, Material(color, 1.0, 0.0, 0.0, 0.0, 1.0));
		this->color			= color;
		this->atten			= atten;
		this->vertexIndex	= 0;
		this->area			= 0;
	}
	
	virtual void addVertex(Vertex v)
	{
		model->addVertex(v);
		vertexIndex++;
		if( vertexIndex>2 )
			prepare();
	}

	void prepare()
	{
		edge1	= Vertex(model->vertices[2] - model->vertices[0]);
		edge2	= Vertex(model->vertices[1] - model->vertices[0]);
		normal	= Vertex::cross(edge1,edge2);
		normal.normalise();
		area	= (edge1.length() * edge2.length() * 0.5) / float(MAX_SAMPLES);
	}

	void Sample( const Vertex& point, Ray& ray, Color& contribution, const float u = 0, const float v = 0)
	{
		ray.origin		= model->vertices[0] + edge1*u + edge2*v;
		ray.direction	= ray.origin - point;

		float d			= ray.direction.length();

		//RAY Direction MUST NOT be normalized, because we need its length in phong
		Vertex ray_direction = ray.direction;
		ray_direction.normalise();
		
		contribution = color * ( ray_direction * normal * area/(atten.x + atten.y*d + atten.z*d*d));
	}

	virtual Color getLightColor() const
	{ return color; }

	Color getIntensity() const
	{ return color; }
	
	virtual bool isAreaLight()
	{ return true; }

};

#endif
