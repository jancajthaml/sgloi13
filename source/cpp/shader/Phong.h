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

		const Material material = model->getMaterial();
		const int size			= lights.size();
		int off					= -1;

		while( ++off<size)
		{

			//Light direction
			Vertex L = lights[off].position - i;
			L.normalise();

			float NL = N*L;

			//---------------[ AMBIENT
			// none

			//---------------[ DIFFUSE

			Color Ld = (material.kd * material.color) * Helper::max(0.0f, NL);

			//---------------[ SPECULAR

			const Color Ls = Color(material.ks, material.ks, material.ks) * powf(Helper::max(0.0f, ray.direction * Vertex::reflextionNormalised(L, N)), material.shine);

			//---------------[ RESULT

			Ld = Ld + Ls;
			Ld = Ld * lights[off].color;

			color = color + Ld;
		}

		return color;
	}


};
#endif /* PHONG_H_ */
