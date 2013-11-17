/*
 * Color.h
 *
 *  Created on: 7.10.2013
 *      Author: jancajthaml
 */

#ifndef COLOR_H_
#define COLOR_H_


//Color
struct Color
{
	float r;
	float g;
	float b;

	// Default constructor sets all values to 0.0f.
	Color()
	{
		this->r = 0.0f;
		this->g = 0.0f;
		this->b = 0.0f;
	}

	// Initializer constructor
	/**
	 * \param r 	the red channel
	 * \param g	the green channel
	 * \param b 	the blue channel
	 */
	Color(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	/// Vyn�sob� farebn� zlo�ky danou kon�tantou
	friend Color operator*(const Color& c, float f) {
		return Color(c.r*f, c.g*f, c.b*f);
	}

	/// Vyn�sob� farebn� zlo�ky danou kon�tantou
	friend Color operator*(float f, const Color& c) {
		return Color(c.r*f, c.g*f, c.b*f);
	}

	/// K farebn�m zlo�k�m pri��ta kon�tantu
	friend Color operator+(float f, const Color& c) {
		return Color(c.r+f, c.g+f, c.b+f);
	}

	/// Vyn�sob� zodpovedaj�ce farebn� zlo�ky farieb
	friend Color operator*(Color& c1, const Color& c2) {
		return Color(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b);
	}

	/// K farebn�m zlo�k�m pri��ta kon�tantu
	friend Color operator+(const Color& c, float f) {
		return Color(c.r+f, c.g+f, c.b+f);
	}

	/// Od farebn�ch zlo�iek od��ta kon�tantu
	friend Color operator-(float f, const Color& c) {
		return Color(c.r-f, c.g-f, c.b-f);
	}

	/// Od farebn�ch zlo�iek od��ta kon�tantu
	friend Color operator-(const Color& c, float f) {
		return Color(c.r-f, c.g-f, c.b-f);
	}

	/// S��ta zodpovedaj�ce farebn� zlo�ky
	friend Color operator+(const Color& c1, const Color& c2) {
		return Color(c1.r+c2.r, c1.g+c2.g, c1.b+c2.b);
	}

	/// Od��ta zodpovedaj�ce farebn� zlo�ky
	friend Color operator-(const Color& c1, const Color& c2)
	{
		return Color(c1.r-c2.r, c1.g-c2.g, c1.b-c2.b);
	}
	bool operator==(const Color &other)
	{
		if(r==other.r && g==other.g && b==other.b) return true;
		return false;
	}

	void clamp()
	{

		//This solves the white points bug - cajthjan
		if( r<0 ) r=0.0f;
		if( g<0 ) g=0.0f;
		if( b<0 ) b=0.0f;

		if( r>1 ) r=1.0f;
		if( g>1 ) g=1.0f;
		if( b>1 ) b=1.0f;
	}

};

#endif /* COLOR_H_ */
