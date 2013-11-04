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

	static void sort(float* &a, int n)
	{
		int i = 1;
		int j = 2;
		float t = 0.0f;

		while( i<n )
		{
			if( a[i - 1] > a[i] )
			{
				t = a[i];
				a[i] = a[i-1];
				a[i-1] = t;

				if(--i)continue;
			}
			i = j++;
		}
	}


	static void sort(float* &a, float* &b, int n)
	{
		float m = 0.0f;
		int i = 1;
		int j = 2;
		float t = 0.0f;

		while(i < n)
		{
			if( a[i - 1] > a[i] )
			{
				t = a[i];
				m = b[i];

				a[i] = a[i-1];
				b[i] = b[i-1];

				a[i-1] = t;
				b[i-1] = m;
				if(--i)continue;
			}
			i = j++;
		}
	}

	static inline int min(float A, float B, float C)
	{ return (A<B)?((A<C)?int(A):int(C)):int(B); }

	static inline int max(float A, float B, float C)
	{ return (A>B)?((A>C)?int(A):int(C)):int(B); }


};

#endif /* HELPERS_H_ */