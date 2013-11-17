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

//TODO COMMENT !!!!!!!
struct Helper
{

	//FIXME this is a example of a HELPER ... move to Helpers.h
	static inline int_fast32_t round(float x)
	{ return ((x>=0.5f)?(int_fast32_t(x)+1):int_fast32_t(x)); }

	static void sort(float *a, int n)
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

	static inline float max(float a, float b)
	{ return a>b?a:b; }

	static inline float min(float a, float b)
	{ return a<b?a:b; }

	static inline float abs(float a)
	{ return a<0?-a:a; }

	//Magical square root from Quake III engine
	static inline float q3sqrt(float x)
	{
		union
		{
			int i;
			float x;
		} u;

		u.x = x;
		u.i = (1<<29) + (u.i >> 1) - (1<<22);

		u.x =       u.x + x/u.x;
		u.x = 0.25f*u.x + x/u.x;

		return u.x;
	}

};

#endif /* HELPERS_H_ */
