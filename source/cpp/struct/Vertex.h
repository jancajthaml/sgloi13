/*
 * Vertex.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "./../helpers/Helpers.h"
#include <cmath>
//#include <CoreServices/CoreServices.h>

//TODO COMMENT !!!!!!!
//FIXME COMMEEEEEEENT!!
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

	Vertex(Vertex p1, Vertex p2)
	{
		x = p2.x - p1.x;
		y = p2.y - p1.y;
		z = p2.z - p1.z;
		w = 1.0f;
	}

	Vertex(const float aVal)
	{
		x = y = z = aVal;
		w= 1.0f;
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
	
	void print() const
	{ printf("[%f, %f, %f, %f]\n", x, y, z, w); }
	
	//FIXME use fast square root
	inline float length()const
	{
		if( w==0.0f ) return 0.0f;
		if( w==1.0f ) return Helper::q3sqrt( x*x + y*y + z*z);

		float v = 1.0f/w;
		float x1 = x*v;
		float y1 = y*v;
		float z1 = z*v;
		return Helper::q3sqrt( x1*x1 + y1*y1 + z1*z1 );
	}

	void normalise()
	{
		float l = 1.0f/length();
		x *= l;
		y *= l;
		z *= l;
	}

	/// Skalární součin vektorů
	friend float operator*(const Vertex& v1,const Vertex& v2)
	{
		//__m128 a, b, c;
		//a = _mm_set_ps(v1.x, v1.y, v1.z, 0.0);
		//b = _mm_set_ps(v2.x, v2.y, v2.z, 0.0);
		//c = _mm_mul_ps(a, b);
		//float f[4];
 	 	// _mm_store_ps(f, c);
		//return f[3]+ f[2] + f[1];
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	/// Rozdíl vektorů
	friend Vertex operator-(const Vertex& v1,const Vertex& v2)
	{ return Vertex(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z); }

	/// Rozdíl vektorů
	void operator-=(const Vertex& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	/// Součet vektorů
	friend Vertex operator+(const Vertex& v1,const Vertex& v2)
	{ return Vertex(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); }

	void operator+=(const Vertex& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	/// Součin čísla a vektoru
	friend Vertex operator*(float f,const Vertex& v)
	{ return Vertex(v.x * f,v.y * f,v.z * f); }

	/// Součin čísla a vektoru
	void operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
	}

	/// Podíl čísla a vektoru
	friend Vertex operator/(const Vertex& v,float f)
	{ return v*(1.0f/f); }

	/// Podíl čísla a vektoru
	void operator/=(float f)
	{
		f = 1.0f/f;
		x *= f;
		y *= f;
		z *= f;
	}

	/// Součin čísla a vektoru
	friend Vertex operator*(const Vertex& v, float f)
	{ return Vertex(v.x * f,v.y * f,v.z * f); }

	static Vertex cross(const Vertex &a, const Vertex &b)
	{ return Vertex( a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x ); }

	static Vertex reflextionNormalised(const Vertex &a, const Vertex &b)
	{
		Vertex result;
		result = a - 2.0f*(a*b) * b;
		result.normalise();
		return result;
	}


	static inline Vertex random()
	{
		RandomPair random;

		return Vertex (
			cosf(random.value2)*Helper::q3sqrt(1-random.value1),
			sinf(random.value2)*Helper::q3sqrt(1-random.value1),
			Helper::q3sqrt(random.value1));
	};

	static inline Vertex rotate(const Vertex& N, const Vertex& vector)
	{
		Vertex cX, cY, cZ, n, m;
		Vertex temp = N;
		cZ = N;
		temp.normalise();
		cY.normalise();

		n = Vertex(1.0f, 0.0f, 0.0f);
		m = Vertex(0.0f, 1.0f, 0.0f);

		cX = Vertex::cross(cZ, n);
		if (cX.length() < 0.01f)
			 cX = Vertex::cross(cZ, m);

		cY = Vertex::cross(cZ, cX);

		temp = (vector.x * cX + vector.y * cY + vector.z * cZ);
		return temp;
	}
};

#endif /* VERTEX_H_ */
