#ifndef COLOR_H
#define COLOR_H

///Color structure
/**
 * \author Jan Brejcha
 */
struct Color
{
	float r, g, b;
	
	///Default constructor sets all values to 0.0f.
	Color()
	{
		this->r = 0.0f;
		this->g = 0.0f;
		this->b = 0.0f;
	}
	
	///Initializer constructor
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
        
};
#endif
