/*
 * DrawingLibrary_FLAT.h
 *
 *  Created on: 21.10.2013
 *      Author: jancajthaml
 */

#ifndef DRAWINGLIBRARYFLAT_H_
#define DRAWINGLIBRARYFLAT_H_

#include "../sgl.h"

class DrawingLibraryFlat : public DrawingLibraryInterface
{

	private:

		DrawingLibraryFlat() : DrawingLibraryInterface()	{}
		~DrawingLibraryFlat()								{}

		inline void fillSymPixel(signed x, signed y, signed center_x, signed center_y, Chunk &context)
		{
			signed to	= center_x+x;
			signed from	= center_x-x;

			signed h1	= center_y+y;
			signed l1	= center_y-y;

			for( ; from <= to; from++ )
			{
				setPixel( from, h1, context );
				setPixel( from, l1, context );
			}

			to		= center_x+y;
			from	= center_x-y;
			h1		= center_y+x;
			l1		= center_y-x;

			for( ; from <= to; from++ )
			{
				setPixel( from, h1 ,context );
				setPixel( from, l1 ,context );
			}
		}

		inline void setSymPixel(signed x, signed y, signed xs, signed ys, Chunk &context)
		{
			signed rx = x + xs;
			signed ry = y + ys;
			signed mrx = -x + xs;
			signed mry = -y + ys;
			setPixel(rx, ry, context);
			setPixel(rx, mry, context);
			setPixel(mrx, ry, context);
			setPixel(mrx, mry, context);
			rx = x + ys;
			ry = y + xs;
			mrx = -x + ys;
			mry = -y + xs;
			setPixel(ry, rx, context);
			setPixel(ry, mrx, context);
			setPixel(mry, rx, context);
			setPixel(mry, mrx, context);
		}

		inline void bresenham_x(signed x1, signed y1, signed x2, signed y2, Chunk &context)
		{
			signed dx	= x2 - x1;
			signed dy	= y2 - y1;
			signed sign	= 1;

			if (dy < 0) sign = -1;

			signed c0	= (dy << 1) * sign;
			signed c1	= c0 - (dx << 1);
			signed p	= c0 - dx;

			setPixel( x1, y1, context );

			for( signed i = x1 + 1; i <= x2; ++i )
			{
				if (p < 0)
				{
					p += c0;
					setPixel_x( context );
				}
				else
				{
					p += c1;
					if (sign > 0)	setPixel_xy  ( context );
					else			setPixel_xmy ( context );
				}
			}
		}

		inline void bresenham_y(signed x1, signed y1, signed x2, signed y2, Chunk &context)
		{
			signed dx	= x2 - x1;
			signed dy	= y2 - y1;
			signed sign	= 1;

			if (dy < 0)	sign = -1;

			signed c0	= (dy << 1) * sign;
			signed c1	= c0 - (dx << 1);
			signed p	= c0 - dx;

			setPixel( y1, x1, context );

			for (signed i = x1 + 1; i <= x2; ++i)
			{
				if (p < 0)
				{
					p += c0;
					setPixel_y(context);
				}
				else
				{
					p += c1;
					if( sign>0 )	setPixel_xy  (context);
					else			setPixel_mxy (context);
				}
			}
		}

	///######### API STARTS HERE ###############################################################

	public:

		inline void drawLine2D(Vertex a, Vertex b, Chunk &context)
		{
			int_fast32_t dx = abs(b.x - a.x);
			int_fast32_t dy = abs(b.y - a.y);

			if (dx > dy)
				if (a.x < b.x)	bresenham_x(a.x, a.y, b.x, b.y, context);
				else			bresenham_x(b.x, b.y, a.x, a.y, context);
			else
				if (a.y < b.y)	bresenham_y(a.y, a.x, b.y, b.x, context);
				else			bresenham_y(b.y, b.x, a.y, a.x, context);
		}

		inline void setPixel(float x, float y, Chunk &context)
		{
			int x1 = (int)x;
			int y1 = (int)y;
			if (x1 >= 0 && x1 < context.w && y1 >= 0 && y1 < context.h)
			{
				context.lastSetPixelIndex = uint_fast32_t(x1 + context.w * y1);
				context.buffer[context.lastSetPixelIndex] = context.color;
			}
		}

		inline void setPixel_x( Chunk &context )
		{
			context.lastSetPixelIndex++;

			if( context.lastSetPixelIndex < context.w_h )
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		inline void setPixel_y( Chunk &context )
		{
			context.lastSetPixelIndex += context.w;

			if( context.lastSetPixelIndex < context.w_h )
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		inline void setPixel_xy( Chunk &context )
		{
			context.lastSetPixelIndex += context.w + 1;

			if( context.lastSetPixelIndex < context.w_h )
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		inline void setPixel_mxy( Chunk &context )
		{
			context.lastSetPixelIndex += context.w - 1;

			if( context.lastSetPixelIndex < context.w_h )
				*((__color*) (context.buffer + context.lastSetPixelIndex))		= *((__color*) &(context.color));
		}

		inline void setPixel_xmy( Chunk &context )
		{
			context.lastSetPixelIndex += 1 - context.w;

			if( context.lastSetPixelIndex < context.w_h )
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		static DrawingLibraryFlat& instance()
		{
			static DrawingLibraryFlat * theInstance = new DrawingLibraryFlat(); // only initialized once!
			return *theInstance;
		}

		//POINTS

		inline void drawPoints( Chunk &context )
		{
			int_fast32_t s = int_fast32_t(context.vertices.index);

			if(context.size==1)
			{
				for (int_fast32_t i = 0; i < s; i++)
					setPixel( (context.vertices)[i].x, (context.vertices)[i].y, context );
			}
			else
			{
				int_fast8_t thickness = context.size;

				if((thickness%2)==0) thickness++;

				thickness >>= 1;

				for (int_fast32_t i = 0; i < s; i++)
				{
					Vertex v = (context.vertices)[i];
					for(int_fast8_t i = -thickness; i<context.size-1; i++)
						for(int_fast8_t j = -thickness; j<context.size-1; j++)
							setPixel(v.x+j, v.y+i, context);
				}
			}
		}
	
	

	//-----------------------------------------------------------------------------

	//LINES

		inline void drawLines( Chunk &context )
		{
			uint_fast32_t size = uint_fast16_t(context.vertices.index-1);

			for (uint_fast32_t i = 0; i < size; i += 2)
				drawLine2D(context.vertices[i], context.vertices[i+1], context);
		}

		inline void drawLineStrip( Chunk &context )
		{
			uint_fast16_t size = uint_fast16_t(context.vertices.index-1);

			for (uint_fast16_t i = 0; i < size; i++)
				drawLine2D(context.vertices[i], context.vertices[i+1], context);
		}

		inline void drawLineLoop( Chunk &context )
		{
			uint_fast16_t size = uint_fast16_t(context.vertices.index-1);

			for (uint_fast16_t i = 0; i < size; i++)
				drawLine2D( context.vertices[i], context.vertices[i+1], context);

			drawLine2D(context.vertices[size], context.vertices[0], context);
		}

		inline void fillTrianglesStrip( Chunk &context )
		{
			//Triangle strip
			uint_fast16_t size = uint_fast16_t(context.vertices.index-1);

			if(context.vertices.index<3) return;

			Vertex A  = context.vertices[0];
			Vertex B  = context.vertices[1];
			Vertex C  = context.vertices[2];

			for( uint_fast16_t i = 2; i < size; i++ )
			{
				drawTriangle(A, B, C, context);
				A  = B;
				B  = C;
				C  = context.vertices[i];
			}
		}

		inline void fillTrianglesFan( Chunk &context )
		{
			//Triangle fan
			uint_fast16_t size = uint_fast16_t(context.vertices.index-1);

			if(context.vertices.index<3) return;

			Vertex center	= context.vertices[0];
			Vertex A		= context.vertices[1];
			Vertex B		= context.vertices[2];

			for (uint_fast16_t i = 2; i <= size; i++)
			{
				drawTriangle(center, A, B, context);
				A	= B;
				B	= context.vertices[i];
			}

			drawTriangle(center, A, B, context);
		}

		inline void drawPolygon( Chunk &context )
		{
			uint_fast32_t size = uint_fast32_t(context.vertices.index-1);

			for (uint_fast32_t i = 0; i < size; i++)
				drawLine2D( context.vertices[i], context.vertices[i+1], context);

			drawLine2D( context.vertices[size], context.vertices[0], context );
		}

		inline void drawCircle(Vertex v, float r, Chunk &context)
		{
			int_fast32_t x = 0;
			int_fast32_t y = r;
			int_fast32_t p = 3 - (int_fast32_t(r) << 1);

			while(x < y)
			{
				setSymPixel(x, y, v.x, v.y, context);
				if (p < 0)
				{
					p += (x << 2) + 6;
				}
				else
				{
					p += ((x - y) << 2) + 10;
					y -= 1;
				}
				x += 1;
			}
			if( x==y ) setSymPixel(x, y, v.x, v.y, context);
		}


	//-----------------------------------------------------------------------------

	//FILL

		inline void fillCircle( Vertex v,float r,Chunk &context )
		{
			int_fast32_t x = 0;
			int_fast32_t y = r;
			int_fast32_t p = 3 - (int_fast32_t(r) << 1);

			while(x <= y)
			{
				fillSymPixel( x, y, v.x, v.y, context);

				if (p < 0)
				{
					p += (x << 1) + 6;
				}
				else
				{
					p += ((x - y) << 1) + 10;
					y -= 1;
				}

				x += 1;
			}
		}

		inline void setPixelChunk(int y, int start, int end, Chunk &context)
		{
			
			int n						= end-start+1;
			context.lastSetPixelIndex	= (start + context.w * y);

			while( n >= 0 )
			{
				context.buffer[context.lastSetPixelIndex++] = context.color;
				/*
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*((__color*) (context.buffer + context.lastSetPixelIndex++))	= *((__color*) &(context.color));
				*/
				n--;
			}
			/*
			switch( n )
			{
				case 7: context.buffer[context.lastSetPixelIndex++] = context.color;
				case 6: context.buffer[context.lastSetPixelIndex++] = context.color;
				case 5: context.buffer[context.lastSetPixelIndex++] = context.color;
				case 4: context.buffer[context.lastSetPixelIndex++] = context.color;
				case 3: context.buffer[context.lastSetPixelIndex++] = context.color;
				case 2: context.buffer[context.lastSetPixelIndex++] = context.color;
				case 1: context.buffer[context.lastSetPixelIndex++] = context.color;
					break;
			}*/
			 
		}

		inline void drawTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, Chunk &context)
		{

			int x0 = int(v0.x);
			int y0 = int(v0.y);
			int x1 = int(v1.x);
			int y1 = int(v1.y);
			int x2 = int(v2.x);
			int y2 = int(v2.y);

			if (y0 > y2)
			{
				int tmp = x0;
				x0      = x2;
				x2      = tmp;

				tmp     = y0;
				y0      = y2;
				y2      = tmp;
			}
			if (y1 > y2)
			{
				int tmp = x1;
				x1      = x2;
				x2      = tmp;

				tmp     = y1;
				y1      = y2;
				y2      = tmp;
			}
			if (y0 > y1)
			{
				int tmp = x0;
				x0      = x1;
				x1      = tmp;

				tmp     = y0;
				y0      = y1;
				y1      = tmp;
			}

		  float xs	= 0.0f;
		  float xe	= 0.0f;
		  float dxs	= 0.0f;
		  float dxe	= 0.0f;
		  float xm	= (x0)+(x2-x0)/float(y2-y0)*float(y1-y0);

		  if (x1<xm)
		  {
			  dxs = (x1-x0)/float(y1-y0);
			  dxe = (x2-x0)/float(y2-y0);
		  }
		  else
		  {
			  dxs = (x2-x0)/float(y2-y0);
			  dxe = (x1-x0)/float(y1-y0);
		  }

		  xs = x0;
		  xe = x0;

		  for(int y=y0;y<y1;y++)
		  {
		      setPixelChunk(y,int(xs)+1,int(xe), context);
		      xs+=dxs;
		      xe+=dxe;
		  }

		  if (x1<xm)
		  {
			  xs=x1;
			  xe=xm;
			  dxs = (x2-x1)/float(y2-y1);
			  dxe = (x2-xm)/float(y2-y1);
		  }
		  else
		  {
			  xe  = x1;
			  xs  = xm;
			  dxe = (x2-x1)/float(y2-y1);
			  dxs = (x2-xm)/float(y2-y1);
		  }

		  for(int y=y1;y<y2;y++)
		  {
			  setPixelChunk(y,int(xs)+1,int(xe),context);
			  xs+=dxs;
			  xe+=dxe;
		  }
		}

		inline void fillPolygon( Chunk &context )
		{
			int    size   =  context.vertices.index ;
			Edge * edges  =  new Edge[size]         ;
			int  * x      =  new int[size]          ;
			int  * y      =  new int[size]          ;

			x[0] = int(context.vertices[0].x);
			y[0] = int(context.vertices[0].y);

			int    min_y  =  y[0]  ;
			int    max_y  =  y[0]  ;

			for(int i=1; i<size; i++)
			{
			    x[i] = int(context.vertices[i].x);
			    y[i] = int(context.vertices[i].y);

			    if( y[i]<min_y )  min_y = y[i];
			    if( y[i]>max_y )  max_y = y[i];

			    if( y[i] < y[i-1] )
			    {
			    	edges[i].min_y  =  y[i]-1 ;
			    	edges[i].max_y  =  y[i-1]    ;
			    	edges[i].x      =  x[i]      ;
			    }
			    else
			    {
			    	edges[i].min_y  =  y[i-1]-1 ;
			    	edges[i].max_y  =  y[i]   ;
			    	edges[i].x      =  x[i-1]   ;
			    }

			    edges[i].delta_x = float(x[i]-x[i-1]) / float(y[i]-y[i-1]);
			  }

			  if( y[0] < y[size-1] )
			  {
				  edges[0].min_y = y[0]-1 ;
				  edges[0].max_y = y[size-1]   ;
				  edges[0].x     = x[0]   ;
			  }
			  else
			  {
				  edges[0].min_y = y[size-1]-1 ;
				  edges[0].max_y = y[0]   ;
				  edges[0].x     = x[size-1]   ;
			  }

			  edges[0].delta_x   = float(x[0]-x[size-1])/float(y[0]-y[size-1]);

			  float * draw =  new float[size];
			  int   count  =  0;

			  for( int y=min_y ; y<max_y ; y++ )
			  {
				  count = 0;

				  for( int v=0 ; v<size ; v++ )
				  {
					  if( (edges[v].min_y<y) & (edges[v].max_y>y) )
					  {
						  edges[v].x  +=  edges[v].delta_x;
						  draw[count]  =  edges[v].x;
						  count++;
					  }
				  }

				  Helper::sort(draw, count);

			    for( int i=0; i<count; i=i+2 )
			    	setPixelChunk(y+1, draw[i], draw[i+1]-1, context);
			  }

			  delete[] draw              ;
			  context.vertices.index = 0 ;
			  delete[] edges             ;
		}

};

#endif /* DRAWINGLIBRARYFLAT_H_ */
