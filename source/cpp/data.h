/*
 * data.h
 *
 *  Created on: 23.9.2013
 *      Author: jancajthaml
 */

#include <vector>
#include <list>
#ifndef DATA_H_
#define DATA_H_

/*
 * Side-notes:
 * Jan Cajthaml -using struct instead of class because of no need of encapsulation
 *
 * */

/*
 * ChangeLog:
 * 23.9.2013, Jan Cajthaml - added struct Vertex
 * 23.9.2013, Jan Cajthaml - added struct Edge
 * 24.9.2013, Jan Cajthaml - added struct Color
 * 24.9.2013, Jan Cajthaml - added struct Matrix
 * 24.9.2013, Jan Cajthaml - added struct Context
 * 25.9.2013, Jan Cajthaml - added ViewPort & depth flag to Context
 * 25.9.2013, Jan Cajthaml - added struct ContextManager
 * */

//---------------------------------------------------------------------------
// Data Structures
//---------------------------------------------------------------------------

//Vertex
struct Vertex
{
	float x;
	float y;
	float z;
	float w;

	Vertex()
	{ x = y = z = w = 0.0f; }

	Vertex(float X, float Y)
	{
		x = X;
		y = Y;
		z = w = 0.0f;
	}

	Vertex(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
		w = 0.0f;
	}

	Vertex(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
};

//Edge
struct Edge
{
	float	x_s;
	float	y_s;
	float	z_s;
	float	x_e;
	float	y_e;
	float	z_e;

    Edge()
    { x_s = y_s = z_s = x_e = y_e = z_e = 0.0f; }

    Edge(Vertex start, Vertex end)
	{
    	x_s	= start.x;
    	y_s	= start.y;
    	z_s	= start.z;
    	x_e	= end.x;
    	y_e	= end.y;
    	z_e	= end.z;
	}

};

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
};

//Matrix
// vectors in columns
struct Matrix
{
    float matrix[16];

    float& operator[](int i)
    {
        return matrix[i];
    }

    void operator=(float* m)
    {
    	//Memory copy faster then iteration
    	//memcpy(&matrix, m, 16 * sizeof(float) );

    	//Operation optimalisation
    	//
    	// ? whats better register shift of static allocation ?
    	// sizeof(float) << 4
    	//
    	memcpy(&matrix, m, sizeof(float) << 4 );

        //for(int i = 0; i<16; i++)
          //  matrix[i] = m[i];
    }

    void set(Matrix &m)
    {
        *this = m;
    }

    Matrix()
    {
    	//Memory fill faster then iteration
    	std::fill_n(matrix, 16, 0.0f);

    	//for(int i = 0; i<16; i++)
    		//matrix[i] = 0.0;
    }
};



//---------------------------------------------------------------------------
// Helper Structures
//---------------------------------------------------------------------------

//Context
struct Context
{
	int id;

    //Drawing
	float *buffer;
	int w;
	int h;
	Color clear;
	Color color;

	//State
	bool depth;

	//Viewport
	float x;
	float y;
	float w2;
	float h2;
	int min_x;
	int min_y;
	int max_x;
	int max_y;

	Context(int width, int height)
	{
		//----------------------//

		//Initialise Drawing
		w		= width;
		h		= height;
		id		= 0;
		buffer	= NULL;

		// ? this shoud be static or const equivalent to NULL ?
		clear	= Color(255,0,0);

		// ? this shoud be static or const equivalent to NULL ?
		color	= Color(0,255,0);

		//----------------------//

		//Initialise Viewport
		x		= 0.0f;
		y		= 0.0f;
		w2		= 0.0f;
		h2		= 0.0f;
		min_x	= 0;
		min_y	= 0;
		max_x	= 0;
		max_y	= 0;

		//----------------------//

		//Initialise Flags
		depth	= false;

	}
};

//Context Manager
struct ContextManager
{
    std::vector<Context*> contexts;
    int current;
};

#endif /* DATA_H_ */
