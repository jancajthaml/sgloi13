#ifndef VERTEX_H
#define VERTEX_H

/** Vertex struct holds information about vertices
 * \author Jan Brejcha
 */
struct Vertex
{
	float x, y, z, w;

	Vertex()
	{
		x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f;
	}

	Vertex(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

#endif
