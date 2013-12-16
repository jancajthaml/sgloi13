/*
 * Helpers.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <cstdlib>
#include "./../struct/Edge.h"
#include "./../struct/Color.h"
#include "../core/types.h"
#include "./../struct/Vertex.h"
#include <cmath>
#define MAX_SAMPLES 16

//TODO COMMENT !!!!!!!
struct Helper
{

	//FIXME this is a example of a HELPER ... move to Helpers.h
	static inline int32 round(float x)
	{ return ((x>=0.5f)?(int32(x)+1):int32(x)); }

	static inline float acos(float x)
	{
		float xa = fabsf(x);
		float t  = ( xa>0.5625f)?(2.0f * asinf ( q3sqrt(0.5f * (1.0f - xa)))):(1.5707963267948966f - asinf (xa));
		return ( x<0.0f ) ? (3.1415926535897932 - t) : t;
	}

	static inline float random()
	{ return float(rand()) * 4.6566129e-10; }


	static inline float asinf( float x )
	{
		float x2	= x * x;
		float x4	= x2 * x2;
		float x8	= x4 * x4;
		return (((4.5334220547132049e-2 * x2 - 1.1226216762576600e-2) * x4 + (2.6334281471361822e-2 * x2 + 2.0596336163223834e-2)) * x8 + (3.0582043602875735e-2 * x2 + 4.4630538556294605e-2) * x4 + (7.5000364034134126e-2 * x2 + 1.6666666300567365e-1)) * x2 * x + x;
	}

	static void sort(float *a, int32 n)
	{
		int32 i = 1;
		int32 j = 2;
		float t = 0.0f;

		while( i<n )
		{
			if( a[i - 1] > a[i] )
			{
				t = a[i];
				a[i] = a[i-1];
				a[i-1] = t;

				if( --i )continue;
			}
			i = j++;
		}
	}


	static void sort(float* &a, float* &b, int32 n)
	{
		float m = 0.0f;
		int32 i = 1;
		int32 j = 2;
		float t = 0.0f;

		while( i<n )
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
	{ return (A<B)?((A<C)?int32(A):int32(C)):int32(B); }

	static inline int max(float A, float B, float C)
	{ return (A>B)?((A>C)?int32(A):int32(C)):int32(B); }

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
			int32 i;
			float x;
		} u;

		u.x = x;
		u.i = (1<<29) + (u.i >> 1) - (1<<22);

		u.x =       u.x + x/u.x;
		u.x = 0.25f*u.x + x/u.x;

		return u.x;
	}


	static inline bool areColorsSimilar(const Color x1,const Color x2)
	{
		float match= 0.15f;

		if(x1.r+match>x2.r && x1.r<x2.r+match)
			if(x1.g+match>x2.g && x1.g<x2.g+match)
				if(x1.b+match>x2.b && x1.b<x2.b+match)
					return true;
		return false;
	}

	static inline bool areColorsSame(const Color x1,const Color x2, const Color x3, const Color x4)
	{
		if(x1.r==x2.r==x3.r==x4.r)
			if(x1.g==x2.g==x3.g==x4.g)
				if(x1.b==x2.b==x3.b==x4.b) return true;
		return false;
	}
};

#endif /* HELPERS_H_ */
