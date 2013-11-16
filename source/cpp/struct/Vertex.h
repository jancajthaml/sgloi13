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
		if( w==1.0f ) return sqrtf( x*x + y*y + z*z);

		float v = 1.0f/w;
		float x1 = x*v;
		float y1 = y*v;
		float z1 = z*v;
		return sqrtf( x1*x1 + y1*y1 + z1*z1 );
	}
	
	inline Vertex operator-(const Vertex &other) const
	{
		Vertex res;
		if (w == 1.0)
		{
			res.x = x - other.x;
			res.y = y - other.y;
			res.z = z - other.z;
			res.w = 1.0f;
			return res;
		}
		float v = 1.0f/w;

		res.x = (x*v) - (other.x*v);
		res.y = (y*v) - (other.y*v);
		res.z = (z*v) - (other.z*v);
		res.w = 1.0f;
		return res;
	}
	
	inline Vertex operator/(const float &rhs) const
	{
		Vertex res;
		if( w==1.0f )
		{
			float r = 1.0f/rhs;

			res.x = x * r;
			res.y = y * r;
			res.z = z * r;
			res.w = 1.0f;
			return res;
		}
		else
		{
			float r = 1.0f/rhs;
			float v = 1.0f/w;

			res.x = (x*v) * r;
			res.y = (y*v) * r;
			res.z = (z*v) * r;
			res.w = 1.0f;
			return res;
		}
	}
	
	
	inline Vertex operator*(const float &rhs) const
	{

		Vertex res;
		if (w == 1.0)
		{
			res.x = x * rhs;
			res.y = y * rhs;
			res.z = z * rhs;
			res.w = 1.0f;
			return res;
		}
		else
		{
			float v = 1.0f/w;

			res.x = x*v * rhs;
			res.y = y*v * rhs;
			res.z = z*v * rhs;
			res.w = 1.0f;
			return res;
		}
	}
	
	inline float operator*(const Vertex &rhs) const
	{
		if( w==1.0f )
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}
		else
		{
			//FIXME
			float v = 1.0f/w;
			return (x*v) * (rhs.x*v) + (y*v) * (rhs.y*v) + (z*v) * (rhs.z*v);
		}
	}
	
	inline Vertex operator+(const Vertex &rhs) const
	{
		Vertex res;
		if (w == 1.0)
		{
			res.x = x + rhs.x;
			res.y = y + rhs.y;
			res.z = z + rhs.z;
			res.w = 1.0f;
			return res;
		}
		else if(w==0)
		{
			res.x = 0.0f;
			res.y = 0.0f;
			res.z = 0.0f;
			res.w = 0.0f;
			return res;
		}
		else
		{
			float v = 1.0f/w;

			res.x = (x*v) + (rhs.x*v);
			res.y = (y*v) + (rhs.y*v);
			res.z = (z*v) + (rhs.z*v);
			res.w = 1.0f;
			return res;
		}
	}

	Vertex crossProduct(const Vertex &rhs) const
	{
		Vertex cross;
		cross.x = y * rhs.z - z * rhs.y;
		cross.y = z * rhs.x - x * rhs.z;
		cross.z = x * rhs.y - y * rhs.x;
		cross.w = 1.0f;
		return cross;
	}

	float scalar(const Vertex& v) const
	{ return x*v.x + y*v.y + z*v.z ; }

	void normalise()
	{
		float l = 1.0f/length();
		x *= l;
		y *= l;
		z *= l;
	}
};

#endif /* VERTEX_H_ */
