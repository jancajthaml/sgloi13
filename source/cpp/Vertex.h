/*
 * Vertex.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#ifndef LIBRARIES
#define LIBRARIES
#include "CrossReferenceDispatcher.h"
#endif

//TODO COMMENT !!!!!!!
struct Vertex
{
	float x;
	float y;
	float z;
	float w;

	Vertex()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}

	Vertex(float X, float Y)
	{
		x = X;
		y = Y;
		z = 0.0f;
		w = 1.0f;
	}

	Vertex(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
		w = 1.0f;
	}

	Vertex(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
};

#endif /* VERTEX_H_ */
