/*
 * data.h
 *
 *  Created on: 23.9.2013
 *      Author: jancajthaml
 */

#include <vector>
#include <list>
#ifndef DATA_H_
#define DATA_H_

/*
 * Side-notes:
 * Jan Cajthaml -using struct instead of class because of no need of encapsulation
 *
 * */

/*
 * ChangeLog:
 * 23.9.2013, Jan Cajthaml - added struct Vertex
 * 23.9.2013, Jan Cajthaml - added struct Edge
 *
 * */

//Vertex
struct Vertex
{
	float x;
	float y;
	float z;
	float w;

	Vertex()
	{ x = y = z = w = 0.0f; }

	Vertex(float X, float Y)
	{
		x = X;
		y = Y;
		z = w = 0.0f;
	}

	Vertex(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
		w = 0.0f;
	}

	Vertex(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
};

//Edge
struct Edge
{
	float	x_s;
	float	y_s;
	float	z_s;
	float	x_e;
	float	y_e;
	float	z_e;

    Edge()
    { x_s = y_s = z_s = x_e = y_e = z_e = 0.0f; }

    Edge(Vertex start, Vertex end)
	{
    	x_s	= start.x;
    	y_s	= start.y;
    	z_s	= start.z;
    	x_e	= end.x;
    	y_e	= end.y;
    	z_e	= end.z;
	}

};
#endif /* DATA_H_ */
