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
    float x1, y1, z1,  x2, y2, z2;
    float deltaX;
    int deltaY;
    float deltaZy;
    float deltaZx;
    float intersectX;
    float intersectZy;
    float intersectZx;

    Edge(Vertex v1, Vertex v2)
    {
        x1 = v1.x;
        y1 = v1.y;
        z1 = v1.z;
        x2 = v2.x;
        y2 = v2.y;
        z2 = v2.z;
        deltaX = (x2-x1)/(y2-y1);
        deltaY =(int)(y2-y1);
        intersectX = x1;
        intersectZy = z1;
    }
    Edge(){}

};

#endif /* EDGE_H_ */
