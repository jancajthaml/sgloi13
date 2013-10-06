#ifndef VERTEX_H
#define VERTEX_H

#include <cstdio>
#define VERTEX4_LENGTH 4
/** Vertex struct holds information about vertices
 * \author Jan Brejcha
 */
struct Vertex
{
	float v[VERTEX4_LENGTH]; 

	Vertex()
	{
		v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f; v[3] = 0.0f;
	}

	Vertex(float x, float y)
	{
		v[0] = x; v[1] = y; v[2] = 0.0f; v[3] = 0.0f;
	}
	Vertex(float x, float y, float z, float w)
	{
		v[0] = x; v[1] = y; v[2] = z; v[3] = w;
	}

	void print()
	{
		printf("%f, %f, %f, %f\n", v[0], v[1], v[2], v[3]);
	}
};

#endif
