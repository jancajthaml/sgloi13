/*
 * Edge.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef EDGE_H_
#define EDGE_H_


struct Edge
{
	//FIXME change to Vertex??

	  int miny,maxy;
	  float dx,actualX;
	  Edge() {};
	  Edge(int miny,int maxy, float dx, float actualX) : miny(miny),maxy(maxy),dx(dx),actualX(actualX) {};

	  /*
	   * //FIXME change to Vertex??

	Vertex v1;
	Vertex v2;

    float deltaX;
    int deltaY;

    float deltaZy;
    float deltaZx;

    float intersectX;

    float intersectZy;
    float intersectZx;

    Edge(Vertex v1, Vertex v2)
    {
    	this->v1			= v1;
    	this->v2			= v2;
    	this->deltaX		= (v2.x-v1.x)/(v2.y-v1.y);
        this->deltaY		= int(v2.y-v1.y);

        this->deltaZy = 0.0f;
        this->deltaZx = 0.0f;


        this->intersectX	= v1.x;
        this->intersectZy	= v1.z;
        this->intersectZx	= 0.0f;
    }

    Edge()
    {
    	this->v1			= Vertex(0.0f,0.0f,1.0f,1.0f);
    	this->v2			= Vertex(0.0f,0.0f,1.0f,1.0f);
    	this->deltaX		= 0.0f;
        this->deltaY		= 0;

        this->deltaZy = 0.0f;
        this->deltaZx = 0.0f;


        this->intersectX	= 0.0f;
        this->intersectZy	= 0.0f;
        this->intersectZx	= 0.0f;
    }
	   */
};

#endif /* EDGE_H_ */
