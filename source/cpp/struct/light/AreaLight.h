/*
 * AreaLight.h
 *
 *  Created on: Dec 9, 2013
 *      Author: jancajthaml
 */

#ifndef libsgl_AreaLightNode_h
#define libsgl_AreaLightNode_h

#include "Light.h"

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

	AreaLight( const Vertex atten,const Color color )
	{
		this->color = color;
		this->atten = atten;
		vertexIndex = 0;
	}
	
	virtual void addVertex(Vertex v)
	{
		switch (vertexIndex)
		{
			case 0: v0 = v; break;
			case 1: v1 = v; break;
			case 2:
			{
				v2 = v;
				prepare();
			}; break;
		}
		vertexIndex++;
	}

	void prepare()
	{
		edge1	= Vertex(this->v2 - this->v1);
		edge2	= Vertex(this->v0 - this->v1);
		Vertex normal	= Vertex::cross(edge1,edge2);
		area			= normal.length();
		normal.normalise();
	}

	void Sample( const Vertex& point, Ray& ray, Color& contribution, const float u = 0, const float v = 0)
	{
		ray.origin		= v1 + edge1*u + edge2*v;
		ray.direction	= point - ray.origin;

		float d			= ray.direction.length();
		//ray.direction.normalise();
		contribution	= color * (1.0f/(atten.x + atten.y*d + atten.z*d*d));
		contribution	= contribution * ((ray.direction*normal)*area);
	}

	virtual Color getLightColor() const
	{ return color; }

	bool shadow(const Ray& aRay) const
	{
		return false;
	}

	bool isPoint()
	{ return false; }

	Color getIntensity() const
	{ return color; }

};

#endif
