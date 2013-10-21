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

		inline void setPixel(signed x, signed y, Chunk &context)
		{ DrawingLibraryFlat::instance().setPixel(x, y, context); }

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


		inline void drawLine2D(Vertex a, Vertex b, Chunk &context)
		{ DrawingLibraryFlat::instance().drawLine2D(a, b, context); }

	///######### API STARTS HERE ###############################################################


	public:
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
			bool depth	= true;

		    EdgeStack* tableEdges = new EdgeStack[context.h];

		    //FIXME
			int min = Helper::bucketSort(&tableEdges[0], context.vertices.index, context.vertices);

			std::vector<Edge> active_edges;

			for(int y = min; y<context.h; y++)
			{
				while(tableEdges[y].index!=0)
				{
					active_edges.push_back(tableEdges[y].back());
					tableEdges[y].pop_back();
				}

				if( active_edges.empty() )continue;

				uint_fast16_t size	= active_edges.size();

				//FIXME use shaker sort
				Helper::bubbleSort(active_edges, size, context.vertices);


				float to	= 0.0f;

				for( int i = 1; i<size; i+=2 )
				{
					to				= active_edges[i].intersectX;
					float z1		= active_edges[i-1].v2.z + (y-active_edges[i-1].v2.y)*(active_edges[i-1].v1.z - active_edges[i-1].v2.z)/(active_edges[i-1].v1.y - active_edges[i-1].v2.y);
					float z2		= active_edges[i].v2.z + (y-active_edges[i].v2.y)*(active_edges[i].v1.z - active_edges[i].v2.z)/(active_edges[i].v1.y - active_edges[i].v2.y);
					float deltaZ	= (z2-z1)/(active_edges[i].intersectX-active_edges[i-1].intersectX);

					for( float from = active_edges[i-1].intersectX; from<=to; from++ )
					{
						if( depth )	z1+=deltaZ;
						else		setPixel(from, y,context);
					}

					if(size % 3==0 && size % 2!=0 && i+2==size)
					{
						to				= active_edges[i+1].intersectX;
						float z1		= active_edges[i].v2.z + (y-active_edges[i].v2.y)*(active_edges[i].v1.z - active_edges[i].v2.z)/(active_edges[i].v1.y - active_edges[i].v2.y);
						float z2		= active_edges[i+1].v2.z + (y-active_edges[i+1].v2.y)*(active_edges[i+1].v1.z - active_edges[i+1].v2.z)/(active_edges[i+1].v1.y - active_edges[i+1].v2.y);
						float deltaZ	= (z2-z1)/(active_edges[i].intersectX-active_edges[i].intersectX);

						for(float from = active_edges[i].intersectX; from<=to; from++)
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

			        for( unsigned int pos=0 ; pos<active_edges.size() ; pos++ )
			        {
			            if(active_edges[pos].deltaY < 1)
			            {
			            	active_edges.erase(active_edges.begin()+pos);
			                pos--;
			            }
			            else
			            {
			            	active_edges[pos].deltaY--;
			            	active_edges[pos].intersectX += active_edges[pos].deltaX;
			            }
			        }
			    }

			    delete[] tableEdges;
			    context.vertices.index=0;
		}

};

//DrawingLibraryDepth DrawingLibraryDepth::self = DrawingLibraryDepth();

#endif /* DRAWINGLIBRARYFLAT_H_ */
