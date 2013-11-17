/*
 * Sphere.h
 *
 *  Created on: 27.10.2013
 *      Author: jancajthaml
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Primitive.h"

class Sphere : public Primitive
{
	public:
		Sphere(Vertex center, float radius, Material *mat)
		{ material = *mat; }

		Sphere(float x, float y, float z, float radius, Material* mat)
		{ material = *mat; }

		virtual Vertex getNormal(const Vertex &v) const
		{ return Vertex( 0.0f, 0.0f ); }

	private:
		~Sphere(){}
};


#endif /* SPHERE_H_ */
