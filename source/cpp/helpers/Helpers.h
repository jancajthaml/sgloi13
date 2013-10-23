/*
 * Helpers.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include "./../struct/Vertex.h"
#include "./../struct/Edge.h"
#include "./../struct/VertexStack.h"

//TODO COMMENT !!!!!!!
struct Helper
{

	//FIXME this is a example of a HELPER ... move to Helpers.h
	static inline int_fast32_t round(float x)
	{ return ((x>=0.5f)?(int_fast32_t(x)+1):int_fast32_t(x)); }



	static inline void swap(float &x,float &y)
	{
	  float tmp=x;
	  x=y;
	  y=tmp;
	}


	static inline void swap(int &x,int &y)
	{
	  int tmp=x;
	  x=y;
	  y=tmp;
	}

	static inline void swap(short &x,short &y)
	{
	  x ^= y;
	  y ^= x;
	  x ^= y;
	}
};

#endif /* HELPERS_H_ */
