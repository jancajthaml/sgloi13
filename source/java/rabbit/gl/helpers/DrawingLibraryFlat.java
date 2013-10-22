package rabbit.gl.helpers;

import rabbit.gl.engine.Chunk;
import rabbit.gl.math.SimpleMath;
import rabbit.gl.struct.Vertex;

public class DrawingLibraryFlat implements DrawingLibraryInterface
{

	private DrawingLibraryFlat()	{}
		//~DrawingLibraryFlat()								{}

		private void fillSymPixel(float x, float y, float center_x, float center_y, Chunk context)
		{
			float to	= center_x+x;
			float from	= center_x-x;

			float h1	= center_y+y;
			float l1	= center_y-y;

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
				setPixel( from, h1, context );
				setPixel( from, l1, context );
			}
		}

		private void setSymPixel(float x, float y, float xs, float ys, Chunk context)
		{
			float rx = x + xs;
			float ry = y + ys;
			float mrx = -x + xs;
			float mry = -y + ys;
			setPixel(rx, ry,  context);
			setPixel(rx, mry,  context);
			setPixel(mrx, ry,  context);
			setPixel(mrx, mry, context);
			rx = x + ys;
			ry = y + xs;
			mrx = -x + ys;
			mry = -y + xs;
			setPixel(ry, rx,  context);
			setPixel(ry, mrx,  context);
			setPixel(mry, rx,  context);
			setPixel(mry, mrx,  context);
		}

	///######### API STARTS HERE ###############################################################

		public void drawLine2D(Vertex a, Vertex b, Chunk context)
		{
			if(a == null || b == null) return;
				
				//obtain the points
				int x1, x2, y1, y2;
				x1 = SimpleMath.round(a.x);
				y1 = SimpleMath.round(a.y);
				x2 = SimpleMath.round(b.x);
				y2 = SimpleMath.round(b.y);

				int dx = SimpleMath.abs(x2 - x1);
				int dy = SimpleMath.abs(y2 - y1);
				
				if (dx > dy)
					if (x1 < x2)	bresenham_x(x1, y1, x2, y2, context);
					else			bresenham_x(x2, y2, x1, y1, context);
				else
					if (y1 < y2)	bresenham_y(y1, x1, y2, x2, context);
					else			bresenham_y(y2, x2, y1, x1, context);
		}

		void setPixel(float x, float y, Chunk context)
		{

			int X = (int)x;
			int Y = (int)y;
			int index = (int)(x+Chunk.max_w*(context.h-Y));
			if (x >= 0 && x < context.w && y >= 0 && y < context.h /*&& z < context.depth[index]*/)
			{
				context.lastSetPixelIndex			= index;
				Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
				Chunk.buffer.newPixels(X, context.h-Y, 1, 1);
			}
		}

		void setPixel_x( Chunk context )
		{
			context.lastSetPixelIndex += 1;
			if (context.lastSetPixelIndex > context.w_h) return;
		//	{
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
		}

		void setPixel_y(Chunk context)
		{
			context.lastSetPixelIndex += context.w;
			if (context.lastSetPixelIndex > context.w_h) return;
			//{
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
			//}
		}

		void setPixel_xy(Chunk context)
		{
			context.lastSetPixelIndex += (context.w + 1);
			if (context.lastSetPixelIndex > context.w_h) return;
			//{
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
			//}
				//buffer[lastSetPixelIndex] = color;
		}

		void setPixel_mxy(Chunk context)
		{
			context.lastSetPixelIndex += (context.w - 1);
			if (context.lastSetPixelIndex > context.w_h) return;
		//	{
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
			//}
				//buffer[lastSetPixelIndex] = color;
		}

		void setPixel_xmy(Chunk context)
		{
			context.lastSetPixelIndex += (1 - context.w);
			if (context.lastSetPixelIndex > context.w_h) return;
		//	{
				//int offset = (x+w*y)*3;

			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
			//}
				//buffer[lastSetPixelIndex] = color;
		}

		public final static DrawingLibraryFlat instance = new DrawingLibraryFlat();
	//POINTS

		public void drawPoints( Chunk context )
		{
			try
			{
				switch(context.size)
				{
					case 1 :
					{
						for(Vertex v_it : context.vertices)
							setPixel(v_it.x, v_it.y, context);
					} break;
					case 2 :
					{
						for(Vertex v_it : context.vertices)
						{
							setPixel(v_it.x  , v_it.y-1, context);
							setPixel(v_it.x  , v_it.y  , context);
							setPixel(v_it.x-1, v_it.y-1, context);
							setPixel(v_it.x-1, v_it.y  , context);
						}
					} break;
					case 3 :
					{
						for(Vertex v_it : context.vertices)
						{
							setPixel(v_it.x+1, v_it.y-1, context);
							setPixel(v_it.x+1, v_it.y  , context);
							setPixel(v_it.x+1, v_it.y+1, context);
						
							setPixel(v_it.x-1, v_it.y-1, context);
							setPixel(v_it.x-1, v_it.y  , context);
							setPixel(v_it.x-1, v_it.y+1, context);
						
							setPixel(v_it.x  , v_it.y-1, context);
							setPixel(v_it.x  , v_it.y  , context);
							setPixel(v_it.x  , v_it.y+1, context);
						}
					} break;
				default :
				{
					byte thickness	= (byte)((((context.size%2)==0)?((context.size+1)):(context.size))>>1);
					byte s			= (byte)(context.size-1);
					
					for(Vertex v_it : context.vertices)
					for(int i = -thickness; i<s; i++)
					for(int j = -thickness; j<s; j++)
						setPixel(v_it.x+j, v_it.y+i, context);
				} break;
			
			}
			}catch(Throwable t){ System.err.println(t.getMessage()); }
		}

	//-----------------------------------------------------------------------------

	//LINES

		public void drawLines( Chunk context )
		{
			
			try
			{
				int size =  context.vertices.size();
				for (int i = 0; i < size; i += 2)
					drawLine2D(context.vertices.get(i), context.vertices.get(i+1), context);
				
			}catch(Throwable t){/*ignore*/}
		}

		public void drawLineStrip( Chunk context )
		{
			try
			{
				int size = context.vertices.size()-1;
				
				for (int i = 0; i < size; i++)
					drawLine2D(context.vertices.get(i), context.vertices.get(i+1), context);
				
			}catch(Throwable t){ System.err.println(t.getMessage()); }
		}

		public void drawLineLoop( Chunk context )
		{

			try
			{
				int size = context.vertices.size()-1;
			
				for (int i = 0; i < size; i++)
					drawLine2D(context.vertices.get(i), context.vertices.get(i+1), context);
				
				drawLine2D(context.vertices.get(size), context.vertices.get(0), context);
			}catch(Throwable t){/*ignore*/}
		}

		public void drawPolygon( Chunk context )
		{
				try
				{
					int size = context.vertices.size()-1;
					for (int i = 0; i < size; i++)
						drawLine2D(context.vertices.get(i), context.vertices.get(i+1), context);

					drawLine2D(context.vertices.get(size), context.vertices.get(0), context);
				}
				catch(Throwable t)
				{/* ignore */}
		}

		public void drawCircle( Vertex v,float r, Chunk context)
		{

			try
			{
			int x = 0;
			int y = (int)r;
			int p = 3 - ((int)r << 1);

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
			catch(Throwable t){System.err.println(t.getMessage()); }
		}

	//-----------------------------------------------------------------------------

	//FILL

		public void fillCircle( Vertex v,float r,Chunk context )
		{
			int x = 0;
			int y = (int)r;
			int p = 3 - ((int)r << 1);

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

		public void fillPolygon( Chunk context )
		{
			/*
			boolean depth	= true;

		    EdgeStack tableEdges = new EdgeStack[context.h];

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
						else		setPixel(from, y, z1, context);
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
								setPixel(from, y, z1, context);
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
			    */
		}


		public void bresenham_x(int x1, int y1, int x2, int y2, Chunk context)
		{
			int dx = x2 - x1;
			int dy = y2 - y1;

			int sign = 1;
			if (dy < 0)
				sign = -1;
			int c0, c1, p;
			c0 = (dy << 1) * sign;
			c1 = c0 - (dx << 1);
			p = c0 - dx;

			setPixel(x1, y1, context);
			for (int i = x1 + 1; i <= x2; ++i)
			{
			if (p < 0)
				p += c0;
			else
			{
				p += c1;
				y1 += sign;
			}

			setPixel(i, y1, context);
		}
		}

		void bresenham_y(int x1, int y1, int x2, int y2, Chunk context)
		{
			int dx = x2 - x1;
			int dy = y2 - y1;

			int sign = 1;
			if (dy < 0)
				sign = -1;
			int c0, c1, p;
			c0 = (dy << 1) * sign;
			c1 = c0 - (dx << 1);
			p = c0 - dx;

			setPixel(y1, x1, context);
			for (int i = x1 + 1; i <= x2; ++i)
			{
				if (p < 0)
					p += c0;
				else
				{
					p += c1;
					y1 += sign;
				}

				setPixel(y1, i, context);
			}
		}

}