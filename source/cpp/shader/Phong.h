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

	Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &normal,const std::vector< Light > &lights) const
	{
		Color color;
		Material material = model->getMaterial();

		// implementovan� pod�a vzorca v http://cs.nyu.edu/~elif/phong.pdf

		// Zo zadania:
		//Je li z NFF na�teno: f R G B Kd Ks Shine T index_of_refraction
		//pak rgb slo�ky koeficientu dif�zn�ho odrazu pro Phong�v model jsou [R*Kd G*Kd B*Kd]
		//a rgb slo�ky koeficientu zrcadlov�ho odrazu pro Phong�v model jsou [Ks Ks Ks] .
		//(Jin�mi slovy: odlesky maj� v�dy b�lou barvu, nez�visle na dif�zn� barv� objektu).


		for( std::vector< Light >::const_iterator light = lights.begin(); light != lights.end(); ++light )
		{

			Vertex lightPosition = light->position;
			Vertex lightDirection = lightPosition - ray.origin;
			lightDirection.normalise();

			float NL = normal * lightDirection;

			Color Ld = (material.kd * material.color) * Helper::max(0.0f, NL);
			Color Ls = Color(material.ks, material.ks, material.ks) * powf(Helper::max(0.0f, ray.direction * Vertex::reflextionNormalised(lightDirection, normal)), material.shine);

			Ld = Ld + Ls;
			Ld = Ld * light->color;

			color = color + Ld;
		}

		return color;
	}


};
#endif /* PHONG_H_ */
