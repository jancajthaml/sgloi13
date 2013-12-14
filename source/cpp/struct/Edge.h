/*
 * Edge.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef EDGE_H_
#define EDGE_H_


struct Edge
{
	float min_y;
	float max_y;
	float delta_x;
	float delta_z;
	float x;
	float z;

	Edge()
	{
		min_y      = 0.0f;
		max_y      = 0.0f;
		delta_x    = 0.0f;
		delta_z    = 0.0f;
		x   	   = 0.0f;
		z          = 0.0f;
	};

};

#endif /* EDGE_H_ */
