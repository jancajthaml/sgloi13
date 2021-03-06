/*
 * Toon.h
 *
 *  Created on: Dec 2, 2013
 *      Author: jancajthaml
 */

#ifndef TOON_H_
#define TOON_H_

#include "./../helpers/Helpers.h"
#include <limits>

class Toon
{

private:
	Toon(){}

	static inline Color calculateColor(const Ray &ray, Model *model, const Vertex &i,const std::vector< Light* > &lights, std::vector< SceneNode* > &children, const Chunk& context)
	{
		Vertex normal			= model->getNormal(i);
		Color color;

		return color;
	}

public:
	static inline Color castAndShade(const Ray &ray,std::vector< SceneNode* > &children,const std::vector< Light* > &lights, const Chunk &context)
	{
		Model *model;
		float tmin	= FLOAT_MAX;
		float t		= FLOAT_MAX;

		for( std::vector< SceneNode* >::iterator child = children.begin(); child != children.end(); ++child )
		{
			Model* m = (*child)->getModel();

			if( m->findIntersection(ray, t) && !m->backfaceCull(ray, t) && t > 0.1 && t<tmin)
			{
				tmin  = t;
				model = m;
			}
		}

		return ( tmin<FLOAT_MAX ) ? calculateColor(ray, model, ray.extrapolate(tmin), lights, children, context) : *context.clear;

	}

};


#endif /* TOON_H_ */
