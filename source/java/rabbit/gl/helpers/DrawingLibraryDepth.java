package rabbit.gl.helpers;

import rabbit.gl.engine.Chunk;
import rabbit.gl.struct.Vertex;

public class DrawingLibraryDepth implements DrawingLibraryInterface
{

	private DrawingLibraryDepth()	{}
		//~DrawingLibraryDepth()								{}

		private void fillSymPixel(float x, float y, float center_x, float center_y, Chunk context)
		{
			float to	= center_x+x;
			float from	= center_x-x;

			float h1	= center_y+y;
			float l1	= center_y-y;

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
				setPixel( from, h1, 0,context );
				setPixel( from, l1, 0,context );
			}
		}

		private void setSymPixel(float x, float y, float xs, float ys, Chunk context)
		{
			float rx = x + xs;
			float ry = y + ys;
			float mrx = -x + xs;
			float mry = -y + ys;
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

		public void drawLine2D(Vertex a, Vertex b, Chunk context)
		{
			//FIXME incorporate z COORDINATE
			DrawingLibraryFlat.instance.drawLine2D(a, b, context); }

		private void setPixel(float x, float y, float z, Chunk context)
		{ setPixel((int)x,(int)y,(int)z,context); }

		private void setPixel(int x, int y, int z, Chunk context)
		{

			int index = (x+Chunk.max_w*(context.h-y));
			if (x >= 0 && x < context.w && y >= 0 && y < context.h /*&& z < context.depth[index]*/)
			{
				context.lastSetPixelIndex			= index;
				Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
				Chunk.buffer.newPixels(x, context.h-y, 1, 1);
			}
		}

		/*
		private void setPixel_x( Chunk context )
		{ DrawingLibraryFlat.instance.setPixel_x(context); }

		private void setPixel_y(Chunk context)
		{ DrawingLibraryFlat.instance.setPixel_y(context); }

		private void setPixel_xy(Chunk context)
		{ DrawingLibraryFlat.instance.setPixel_xy(context); }

		private void setPixel_mxy(Chunk context)
		{ DrawingLibraryFlat.instance.setPixel_mxy(context); }

		private void setPixel_xmy(Chunk context)
		{ DrawingLibraryFlat.instance.setPixel_xmy(context); }
*/
		public final static DrawingLibraryDepth instance = new DrawingLibraryDepth();
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
							setPixel(v_it.x, v_it.y, v_it.z, context);
					} break;
					case 2 :
					{
						for(Vertex v_it : context.vertices)
						{
							setPixel(v_it.x  , v_it.y-1, v_it.z, context);
							setPixel(v_it.x  , v_it.y  , v_it.z, context);
							setPixel(v_it.x-1, v_it.y-1, v_it.z, context);
							setPixel(v_it.x-1, v_it.y  , v_it.z, context);
						}
					} break;
					case 3 :
					{
						for(Vertex v_it : context.vertices)
						{
							setPixel(v_it.x+1, v_it.y-1, v_it.z, context);
							setPixel(v_it.x+1, v_it.y  , v_it.z, context);
							setPixel(v_it.x+1, v_it.y+1, v_it.z, context);
						
							setPixel(v_it.x-1, v_it.y-1, v_it.z, context);
							setPixel(v_it.x-1, v_it.y  , v_it.z, context);
							setPixel(v_it.x-1, v_it.y+1, v_it.z, context);
						
							setPixel(v_it.x  , v_it.y-1, v_it.z, context);
							setPixel(v_it.x  , v_it.y  , v_it.z, context);
							setPixel(v_it.x  , v_it.y+1, v_it.z, context);
						}
					} break;
				default :
				{
					byte thickness	= (byte)((((context.size%2)==0)?((context.size+1)):(context.size))>>1);
					byte s			= (byte)(context.size-1);
					
					for(Vertex v_it : context.vertices)
					for(int i = -thickness; i<s; i++)
					for(int j = -thickness; j<s; j++)
						setPixel(v_it.x+j, v_it.y+i, v_it.z, context);
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

}