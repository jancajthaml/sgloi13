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
#define EPSILON 0.000001

#define USE_TRIANGLE_NORMAL

class FillPolygonModel : public Model
{
private:
	Vertex normal;
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	FillPolygonModel( Chunk _context, Material _material) : Model( _context, _material)
	{
		normal.w=-1;
	}
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 @author Jan Cajthaml
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		Model::multiplyVerticesWithMVP(mpv);
		
		const int    size   =  vertices.size()		;
		Edge * edges  =  new Edge[size]         ;
		float  delta  =  0.0f                   ;
		
		int* x     	=  new int[size]          ;
		int* y      =  new int[size]          ;
		
		x[0] = int(vertices[0].x);
		y[0] = int(vertices[0].y);
		
		int    min_y  =  y[0];
		int    max_y  =  y[0];
		
		size_t i = -1;
		while( ++i<size )
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
	
	virtual bool findIntersection(const Ray &ray, float &t)
	{
		Vertex p1 = vertices[0];
		Vertex p2 = vertices[1];
		Vertex p3 = vertices[2];
		Vertex e1 = p2 - p1;
		Vertex e2 = p3 - p1;
		Vertex s1 = ray.direction.crossProduct(e2);
		float divisor = s1*e1;
		if (divisor == 0.0f) return false;

		float invDivisor = 1.f / divisor;

		Vertex d = ray.origin - p1;
		float b1 = (d* s1) * invDivisor;
		if (b1 < 0.0f || b1 > 1.0f) return false;

			// Compute second barycentric coordinate
			Vertex s2 = d.crossProduct(e1);
			s2.normalise();
			float b2 = (ray.direction* s2) * invDivisor;
			if (b2 < 0.0f || b1 + b2 > 1.0f)
				return false;

			// Compute _t_ to intersection point
			float hit = (e2*s2) * invDivisor;

			if (hit < ray.t_min || hit > ray.t_max)
				return false;

			//printf("tu");
			t = hit;
			return true;

	}
	

	virtual Vertex getNormal(const Vertex &i)
	{
		#ifdef USE_TRIANGLE_NORMAL
		if(normal.w==-1)
		{
			normal = (vertices[2] - vertices[0]).crossProduct(vertices[2] - vertices[1]);
			normal.normalise();
		}
		return normal;
		#endif

		Vertex n = (i - vertices[0]).crossProduct(i - vertices[1]);
		n.normalise();

		return n;
	}
	
	virtual const char * getName()
	{
		return "TRI\n";
	}
	
	virtual bool backfaceCull(const Ray &ray, const float &t)
	{ return ray.direction * getNormal(ray.extrapolate(t)) >= 0.0; }

};

#endif
