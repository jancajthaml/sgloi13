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

#define FLOAT_MAX std::numeric_limits<float>::max()

#define REFLECTION
#define REFRACTION
#define SHADOWS

class Phong
{

private:
	Phong(){}

	static inline Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &N,const std::vector< Light > &lights, std::vector< SceneNode* > &children, const Chunk& context)
	{

		Color color;

		const Material material = model->getMaterial();
		const int size = lights.size();
		int pointer = -1;

		//####################[DEPTH OF FIELD


		while( ++pointer<size )
	    {
			//Light direction
			Vertex L = lights[pointer].position - i;
			L.normalise();

			float NL = N*L;
			bool under_the_shadow = false;

			//####################[SHADOWS
			#ifdef SHADOWS
			float length= L.length();

			Ray r;
			r.origin=i;
			r.direction=L;

			float t	 = FLOAT_MAX;
			if (0.0f < length - 0.1)
			{
				for( std::vector< SceneNode* >::iterator child = children.begin(); child != children.end(); ++child )
				{
					Model* m = (*child)->getModel();
					if( m->findIntersection(r, t) && t > 0.01)
					{
						under_the_shadow=true;
						break;
					}
				}
			}
			#endif

			//####################[ SHADING

			if(!under_the_shadow)
			{
				//---------------[ DIFFUSE

				Color Ld = (material.kd * material.color) * Helper::max(0.0f, NL);

				//---------------[ SPECULAR

				const Color Ls = Color(material.ks, material.ks, material.ks) * powf(Helper::max(0.0f, ray.direction * Vertex::reflextionNormalised(L, N)), material.shine);

				//---------------[ RESULT

				Ld = Ld + Ls;
				Ld = Ld * lights[pointer].color;

				color = color + Ld;
			}

			//####################[REFLECTION
			#ifdef REFLECTION
			if (material.ior >= 0.0f && material.trn < 1.0f && material.ks > 0.0f && ray.depth >= 0)
			{
				
				Vertex R = ray.direction - ( N * (ray.direction*N*2) );
				R.normalise();

				Ray ray_2;
				ray_2.origin	= i;
				ray_2.direction	= R;
				ray_2.depth		= ray.depth-1;

				color = color + material.ior * castAndShade(ray_2,children,lights,context);
			}
			#endif

			//####################[REFRACTION
			#ifdef REFRACTION
			//idealni lom
			float trans		= material.trn;
			float R_index = 1.0f/material.ior;
			if( R_index != 0 && trans > 0 && ray.depth >= 0)
			{
				Vertex M = Vertex::cross(N,L);
				M.normalise();

				float cosI =  - (M* ray.direction );

				float cosT2 = 1.0f - R_index*R_index*(1.0f-cosI*cosI);

				if( cosT2>0.0f )
				{
					Vertex T = (ray.direction * R_index) + (M *(R_index * cosI - Helper::q3sqrt( cosT2 ))) ;
					T.normalise();
					Color barv( 0, 0, 0 );
					Vertex O = i + (T * 0.1f);
					Ray r_r;
					r_r.origin		= O;
					r_r.direction	= T;
					r_r.depth		= ray.depth-1;
					color = color + castAndShade(r_r,children,lights,context);
				}
			}
			#endif
		}

		return color;
	}

public:
	static inline Color castAndShade(const Ray &ray,std::vector< SceneNode* > &children,const std::vector< Light > &lights, const Chunk &context)
	{
		float tmin = FLOAT_MAX;
		Model *model;
		float t	 = FLOAT_MAX;

		for( std::vector< SceneNode* >::iterator child = children.begin(); child != children.end(); ++child )
		{
			Model* m = (*child)->getModel();
			//find nearest object
			if( m->findIntersection(ray, t) && t > 0.1 )
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

			return calculateColor(ray, model, i, normal, lights, children, context);
		}
		return *context.clear;
	}

};
#endif /* PHONG_H_ */
