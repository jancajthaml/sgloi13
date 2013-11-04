/*
 * DrawingLibrary_FLAT.h
 *
 *  Created on: 21.10.2013
 *      Author: jancajthaml
 */

#ifndef DRAWINGLIBRARY_DEPTH_H_
#define DRAWINGLIBRARY_DEPTH_H_


class DrawingLibraryDepth : public DrawingLibraryInterface
{

	private:
		DrawingLibraryDepth() : DrawingLibraryInterface()	{}
		~DrawingLibraryDepth()								{}

		inline void fillSymPixel(signed x, signed y, signed center_x, signed center_y, Chunk &context)
		{
			signed to	= center_x+x;
			signed from	= center_x-x;

			signed h1	= center_y+y;
			signed l1	= center_y-y;

			for( ; from <= to; from++ )
			{
				setPixel( from, h1, 0, context );
				setPixel( from, l1, 0, context );
			}

			to		= center_x+y;
			from	= center_x-y;
			h1		= center_y+x;
			l1		= center_y-x;

			for( ; from <= to; from++ )
			{
				setPixel( from, h1, 0, context );
				setPixel( from, l1, 0, context );
			}
		}

		inline void setSymPixel(signed x, signed y, signed xs, signed ys, Chunk &context)
		{
			signed rx = x + xs;
			signed ry = y + ys;
			signed mrx = -x + xs;
			signed mry = -y + ys;
			setPixel(rx, ry, 0, context);
			setPixel(rx, mry, 0, context);
			setPixel(mrx, ry, 0, context);
			setPixel(mrx, mry, 0, context);
			rx = x + ys;
			ry = y + xs;
			mrx = -x + ys;
			mry = -y + xs;
			setPixel(ry, rx, 0, context);
			setPixel(ry, mrx, 0, context);
			setPixel(mry, rx, 0, context);
			setPixel(mry, mrx, 0, context);
		}

	///######### API STARTS HERE ###############################################################


	public:

		inline void drawLine2D(Vertex a, Vertex b, Chunk &context)
		{
			//FIXME incorporate z COORDINATE
			//DrawingLibraryFlat::instance().drawLine2D(a, b, context);

			int_fast32_t dx = abs(b.x - a.x);
			int_fast32_t dy = abs(b.y - a.y);

			if (dx > dy)
				if (a.x < b.x)	bresenham_x(a.x, a.y, a.z, b.x, b.y, b.z, context);
				else			bresenham_x(b.x, b.y, a.z, a.x, a.y, b.z, context);
			else
				if (a.y < b.y)	bresenham_y(a.y, a.x, a.z, b.y, b.x, b.z, context);
				else			bresenham_y(b.y, b.x, a.z, a.y, a.x, b.z, context);
		}


		inline void bresenham_x(signed x1, signed y1, signed z1, signed x2, signed y2, signed z2, Chunk &context)
		{
			signed dx	= x2 - x1;
			signed dy	= y2 - y1;
			signed sign	= 1;

			if (dy < 0) sign = -1;

			signed c0	= (dy << 1) * sign;
			signed c1	= c0 - (dx << 1);
			signed p	= c0 - dx;

			setPixel( x1, y1, z1, context );

			for (signed i = x1 + 1; i <= x2; ++i)
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

		inline void bresenham_y(signed x1, signed y1, signed z1, signed x2, signed y2, signed z2, Chunk &context)
		{
			signed dx	= x2 - x1;
			signed dy	= y2 - y1;
			signed sign	= 1;

			if (dy < 0)	sign = -1;

			signed c0	= (dy << 1) * sign;
			signed c1	= c0 - (dx << 1);
			signed p	= c0 - dx;

			setPixel( y1, x1, z1, context );

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

		inline void setPixel(float x, float y, float z, Chunk &context)
		{
			uint_fast32_t index = uint_fast32_t((int)x + context.w * (int)y);
			if (x >= 0 && x < context.w && y >= 0 && y < context.h && context.depth[index] > z)
			{
				context.lastSetPixelIndex	= index;
				context.depth[index]		= z;
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
			}
		}

		inline void setPixel_x( Chunk &context )
		{ DrawingLibraryFlat::instance().setPixel_x(context); }

		inline void setPixel_y(Chunk &context)
		{ DrawingLibraryFlat::instance().setPixel_y(context); }

		inline void setPixel_xy(Chunk &context)
		{ DrawingLibraryFlat::instance().setPixel_xy(context); }

		inline void setPixel_mxy(Chunk &context)
		{ DrawingLibraryFlat::instance().setPixel_mxy(context); }

		inline void setPixel_xmy(Chunk &context)
		{ DrawingLibraryFlat::instance().setPixel_xmy(context); }

		static DrawingLibraryDepth& instance()
		{
			static DrawingLibraryDepth * theInstance = new DrawingLibraryDepth(); // only initialized once!
			return *theInstance;
		}
	//POINTS

		inline void drawPoints( Chunk &context )
		{
			int_fast32_t s = int_fast32_t(context.vertices.index);

			if(context.size==1)
			{
				for (int_fast32_t i = 0; i < s; i++)
					setPixel( (context.vertices)[i].x, (context.vertices)[i].y, (context.vertices)[i].z, context );
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
							setPixel(v.x+j, v.y+i, (context.vertices)[i].z, context);
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

		inline void fillTrianglesFan( Chunk &context )
		{ DrawingLibraryFlat::instance().fillTrianglesFan(context); }

		inline void fillTrianglesStrip( Chunk &context )
		{ DrawingLibraryFlat::instance().fillTrianglesStrip(context); }

		inline void drawPolygon( Chunk &context )
		{
			uint_fast32_t size = uint_fast32_t(context.vertices.index-1);

			for (uint_fast32_t i = 0; i < size; i++)
				drawLine2D( context.vertices[i], context.vertices[i+1], context);

			drawLine2D( context.vertices[size], context.vertices[0], context );
		}


		inline void drawCircle( Vertex v,float r, Chunk &context)
		{
			int_fast32_t x = 0;
			int_fast32_t y = r;
			int_fast32_t p = 3 - (int_fast32_t(r) << 1);

			while( x<y )
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

		inline void fillPolygon( Chunk &context )
		{
			int    size   =  context.vertices.index ;
			Edge * edges  =  new Edge[size]         ;
			float  delta  =  0.0f                   ;

			int  * x      =  new int[size]          ;
			int  * y      =  new int[size]          ;

			x[0] = int(context.vertices[0].x);
			y[0] = int(context.vertices[0].y);

			int    min_y  =  y[0];
			int    max_y  =  y[0];

			for( int i=1; i<size; i++ )
			{
			    x[i] = int(context.vertices[i].x);
			    y[i] = int(context.vertices[i].y);

			    if( y[i]<min_y )  min_y = y[i];
			    if( y[i]>max_y )  max_y = y[i];

				if( y[i] < y[i-1] )
				{
					edges[i].min_y  =  y[i]-1 ;
					edges[i].max_y  =  y[i-1]   ;
					edges[i].x      =  x[i];
					edges[i].z      =  context.vertices[i]   . z   ;
				}
				else
				{
					edges[i].min_y  =  y[i-1]-1 ;
					edges[i].max_y  =  y[i]  ;
					edges[i].x      =  x[i-1]   ;
					edges[i].z      =  context.vertices[i-1] . z   ;
				}

				delta            = float(y[i]-y[i-1]);
				edges[i].delta_x = float(x[i]-x[i-1]) / delta;
				edges[i].delta_z = (context.vertices[i].z-context.vertices[i-1].z) / delta;
			  }

			  if( y[0] < y[size-1] )
			  {
				  edges[0].min_y  =  y[0]-1 ;
				  edges[0].max_y  =  y[size-1]  ;
				  edges[0].x      =  x[0] ;
				  edges[0].z      =  context.vertices[0]      . z   ;
			  }
			  else
			  {
				  edges[0].min_y  =  y[size-1]-1 ;
				  edges[0].max_y  =  y[0]   ;
				  edges[0].x      =  x[size-1]  ;
				  edges[0].z      =  context.vertices[size-1] . z   ;
			  }


			  delta             =  float(y[0]-y[size-1]);
			  edges[0].delta_x  =  float(x[0]-x[size-1])/ delta;
			  edges[0].delta_z  =  (context.vertices[0].z-context.vertices[size-1].z) / delta;

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
			  context.vertices.index = 0 ;
			  delete[] edges             ;
		}

		inline void setPixelChunk( int y, int start, int end, float z, float z_growth, Chunk &context )
		{
			 for( int x=start ; x<end ; x++ )
			 {
				 setPixel(x,y,z,context);
				 z+=z_growth;
			 }
		}

};

#endif /* DRAWINGLIBRARYFLAT_H_ */
