/*
 * Vertex.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef VERTEX_H_
#define VERTEX_H_

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
	inline float length()
	{
		if( w==0.0f ) return 0.0f;
		if( w==1.0f ) return sqrtf( x*x + y*y + z*z);

		float v = 1.0f/w;
		float x1 = x*v;
		float y1 = y*v;
		float z1 = z*v;
		return sqrtf( x1*x1 + y1*y1 + z1*z1 );
	}
	
	


	void normalise()
	{
		float l = 1.0f/length();
		x *= l;
		y *= l;
		z *= l;
	}

	//Ray helper functions

	static Vertex crossNormalised(const Vertex &a, const Vertex &b)
	{
		Vertex result;
		result.x = a.y*b.z - a.z*b.y;
		result.y = a.z*b.x - a.x*b.z;
		result.z = a.x*b.y - a.y*b.x;

		result.normalise();
		return result;
	}

	/// Skalárny súèin vektorov
	friend float operator*(const Vertex& v1,const Vertex& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	/// Odèítanie vektorov
	friend Vertex operator-(const Vertex& v1,const Vertex& v2) {
		return Vertex(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
	}

	/// Sèítanie vektorov
	friend Vertex operator+(const Vertex& v1,const Vertex& v2) {
		return Vertex(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
	}

	Vertex crossProduct(const Vertex& v2)
	{
		Vertex result;

		result.x = y*v2.z - z*v2.y;
		result.y = z*v2.x - x*v2.z;
		result.z = x*v2.y - y*v2.x;

		return result;
	}

	/// Súèín èísla s vektorom
	friend Vertex operator*(float f,const Vertex& v) {
		return Vertex(v.x * f,v.y * f,v.z * f);
	}
	friend Vertex operator/(const Vertex& v,float f) {
			return Vertex(v.x / f,v.y / f,v.z / f);
	}

	/// Súèín èísla s vektorom
	friend Vertex operator*(const Vertex& v, float f) {
		return Vertex(v.x * f,v.y * f,v.z * f);
	}
	static Vertex reflextionNormalised(const Vertex &a, const Vertex &b)
	{
		Vertex result;
		result = a - 2.0f*(a*b) * b;
		result.normalise();
		return result;
	}
};

#endif /* VERTEX_H_ */
