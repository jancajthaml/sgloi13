//
//  FillPolygonModel.h
//  libsgl
//
//  Created by Jan Brejcha on 11.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_FillPolygonModel_h
#define libsgl_FillPolygonModel_h

#include "../helpers/Helpers.h"
class FillPolygonModel : public Model
{
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	FillPolygonModel(DrawingLibraryBase _g, Chunk _context) : Model(_g, _context)
	{
		this->g = _g;
		this->context = _context;
	}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		unsigned long    size   =  vertices.size()		;
		Edge * edges  =  new Edge[size]         ;
		float  delta  =  0.0f                   ;
		
		int  * x      =  new int[size]          ;
		int  * y      =  new int[size]          ;
		
		x[0] = int(vertices[0].x);
		y[0] = int(vertices[0].y);
		
		int    min_y  =  y[0];
		int    max_y  =  y[0];
		
		for( int i=1; i<size; i++ )
		{
			x[i] = int(vertices[i].x);
			y[i] = int(vertices[i].y);
			
			if( y[i]<min_y )  min_y = y[i];
			if( y[i]>max_y )  max_y = y[i];
			
			if( y[i] < y[i-1] )
			{
				edges[i].min_y  =  y[i]-1 ;
				edges[i].max_y  =  y[i-1]   ;
				edges[i].x      =  x[i];
				edges[i].z      =  vertices[i]   . z   ;
			}
			else
			{
				edges[i].min_y  =  y[i-1]-1 ;
				edges[i].max_y  =  y[i]  ;
				edges[i].x      =  x[i-1]   ;
				edges[i].z      =  vertices[i-1] . z   ;
			}
			
			delta            = float(y[i]-y[i-1]);
			edges[i].delta_x = float(x[i]-x[i-1]) / delta;
			edges[i].delta_z = (vertices[i].z - vertices[i-1].z) / delta;
		}
		
		if( y[0] < y[size-1] )
		{
			edges[0].min_y  =  y[0]-1 ;
			edges[0].max_y  =  y[size-1]  ;
			edges[0].x      =  x[0] ;
			edges[0].z      =  vertices[0]      . z   ;
		}
		else
		{
			edges[0].min_y  =  y[size-1]-1 ;
			edges[0].max_y  =  y[0]   ;
			edges[0].x      =  x[size-1]  ;
			edges[0].z      =  vertices[size-1] . z   ;
		}
		
		
		delta             =  float(y[0]-y[size-1]);
		edges[0].delta_x  =  float(x[0]-x[size-1])/ delta;
		edges[0].delta_z  =  (vertices[0].z-vertices[size-1].z) / delta;
		
		float * draw   =  new float[size] ;
		float * drawZ  =  new float[size] ;
		int     count  =  0;
		
		for( int y=min_y ; y<max_y ; y++ )
		{
			count = 0;
			for( int v=0 ; v<size ; v++ )
			{
				if( (edges[v].min_y<y) & (edges[v].max_y>y) )
				{
					edges[v].x  += edges[v].delta_x;
					edges[v].z  += edges[v].delta_z;
					draw[count]  = edges[v].x;
					drawZ[count] = edges[v].z;
					
					count++;
				}
			}
			
			Helper::sort(draw,drawZ,count);
			
			for( int i=0 ; i<count ; i=i+2 )
				setPixelChunk( y+1, draw[i], draw[i+1], drawZ[i], (drawZ[i+1]-drawZ[i])/(draw[i+1]-draw[i]), context );
		}
		// BUBBLE SORT END
		
		delete[] drawZ             ;
		delete[] draw              ;
		vertices.clear()		   ;
		delete[] edges             ;
	}
};

#endif
