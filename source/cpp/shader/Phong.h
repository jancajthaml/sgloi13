/*
 * Phong.h
 *
 *  Created on: Nov 17, 2013
 *      Author: jancajthaml
 */

#ifndef PHONG_H_
#define PHONG_H_

#include "./../helpers/Helpers.h"
struct Phong
{
	Phong()
	{}

	~Phong()
	{}

	Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &N,const std::vector< Light > &lights) const
	{
		Color color;
		Material material = model->getMaterial();

		for( std::vector< Light >::const_iterator light = lights.begin(); light != lights.end(); ++light )
		{

			//Light direction
			Vertex L = light->position - i;
			L.normalise();

			float NL = N*L;

			//---------------[ AMBIENT
			// none

			//---------------[ DIFFUSE

			Color Ld = (material.kd * material.color) * Helper::max(0.0f, NL);

			//---------------[ SPECULAR

			Color Ls = Color(material.ks, material.ks, material.ks) * powf(Helper::max(0.0f, ray.direction * Vertex::reflextionNormalised(L, N)), material.shine);

			//---------------[ RESULT

			Ld = Ld + Ls;

			//Ld = Ld * light->color;		//Uncomment if light is no longer white

			color = color + Ld;
		}

		return color;
	}


};
#endif /* PHONG_H_ */
