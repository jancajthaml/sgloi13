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

	friend Color operator*(const Color& c, float f)
	{ return Color(c.r*f, c.g*f, c.b*f); }

	friend Color operator*(float f, const Color& c)
	{ return Color(c.r*f, c.g*f, c.b*f); }

	void operator*=(const float f)
	{
		r *= f;
		g *= f;
		b *= f;
	}

	friend Color operator*(Color& c1, const Color& c2)
	{ return Color(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b); }

	void operator*=(const Color& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
	}

	friend Color operator+(float f, const Color& c)
	{ return Color(c.r+f, c.g+f, c.b+f); }

	void operator+=(const float f)
	{
		r += f;
		g += f;
		b += f;
	}

	/// Pricteni kontrastu
	friend Color operator+(const Color& c, float f)
	{ return Color(c.r+f, c.g+f, c.b+f); }

	friend Color operator-(float f, const Color& c)
	{ return Color(c.r-f, c.g-f, c.b-f); }

	/// Odecteni kontrastu
	friend Color operator-(const Color& c, float f)
	{ return Color(c.r-f, c.g-f, c.b-f); }

	void operator-(const float f)
	{
		r -= f;
		g -= f;
		b -= f;
	}

	friend Color operator+(const Color& c1, const Color& c2)
	{ return Color(c1.r+c2.r, c1.g+c2.g, c1.b+c2.b); }

	void operator+=(const Color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
	}
	void operator+=(Color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
	}

	friend Color operator-(const Color& c1, const Color& c2)
	{ return Color(c1.r-c2.r, c1.g-c2.g, c1.b-c2.b); }

	void operator-=(const Color& c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
	}

	bool operator==(const Color &other)
	{ return (r==other.r && g==other.g && b==other.b); }

};

#endif /* COLOR_H_ */
