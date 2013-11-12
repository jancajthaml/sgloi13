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


class FillPolygonModel : public Model
{
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 */
	FillPolygonModel(DrawingLibraryBase _g, Chunk _context, Material _material) : Model(_g, _context, _material){}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 @author Jan Cajthaml
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		Model::multiplyVerticesWithMVP(mpv);
		
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
	
	
	virtual bool findIntersection(const Ray &ray, float &t)
	{
		return triangle_intersection(vertices[0], vertices[1], vertices[2], ray, t);
	}
	
	/**
	 Triangle intersection.
	 Algorithm used from: http://geomalgorithms.com/a06-_intersect-2.html
	 */
	bool triangle_intersection(const Vertex &V0,  // Triangle vertices
							   const Vertex &V1,
							   const Vertex &V2,
							   const Ray &ray,
							   float &param)
	{
		Vertex    u, v, n;              // triangle vectors
		Vertex    dir, w0, w;           // ray vectors
		float     r, a, b;              // params to calc ray-plane intersect
		
		// get triangle edge vectors and plane normal
		u = V1 - V0;
		v = V2 - V0;
		n = u.crossProduct(v);              // cross product
		if (n.x == 0 && n.y == 0 && n.z == 0)             // triangle is degenerate
			return false;                  // do not deal with this case
		
		dir = ray.direction;         // ray direction vector
		w0 = ray.origin - V0;
		a = (n * -1) * w0;
		b = n * dir;
		if (fabs(b) < EPSILON) {     // ray is  parallel to triangle plane
			/*if (a == 0)                 // ray lies in triangle plane
				return 2;
			else return 0;*/		// ray disjoint from plane
			return false;
		}
		
		// get intersect point of ray with triangle plane
		r = a / b;
		if (r < 0.0)                    // ray goes away from triangle
			return false;                   // => no intersect
		// for a segment, also test if (r > 1.0) => no intersect
		param = r;
		Vertex I = ray.origin + (dir * r);            // intersect point of ray and plane
		// is I inside T?
		float    uu, uv, vv, wu, wv, D;
		uu = u * u;
		uv = u * v;
		vv = v * v;
		w = I - V0;
		wu = w * u;
		wv = w * v;
		D = uv * uv - uu * vv;
		
		// get and test parametric coords
		float s, t;
		s = (uv * wv - vv * wu) / D;
		if (s < 0.0 || s > 1.0)         // I is outside T
			return false;
		t = (uv * wu - uu * wv) / D;
		if (t < 0.0 || (s + t) > 1.0)  // I is outside T
			return false;
		
		return true;                       // I is in T	}
	}
};

#endif
