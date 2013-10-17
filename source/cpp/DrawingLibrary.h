/*
 * DrawingLibrary.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef DRAWINGLIBRARY_H_
#define DRAWINGLIBRARY_H_

#include <stdint.h>
#include "Helpers.h"
#include "Color.h"
#include "Vertex.h"
#include "Edge.h"
#include "EdgeStack.h"
#include "VertexStack.h"
#include "ContextChunk.h"

class DrawingLibrary
{

	private:

	//Buffer Manipulation methods


		DrawingLibrary()
		{
		}

		~DrawingLibrary()
		{}

		static inline void setPixel(signed x, signed y, Chunk &context)
		{
			//Condition not needed for now
			if (x >= 0 && x < context.w && y >= 0 && y < context.h)
			{
				context.lastSetPixelIndex = (x + context.w * y);
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
			}
		}

		static inline void setPixel_x( Chunk &context )
		{
			context.lastSetPixelIndex += 1;

			//Safety off
			//if (lastSetPixelIndex < w_h)
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		static inline void setPixel_y(Chunk &context)
		{
			context.lastSetPixelIndex += context.w;

			//Safety off
	//		if (lastSetPixelIndex < w_h)
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		static inline void setPixel_xy(Chunk &context)
		{
			context.lastSetPixelIndex += (context.w + 1);

			//Safety off
			//if (lastSetPixelIndex < w_h)
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		static inline void setPixel_mxy(Chunk &context)
		{
			context.lastSetPixelIndex += context.w - 1;

			//Safety off
			//if (lastSetPixelIndex < w_h)
				*((__color*) (context.buffer + context.lastSetPixelIndex))		= *((__color*) &(context.color));
		}

		static inline void setPixel_xmy(Chunk &context)
		{
			context.lastSetPixelIndex += (1 - context.w);
			//if (lastSetPixelIndex < w_h)
				*((__color*) (context.buffer + context.lastSetPixelIndex))	= *((__color*) &(context.color));
		}

		static inline void fillSymPixel(signed x, signed y, signed xs, signed ys, Chunk &context)
		{

			//filling the TOP and BOTTOM quarter
			int to = x+xs;
			int from = x-xs;

			for( ; from <= to; from++ )
			{
				setPixel(from, y+ys, context);
				setPixel(from, y-ys, context);
			}

			//filling the LEFT and RIGHT quarter
			to = x+ys;
			from = x-ys;

			for( ; from <= to; from++ )
			{
				setPixel(from, y+xs, context);
				setPixel(from, y-xs, context);
			}
		}

		static inline void setSymPixel(signed x, signed y, signed xs, signed ys, Chunk &context)
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

		static inline void bresenham_x(signed x1, signed y1, signed x2, signed y2, Chunk &context)
		{
			signed dx = x2 - x1;
			signed dy = y2 - y1;
			signed sign = 1;

			if (dy < 0) sign = -1;

			signed c0 = (dy << 1) * sign;
			signed c1 = c0 - (dx << 1);
			signed p = c0 - dx;

			setPixel( x1, y1, context );

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
					if (sign > 0)	setPixel_xy( context );
					else			setPixel_xmy( context );
				}
			}
		}

		static inline void bresenham_y(signed x1, signed y1, signed x2, signed y2, Chunk &context)
		{
			signed dx = x2 - x1;
			signed dy = y2 - y1;
			signed sign = 1;
			if (dy < 0)	sign = -1;
			signed c0, c1, p;
			c0 = (dy << 1) * sign;
			c1 = c0 - (dx << 1);
			p = c0 - dx;

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
					if (sign > 0)
						setPixel_xy(context);
					else
						setPixel_mxy(context);
				}
			}
		}

		static inline void bresenham_circle_draw( int_fast32_t xs, int_fast32_t ys, int_fast32_t r, Chunk &context)
		{
			int_fast32_t x, y, p;
			x = 0;
			y = r;
			p = 3 - (r << 1);
			while (x < y)
			{
				setSymPixel(x, y, xs, ys, context);
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
			if (x == y) setSymPixel(x, y, xs, ys, context);
		}

		static inline void bresenham_circle_fill( int_fast32_t xs, int_fast32_t ys, int_fast32_t r, Chunk &context)
		{
			int_fast32_t x, y, p;
			x = 0;
			y = r;
			p = 3 - (r << 1);

			int from	= x-r;
			int to		= x+r;

			for(; from<=to; from++) setPixel(from, y, context);

			while (x < y)
			{
				if (p < 0)
				{
					p += (x << 2) + 6;
				}
				else
				{
					p += ((x - y) << 2) + 10;
					y -= 1;
				}
				fillSymPixel( x, y, xs, ys, context );
				x += 1;
			}
		}

		static inline void drawLine2D(Vertex a, Vertex b, Chunk &context)
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

	///######### API STARTS HERE ###############################################################

	public:

	//POINTS

		static void drawPoints( Chunk &context )
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

		static void drawLines( Chunk &context )
		{
			uint_fast32_t size = uint_fast16_t(context.vertices.index-1);

			for (uint_fast32_t i = 0; i < size; i += 2)
				drawLine2D(context.vertices[i], context.vertices[i+1], context);
		}

		static void drawLineStrip( Chunk &context )
		{
			uint_fast16_t size = uint_fast16_t(context.vertices.index-1);

			for (uint_fast16_t i = 0; i < size; i++)
				drawLine2D(context.vertices[i], context.vertices[i+1], context);
		}

		static void drawLineLoop( Chunk &context )
		{
			uint_fast16_t size = uint_fast16_t(context.vertices.index-1);

			for (uint_fast16_t i = 0; i < size; i++)
				drawLine2D( context.vertices[i], context.vertices[i+1], context);

			drawLine2D(context.vertices[size], context.vertices[0], context);
		}


		static void drawPolygon( Chunk &context )
		{
			uint_fast32_t size = uint_fast32_t(context.vertices.index-1);

			for (uint_fast32_t i = 0; i < size; i++)
				drawLine2D( context.vertices[i], context.vertices[i+1], context);

			drawLine2D( context.vertices[size], context.vertices[0], context );
		}

		static void drawTriangles() {}

		static void drawCircle( Vertex v,float r, Chunk &context)
		{
			bresenham_circle_draw(int_fast32_t(v.x), (int_fast32_t)(v.y), int_fast32_t(r), context);
		}


	//-----------------------------------------------------------------------------

	//FILL


		static void fillCircle( Vertex v,float r,Chunk &context )
		{

			bresenham_circle_fill(int_fast32_t(v.x), (int_fast32_t)(v.y), int_fast32_t(r), context);
		}


		static void fillPolygon( Chunk &context )
		{
			bool depth	= false;

		    EdgeStack* tableEdges = new EdgeStack[context.h];
		    //FIXME
			int min = Helper::bucketSort(&tableEdges[0], context.vertices.index-1, context.vertices);

			std::vector<Edge > actEdges;

			for(int y = min; y<context.h; y++)
			{
				while(tableEdges[y].index!=0)
				{
					actEdges.push_back(tableEdges[y].back());
					tableEdges[y].pop_back();
				}
				if(actEdges.empty())continue;

				//FIXME
				Helper::bubbleSort(actEdges, actEdges.size(), context.vertices);

				int sizeAE = actEdges.size();
				float to;

				for(int i = 1; i<sizeAE; i+=2)
				{
					to = actEdges[i].intersectX;
					float z1 = actEdges[i-1].z2 + (y-actEdges[i-1].y2)*(actEdges[i-1].z1 - actEdges[i-1].z2)/(actEdges[i-1].y1 - actEdges[i-1].y2);
					float z2 = actEdges[i].z2 + (y-actEdges[i].y2)*(actEdges[i].z1 - actEdges[i].z2)/(actEdges[i].y1 - actEdges[i].y2);
					float deltaZ = (z2-z1)/(actEdges[i].intersectX-actEdges[i-1].intersectX);

					for(float from = actEdges[i-1].intersectX; from<=to; from++)
					{
						if(depth)
						{
							z1+=deltaZ;
						}
						else
						{
							setPixel(from, y,context);
						}
					}

					if(sizeAE % 3==0 && sizeAE % 2!=0 && i+2==sizeAE)
					{
						to = actEdges[i+1].intersectX;
						float z1 = actEdges[i].z2 + (y-actEdges[i].y2)*(actEdges[i].z1 - actEdges[i].z2)/(actEdges[i].y1 - actEdges[i].y2);
						float z2 = actEdges[i+1].z2 + (y-actEdges[i+1].y2)*(actEdges[i+1].z1 - actEdges[i+1].z2)/(actEdges[i+1].y1 - actEdges[i+1].y2);
						float deltaZ = (z2-z1)/(actEdges[i].intersectX-actEdges[i].intersectX);

						for(float from = actEdges[i].intersectX; from<=to; from++)
						{
							if(depth)
							{
								z1+=deltaZ;
							}
							else
							{
								setPixel(from, y,context);
			                }
			           }
			        }
			        }

			        for(unsigned int pos = 0; pos<actEdges.size(); pos++){

			            if(actEdges[pos].deltaY < 1){
			                actEdges.erase(actEdges.begin()+pos);
			                pos--;
			            }else{
			                actEdges[pos].deltaY--;
			                actEdges[pos].intersectX+=actEdges[pos].deltaX;
			            }
			        }


			    }
			    delete[] tableEdges;
			    context.vertices.index=0;
		}



};


#endif /* DRAWINGLIBRARY_H_ */
