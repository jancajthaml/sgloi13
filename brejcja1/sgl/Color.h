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
	float a;

	Color()
	{
		r = g = b = 0.0f;

		// ? is alpha channel 1 by default or 0 ?
		a = 1.0f;
	}

    Color(float R, float G, float B)
    {
    	r	= R;
    	g	= G;
    	b	= B;

    	// ? is alpha channel 1 by default or 0 ?
    	a	= 1.0f;
    }

    Color(float R, float G, float B, float A)
    {
    	r	= R;
    	g	= G;
    	b	= B;
    	a	= A;
    }

    inline Color operator +(const Color &other)
    { return Color(*this) += other; }

    inline Color operator -(const Color &other)
    { return Color(*this) -= other; }

    Color& operator+=(const Color& other)
	{
        this->r = (other.r + this->r) * 0.5f;
        this->g = (other.g + this->g) * 0.5f;
        this->b = (other.b + this->b) * 0.5f;

        return *this;
    }

    Color& operator-=(const Color& other)
    {
    	this->r = (this->r - other.r) * 0.5f;
    	this->g = (this->g - other.g) * 0.5f;
    	this->b = (this->b - other.b) * 0.5f;

    	return *this;
    }

};

#endif /* COLOR_H_ */
