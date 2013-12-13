/*
 * Phong.h
 *
 *  Created on: Nov 17, 2013
 *      Author: jancajthaml
 */

#ifndef PHONG_H_
#define PHONG_H_

#include "./../helpers/Helpers.h"
#include <limits>
#include <cmath>
#include <typeinfo>

class Phong
{

private:
	Phong(){}

	static inline Color calculateColor(const Ray &ray, Model *model, const Vertex &vantage_point,const std::vector< Light* > &lights, std::vector< SceneNode* > &children, const Chunk& context)
	{
		Color color;
		if( ray.depth<=0 ) return color;

		Vertex  L;
		Vertex N = model->getNormal(vantage_point);

		//vector toward viewer
		//V = -1.0f * ray.direction;

		const Material material	= model->getMaterial();

		const int NUM_SAMPLES = 0;

		if( ray.type == RAY_PRIMARY )
		{
			float contrib = 1.0f / NUM_SAMPLES;

			for (int i = 0; i < NUM_SAMPLES; ++i)
			{
				Vertex random	= Vertex::random();
				random			= Vertex::rotate( N,random );

				Ray global_illumination_ray;
				global_illumination_ray.type		= RAY_SECONDARY;
				global_illumination_ray.direction	= random;
				global_illumination_ray.origin		= i;
				Color m = material.color;
				Color plus = (castAndShade( global_illumination_ray, children, lights, context) * material.kd * contrib) /* *m */ ;

				color.r += m.r+plus.r;
				color.g += m.g+plus.g;
				color.b += m.b+plus.b;
			}
		}

		const long size			= lights.size();
		//int pointer				= -1;
		//float lightContrib		= 1.0f/size;
		int maxSample			= 1;

		for( int pointer = 0; pointer<size; pointer++ )
		{
			Light *light = lights[pointer];

			//C++ doesnt have a instanceof

			//if(light.isPoint())printf("area light is point\n");
							//else printf("area light not point\n");
			if( !light->isPoint() )
			{
				maxSample = 8;
			}

			for( int sample = 0; sample < maxSample; sample++ )
			{

				/*if(is_point)
				{
					//static_cast<PointLight*>(light)
					//SAMPLE POINT LIGHT
				}
				else
				{
					//static_cast<AreaLight*>(light)
					//SAMPLE AREA LIGHT
				}*/

				//LIGHT DIRECTION
				//L				= light.position - vantage_point;
				Ray lightRay;
				Color lightColor;
				light->Sample(vantage_point, lightRay, lightColor);
				L = lightRay.direction;
				float length	= L.length();
				L.normalise();

				bool under_the_shadow	= false;

				//####################[SHADOWS

				//COMMENT!
				Ray shadow_ray;
				shadow_ray.origin    = vantage_point;
				shadow_ray.direction = L;

				float t     = FLOAT_MAX;

				if(0.0f < length - 0.1 && ray.depth >= 0)
				{
					for( std::vector< SceneNode* >::iterator child = children.begin(); child != children.end(); ++child )
					{
						Model* m = (*child)->getModel();
						if( m->findIntersection( shadow_ray,t ) && Helper::abs(t) > 0.01)
						{
							//calculate the intersection point that causes the shadow
							Vertex intersection = shadow_ray.extrapolate(t);
							//calculate the distance between intersection point and the point where we are going to calculate the color
							float distanceToIntersection = (intersection - vantage_point).length();
							//the intersection point that causes the shadow must be between the current point and the light.
							//if the distanceToIntersection is > length, then the point causing the shadow is behind the light and
							//therefore it cannot cause the shadow. Simple :-)
							if( distanceToIntersection<=length - 0.1 )
							{
								if( m->backfaceCull(ray, t) ) continue;
								under_the_shadow=true;
								break;
							}
						}
					}
				}

				//####################[ SHADING

				if(!under_the_shadow)
				{
					//---------------[ DIFFUSE

					color += (material.kd * material.color) * Helper::max(0.0f, N*L);
				}

				//---------------[ SPECULAR

				color += Color(material.ks, material.ks, material.ks) * powf(Helper::max(0.0f, ray.direction * Vertex::reflextionNormalised(L,N)), material.shine);
				color *= light->color;

				//####################[REFLECTION

				if( material.trn < 1.0f && material.ks > 0.0f && ray.depth >= 0 )
				{
					Ray reflected_ray;
					reflected_ray.origin     = vantage_point;
					reflected_ray.direction  = ray.direction - ( N * (ray.direction*N*2) );
					//reflected_ray.direction	 = ray.direction + 2.0f * (N*V)*N;
					reflected_ray.depth      = ray.depth-1;
					reflected_ray.type		 = RAY_SECONDARY;
					reflected_ray.direction.normalise();

					color += material.ks * castAndShade(reflected_ray,children,lights,context);
				}

				//####################[REFRACTION

				float dot		= ray.direction * N;
				float R_index	= 0.0f;

				if( dot<0.0f )
				{
					R_index = 1.0f/material.ior;
				}
				else
				{
					R_index	= material.ior;
					dot		= -dot;
					N		= -1.0f * N;
				}
				if( material.trn>0 && ray.depth>=0 )
				{
					float cosT2 = 1.0f - R_index*R_index*(1.0f-dot*dot);

					if( cosT2>0.0f )
					{
						cosT2		= dot * R_index + Helper::q3sqrt(cosT2);
						Vertex T	= -cosT2 * N + ray.direction * R_index;

						Ray refracted_ray;
						refracted_ray.origin		= vantage_point + T * 0.1f;
						refracted_ray.direction		= T;
						refracted_ray.depth			= ray.depth-1;
						refracted_ray.type			= RAY_SECONDARY;
						refracted_ray.environment	= R_index;

						refracted_ray.direction.normalise();

						color						+= material.trn * castAndShade(refracted_ray,children,lights,context);
					}
				}
			}
		}
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
#endif /* PHONG_H_ */
