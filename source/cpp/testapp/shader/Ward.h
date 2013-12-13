/*
 * Ward.h
 *
 *  Created on: Nov 17, 2013
 *      Author: jancajthaml
 */

#ifndef WARD_H_
#define WARD_H_

const float PI4 = 12.5663706144;

#include "./../helpers/Helpers.h"

class Ward
{
private:
	Ward(){}

	static inline Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &N,const std::vector< Light > &lights, std::vector< SceneNode* > &children, const Color& clear)
	{
		Material material = model->getMaterial();
		const int size = lights.size();
		int pointer		= -1;
		Color color;

		return color;
	}

public:
	static inline Color castAndShade(const Ray &ray,std::vector< SceneNode* > &children,const std::vector< Light > &lights, const Color &clear)
	{
			float tmin = FLOAT_MAX;
			Model *model;
			float t	 = FLOAT_MAX;

			for( std::vector< SceneNode* >::iterator child = children.begin(); child != children.end(); ++child )
			{
				Model* m = (*child)->getModel();
				//find nearest object
				if( m->findIntersection(ray, t) )
				{
					if( t<tmin )
					{
						tmin  = t;
						model = m;
					}
				}
			}
			if( tmin<FLOAT_MAX )
			{
				Vertex i		= ray.extrapolate(tmin);
				Vertex normal	= model->getNormal(i);

				return calculateColor(ray, model, i, normal, lights, children, clear);
			}
			return clear;
		}

};

#endif /* WARD_H_ */
