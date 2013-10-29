package rabbit.gl.helpers;

import rabbit.gl.engine.Chunk;
import rabbit.gl.struct.Color;
import rabbit.gl.struct.Edge;
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
		
		public void drawBezier( Vertex a, Vertex b, Vertex c, Chunk context )
		{
			//FIXME incorporate z COORDINATE
			DrawingLibraryFlat.instance.drawBezier(a, b, c, context);
		}
		
		private void setPixel(float x, float y, float z, Chunk context)
		{
			int index = (int)(x+Chunk.max_w*(context.h-((int)y)));
			if (x >= 0 && x < context.w && y >= 0 && y < context.h && Chunk.depth[index] > z)
			{
				context.lastSetPixelIndex					= index;
				Chunk.depth[index]							= z;
				renderPixel(index, context.color.a, context);
			}
		}

		private void renderPixel(int index, float alpha, Chunk context)
		{
			Chunk._pixels[index]	= Color.mix(context.color,Chunk._pixels[index],alpha);
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
			}catch(Throwable t)
			{ System.err.println(t.getMessage()); }
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

		public void drawLineBezier( Chunk context )
		{
			DrawingLibraryFlat.instance.drawLineBezier(context);
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
			int size		= context.vertices.size();
			if(size==0)
			{
				System.err.println("ZERO");
				return;
			}
			float delta		= 0.0f;
			int[] x			= new int[size];
			int[] y			= new int[size];
			Edge[] edges	= new Edge[size];
			
			for(int i=0; i<size; i++) edges[i]=new Edge();

			x[0] = (int)(context.vertices.get(0).x+1.0f);
			y[0] = (int)(context.vertices.get(0).y+1.0f);

			int    min_y  =  y[0]  ;
			int    max_y  =  y[0]  ;

			for( int i=1; i<size; i++ )
			{
			    x[i] = (int)(context.vertices.get(i).x+1.0f);
			    y[i] = (int)(context.vertices.get(i).y+1.0f);

			    if( y[i]<min_y )  min_y = y[i];
			    if( y[i]>max_y )  max_y = y[i];

				if( y[i] < y[i-1] )
				{
					edges[i].min_y  =  y[i]-1 ;
					edges[i].max_y  =  y[i-1]   ;
					edges[i].x      =  x[i];
					edges[i].z      =  context.vertices.get(i)   . z   ;
				}
				else
				{
					edges[i].min_y  =  y[i-1]-1 ;
					edges[i].max_y  =  y[i]  ;
					edges[i].x      =  x[i-1]   ;
					edges[i].z      =  context.vertices.get(i-1) . z   ;
				}

				delta            = (float)(y[i]-y[i-1]);
				edges[i].delta_x = (float)(x[i]-x[i-1]) / delta;
				edges[i].delta_z = (context.vertices.get(i).z-context.vertices.get(i-1).z) / delta;
			  }

			  if( y[0] < y[size-1] )
			  {
				  edges[0].min_y  =  y[0]-1 ;
				  edges[0].max_y  =  y[size-1]  ;
				  edges[0].x      =  x[0] ;
				  edges[0].z      =  context.vertices.get(0)      . z   ;
			  }
			  else
			  {
				  edges[0].min_y  =  y[size-1]-1 ;
				  edges[0].max_y  =  y[0]   ;
				  edges[0].x      =  x[size-1]  ;
				  edges[0].z      =  context.vertices.get(size-1) . z   ;
			  }

			  delta             =  (float)(y[0]-y[size-1]);
			  edges[0].delta_x  =  (float)(x[0]-x[size-1]) / delta;
			  edges[0].delta_z  =  (context.vertices.get(0).z-context.vertices.get(size-1).z) / delta;

			  float[] draw   =  new float[size] ;
			  float[] drawZ  =  new float[size] ;
			  int     count  =  0;

			  for( int Y=min_y ; Y<max_y ; Y++ )
			  {
				  count = 0;
				  for( int v=0 ; v<size ; v++ )
				  {
					  if( (edges[v].min_y<Y) & (edges[v].max_y>Y) )
					  {
						  edges[v].x  += edges[v].delta_x;
						  edges[v].z  += edges[v].delta_z;
						  draw[count]  = edges[v].x;
						  drawZ[count] = edges[v].z;

						  count++;
					  }
				  }

				  float m = 0.0f;
				  int i	= 1;
				  int j	= 2;
				  float t = 0.0f;
				  
				  while( i < count )
				  {
					  if( draw[i - 1] > draw[i] )
					  {
						  t			= draw[i];
						  m			= drawZ[i];

						  draw[i]	= draw[i-1];
						  drawZ[i]	= drawZ[--i];

						  draw[i]	= t;
						  drawZ[i]	= m;
						  if(i>0)continue;
					  }
					  i = j++;
				  }

				  for( i=0 ; i<count ; i=i+2 )
					  setPixelChunk( Y+1, (int)draw[i], (int)draw[i+1], drawZ[i], (drawZ[i+1]-drawZ[i])/(draw[i+1]-draw[i]), context );
			  }

			  context.vertices.clear();
		}

		private void setPixelChunk( int y, int start, int end, float z, float z_growth, Chunk context )
		{
			 for( int x=start ; x<end ; x++ )
			 {
				 setPixel(x,y,z,context);
				 z+=z_growth;
			 }
		}
		
		@Override public void fillTrianglesFan(Chunk context)
		{
			DrawingLibraryFlat.instance.fillTrianglesFan(context);
		}

		@Override public void fillTrianglesStrip(Chunk context)
		{
			DrawingLibraryFlat.instance.fillTrianglesStrip(context);
			
		}

}