/*
 * Primitive.h
 *
 *  Created on: 27.10.2013
 *      Author: jancajthaml
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "../Material.h"

class Primitive
{

	public:
		Primitive(){}

		Material material;

		virtual Vertex getNormal(const Vertex &v) const = 0;

	private:
		~Primitive(){}
};


#endif /* PRIMITIVE_H_ */
