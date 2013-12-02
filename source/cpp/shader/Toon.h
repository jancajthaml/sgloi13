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

	static inline Color calculateColor(const Ray &ray, Model *model, const Vertex &i,const std::vector< Light > &lights, std::vector< SceneNode* > &children, const Chunk& context)
	{
		Vertex normal			= model->getNormal(i);
		Color color;
		const Material material	= model->getMaterial();
		const long size			= lights.size();
		int pointer				= -1;

		//####################[DEPTH OF FIELD

		while( ++pointer<size )
		{
			Vertex light_direction		= lights[pointer].position - i;
			float length				= light_direction.length();

			light_direction.normalise();

			float NL				= Helper::max(0.0f,(normal*light_direction));
			bool under_the_shadow	= false;

			//####################[SHADOWS


			//COMMENT!
			Ray r;
			r.origin    = i;
			r.direction = light_direction;

			float t         = FLOAT_MAX;
			if(0.0f < length - 0.1 && ray.depth >= 0)
			{
				for( std::vector< SceneNode* >::iterator child = children.begin(); child != children.end(); ++child )
				{
					Model* m = (*child)->getModel();
					if( m->findIntersection(r, t) && Helper::abs(t) > 0.01)
					{
						//calculate the intersection point that causes the shadow
						Vertex intersection = r.extrapolate(t);
						//calculate the distance between intersection point and the point where we are going to calculate the color
						float distanceToIntersection = (intersection - i).length();
						//the intersection point that causes the shadow must be between the current point and the light.
						//if the distanceToIntersection is > length, then the point causing the shadow is behind the light and
						//therefore it cannot cause the shadow. Simple :-)
						if( distanceToIntersection<=length - 0.1 )
						{
							if(m->backfaceCull(ray, t)) continue;
							under_the_shadow=true;
							break;
						}
					}
				}
			}

			//####################[ SHADING

			if(!under_the_shadow)
			{
				//---------------[ DIFFUSE COLOR DISCRETIZATION
				float I_diff = material.kd * NL;

				if ( I_diff < 0.15f )		color = color + (material.color * 0.3f);
				else if ( I_diff < 0.35f )	color = color + (material.color * 0.5f);
				else if ( I_diff < 0.5f )	color = color + (material.color * 0.7f);
				else if ( I_diff < 0.7f )	color = color + (material.color);
				else						color = color + (material.color * 1.2f);
			}
			else color = color + *context.clear;

		}
		return color;
	}

public:
	static inline Color castAndShade(const Ray &ray,std::vector< SceneNode* > &children,const std::vector< Light > &lights, const Chunk &context)
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
