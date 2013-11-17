/*
 * FlatShader.h
 *
 *  Created on: Nov 17, 2013
 *      Author: jancajthaml
 */

#ifndef FLATSHADER_H_
#define FLATSHADER_H_

struct Flat
{

	Flat()
	{}

	~Flat()
	{}

	Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &normal,const std::vector< Light > &lights) const
	{
		Material material = model->getMaterial();

		return material.color;
	}

};

#endif /* FLATSHADER_H_ */
