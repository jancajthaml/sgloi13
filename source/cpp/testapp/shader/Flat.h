/*
 * FlatShader.h
 *
 *  Created on: Nov 17, 2013
 *      Author: jancajthaml
 */

#ifndef FLATSHADER_H_
#define FLATSHADER_H_

class Flat
{

private:
	Flat(){}

public:
	static inline Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &N,const std::vector< Light > &lights)
	{ return model->getMaterial().color; }

};

#endif /* FLATSHADER_H_ */
