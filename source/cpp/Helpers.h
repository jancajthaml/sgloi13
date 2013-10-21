/*
 * Helpers.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef HELPERS_H_
#define HELPERS_H_


//#include "Helpers.h"
//#include "Color.h"
#include "Vertex.h"
#include "Edge.h"
//#include "Matrix.h"
#include "EdgeStack.h"
#include "VertexStack.h"

//TODO COMMENT !!!!!!!
struct Helper
{

	static inline void bubbleSort(std::vector<Edge>& pole, int length, VertexStack& vertices)
	{
	    bool swap = true;
	    int n = length;
	    while(swap){
	        swap = false;
	        for(int i=1;i<n;i++){
	            if(pole[i-1].intersectX>pole[i].intersectX){
	                swap = true;
	                Edge temp=pole[i];
	                pole[i]= pole[i-1];
	                pole[i-1]=temp;
	            }
	        }
	        n--;
	    }

	}

	static inline int bucketSort(EdgeStack* tableEdges, int length, VertexStack& vertices)
	{
	    int min = 0;
	    int pos;
	    int i;
	    for( i = 0; i<length-1; i++)
	    {
	        if(vertices[i].y== vertices[i+1].y) continue;

	        if(vertices[i].y>vertices[i+1].y)
	        {
	            Edge e(vertices[i+1], vertices[i]);
	            pos= int(e.v1.y);
	            tableEdges[pos].push_back(e);
	        }
	        else
	        {
	            Edge e(vertices[i], vertices[i+1]);
	            pos= int(e.v1.y);
	            tableEdges[pos].push_back(e);
	        }
	        if(pos<min || min==0 )min = pos;
	    }

	    if(vertices[i].y == vertices[0].y) return min;
	    if(vertices[i].y>vertices[0].y)
	    {
	        Edge e(vertices[0], vertices[i]);
	        pos= int(e.v1.y);
	        tableEdges[pos].push_back(e);
	    }
	    else
	    {
	        Edge e(vertices[i], vertices[0]);
	        pos = int(e.v1.y);
	        tableEdges[pos].push_back(e);
	    }
	    if(pos<min || min==0 )min = pos;

	    return min;
	}
};

#endif /* HELPERS_H_ */
