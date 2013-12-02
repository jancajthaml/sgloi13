/*
 * Rim.h
 *
 *  Created on: Dec 2, 2013
 *      Author: jancajthaml
 */

#ifndef RIM_H_
#define RIM_H_
#include <cmath>

#define FLOAT_MAX std::numeric_limits<float>::max()

class Rim
{

private:
	Rim(){}

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

			float NL				= normal*light_direction;
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

			 const Color  diffColor = Color( 0.5f, 0.0f, 0.0f);
			 const Color  specColor = Color( 0.7, 0.7, 0.0 );
			 const float specPower = 30.0;
			 const float rimPower  = 8.0;
			 const float bias      = 0.3;

			    Vertex  n2   = normal;
			    Vertex  l2   = light_direction;
			    Vertex  h2   = r.origin;
			    Vertex  v2   = r.direction;
			    n2.normalise();
			    l2.normalise();
			    v2.normalise();
			    v2.normalise();
			    Color  diff = diffColor * Helper::max ( dot ( n2, l2 ), 0.0 );
			    Color  spec = specColor * pow ( Helper::max ( dot ( n2, h2 ), 0.0 ), specPower );
			    float rim  = pow ( 1.0 + bias - Helper::max ( dot ( n2, v2 ), 0.0 ), rimPower );

			    color = color + ( diff + rim * Color ( 0.5f, 0.0f, 0.2f ) + spec * specColor);
			//####################[ SHADING
/*
			if(!under_the_shadow)
			{
				//---------------[ DIFFUSE

				color = color + (material.kd * material.color) * Helper::max(0.0f, NL);
			}

			//---------------[ SPECULAR

			color = color + Color(material.ks, material.ks, material.ks) * powf(Helper::max(0.0f, ray.direction * Vertex::reflextionNormalised(light_direction, normal)), material.shine);
			color = color * lights[pointer].color;

			//####################[REFLECTION

			if (material.trn < 1.0f && material.ks > 0.0f && ray.depth >= 0)
			{
				Ray reflection_ray;
				reflection_ray.origin     = i;
				reflection_ray.direction  = ray.direction - ( normal * (ray.direction*normal*2) );
				reflection_ray.depth      = ray.depth-1;

				reflection_ray.direction.normalise();

				color = color + material.ks * castAndShade(reflection_ray,children,lights,context);
			}

			//####################[REFRACTION

			Vertex nN		= normal;
			float dot		= ray.direction * nN;
			float R_index	= 0.0f;

			if( dot<0.0f )
			{
				R_index = 1.0f/material.ior;
			}
			else
			{
				R_index	= material.ior;
				dot		= -dot;
				nN		= normal * -1.0;
			}
			if( material.trn>0 && ray.depth>=0 )
			{
				float cosT2 = 1.0f - R_index*R_index*(1.0f-dot*dot);

				if( cosT2>0.0f )
				{
					cosT2		= dot * R_index + Helper::q3sqrt(cosT2);
					Vertex T	= -cosT2 * nN + ray.direction * R_index;

					Ray refraction_ray;
					refraction_ray.origin		= i + T*0.1;
					refraction_ray.direction	= T;
					refraction_ray.depth		= ray.depth-1;
					color						= color + material.trn * castAndShade(refraction_ray,children,lights,context);
				}
			}
			*/
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


#endif /* RIM_H_ */
