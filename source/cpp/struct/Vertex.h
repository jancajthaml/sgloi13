/*
 * Vertex.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef VERTEX_H_
#define VERTEX_H_


//TODO COMMENT !!!!!!!
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
	{
		printf("[%f, %f, %f, %f]\n", x, y, z, w);
	}
	
	
	inline float length()
	{
		if (w == 1.0){
			return sqrt(x * x + y * y + z * z);
		}
		float x1 = x/w;
		float y1 = y/w;
		float z1 = z/w;
		return sqrt(x1 * x1 + y1 * y1 + z1 * z1);
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
		res.x = (x/w) - (other.x/w);
		res.y = (y/w) - (other.y/w);
		res.z = (z/w) - (other.z/w);
		res.w = 1.0f;
		return res;
	}
	
	inline Vertex operator/(const float &rhs) const
	{
		Vertex res;
		if (w == 1.0)
		{
			res.x = x / rhs;
			res.y = y / rhs;
			res.z = z / rhs;
			res.w = 1.0f;
			return res;
		}
		else
		{
			res.x = (x/w) / rhs;
			res.y = (y/w) / rhs;
			res.z = (z/w) / rhs;
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
			res.x = (x/w) * rhs;
			res.y = (y/w) * rhs;
			res.z = (z/w) * rhs;
			res.w = 1.0f;
			return res;
		}
	}
	
	inline float operator*(const Vertex &rhs) const
	{
		float res;
		if (w == 1.0)
		{
			res = x * rhs.x + y * rhs.y + z * rhs.z;
			return res;
		}
		else
		{
			res = (x/w) * (rhs.x/w) + (y/w) * (rhs.y/w) + (z/w) * (rhs.z/w);
			return res;
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
		else
		{
			res.x = (x/w) + (rhs.x/w);
			res.y = (y/w) + (rhs.y/w);
			res.z = (z/w) + (rhs.z/w);
			res.w = 1.0f;
			return res;
		}
	}

};

#endif /* VERTEX_H_ */
