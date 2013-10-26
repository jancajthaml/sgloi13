package rabbit.gl.helpers;

import rabbit.gl.engine.Chunk;
import rabbit.gl.math.SimpleMath;
import rabbit.gl.struct.Edge;
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
			int index = (int)(x+Chunk.max_w*(context.h-((int)y)));
			if (x >= 0 && x < context.w && y >= 0 && y < context.h /*&& z < context.depth[index]*/)
			{
				context.lastSetPixelIndex			= index;
				Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
			}
		}

		void setPixel_x( Chunk context )
		{
			context.lastSetPixelIndex += 1;
			if (context.lastSetPixelIndex > context.w_h) return;
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
		}

		void setPixel_y(Chunk context)
		{
			context.lastSetPixelIndex += context.w;
			if (context.lastSetPixelIndex > context.w_h) return;
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
		}

		void setPixel_xy(Chunk context)
		{
			context.lastSetPixelIndex += (context.w + 1);
			if (context.lastSetPixelIndex > context.w_h) return;
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
		}

		void setPixel_mxy(Chunk context)
		{
			context.lastSetPixelIndex += (context.w - 1);
			if (context.lastSetPixelIndex > context.w_h) return;
			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();
		}

		void setPixel_xmy(Chunk context)
		{
			context.lastSetPixelIndex += (1 - context.w);
			if (context.lastSetPixelIndex > context.w_h) return;

			Chunk._pixels[context.lastSetPixelIndex]	= context.color.getRGB();

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
			}catch(Throwable t){ t.printStackTrace(); }
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
				
			}catch(Throwable t){ t.printStackTrace(); }
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
			catch(Throwable t){ t.printStackTrace(); }
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
			int size = context.vertices.size();

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
			    	edges[i].max_y  =  y[i-1]    ;
			    	edges[i].x      =  x[i]      ;
			    }
			    else
			    {
			    	edges[i].min_y  =  y[i-1]-1 ;
			    	edges[i].max_y  =  y[i]   ;
			    	edges[i].x      =  x[i-1]   ;
			    }

			    edges[i].delta_x = (float)(x[i]-x[i-1]) / (float)(y[i]-y[i-1]);
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

			  edges[0].delta_x   = (float)(x[0]-x[size-1])/(float)(y[0]-y[size-1]);

			  float[] draw =  new float[size];
			  int   count  =  0;

			  for( int Y=min_y ; Y<max_y ; Y++ )
			  {
				  count = 0;

				  for( int v=0 ; v<size ; v++ )
				  {
					  if( (edges[v].min_y<Y) & (edges[v].max_y>Y) )
					  {
						  edges[v].x  +=  edges[v].delta_x;
						  draw[count]  =  edges[v].x;
						  count++;
					  }
				  }

				  int i = 1;
				  int j = 2;
				  float t = 0.0f;
				  
				  while( i<count )
				  {
					  if( draw[i - 1] > draw[i] )
					  {
						  t = draw[i];
						  draw[i] = draw[--i];
						  draw[i] = t;
						  if(i>0)continue;
					  }
					  i = j++;
				  }

				  for( i=0; i<count; i=i+2 )
					  setPixelChunk(Y+1, (int)draw[i], (int)(draw[i+1]-1), context);
			  }
			  
			  context.vertices.clear();
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
		
		public void fillTrianglesStrip( Chunk context )
		{
			try
			{
				//Triangle fan
				int size = context.vertices.size()-1;
			
				//if(size<2) return;
				
				Vertex A	= context.vertices.get(0);
				Vertex B	= context.vertices.get(1);
				Vertex C	= context.vertices.get(2);
				
				for (int i = 2; i <= size; i++)
				{
					drawTriangle(A, B, C, context);
					A  = B;
					B  = C;
					C  = context.vertices.get(i);
				}
			}catch(Throwable t){/*ignore*/}
		}

		public void fillTrianglesFan( Chunk context )
		{
			try
			{
				//Triangle fan
				int size = context.vertices.size()-1;
			
			//	if(size<2) return;
				
				Vertex center	= context.vertices.get(0);
				Vertex A		= context.vertices.get(1);
				Vertex B		= context.vertices.get(2);
				
				for (int i = 2; i <= size; i++)
				{
					drawTriangle(center, A, B, context);
					A	= B;
					B	= context.vertices.get(i);
				}

				drawTriangle(center, A, B, context);

			}catch(Throwable t){/*ignore*/}
		}

		private void drawTriangle(Vertex v0, Vertex v1, Vertex v2, Chunk context)
		{

			int x0 = (int)v0.x;
			int y0 = (int)v0.y;
			int x1 = (int)v1.x;
			int y1 = (int)v1.y;
			int x2 = (int)v2.x;
			int y2 = (int)v2.y;

			if (y0 > y2)
			{
				int tmp=x2;
				x2=x0;
				x0=tmp;
				
				tmp=y2;
				y2=y0;
				y0=tmp;
			}
			if (y1 > y2)
			{
				int tmp=x2;
				x2=x1;
				x1=tmp;
				
				tmp=y2;
				y2=y1;
				y1=tmp;
			}
			if (y0 > y1)
			{
				int tmp=x1;
				x1=x0;
				x0=tmp;
				
				tmp=y1;
				y1=y0;
				y0=tmp;
			}

		  float xs	= 0.0f;
		  float xe	= 0.0f;
		  float dxs	= 0.0f;
		  float dxe	= 0.0f;
		  float xm	= (x0)+(x2-x0)/(float)(y2-y0)*(float)(y1-y0);

		  if (x1<xm)
		  {
			  dxs = (x1-x0)/(float)(y1-y0);
			  dxe = (x2-x0)/(float)(y2-y0);
		  }
		  else
		  {
			  dxs = (x2-x0)/(float)(y2-y0);
			  dxe = (x1-x0)/(float)(y1-y0);
		  }

		  xs = x0;
		  xe = x0;

		  for(int y=y0;y<y1;y++)
		  {
		      setPixelChunk(y,(int)xs+1,(int)xe, context);
		      xs+=dxs;
		      xe+=dxe;
		  }

		  if (x1<xm)
		  {
			  xs=x1;
			  xe=xm;
			  dxs = (x2-x1)/(float)(y2-y1);
			  dxe = (x2-xm)/(float)(y2-y1);
		  }
		  else
		  {
			  xe  = x1;
			  xs  = xm;
			  dxe = (x2-x1)/(float)(y2-y1);
			  dxs = (x2-xm)/(float)(y2-y1);
		  }

		  for(int y=y1;y<y2;y++)
		  {
			  setPixelChunk(y,(int)xs+1,(int)xe,context);
			  xs+=dxs;
			  xe+=dxe;
		  }
		}
		
		private void setPixelChunk(int y, int start, int end, Chunk context)
		{
			try{
			y = context.h-y;
			int size					= end-start+1;
			int n						= size;
			context.lastSetPixelIndex	= (start + Chunk.max_w * (y));
			int rgb						= context.color.getRGB();
			
			while( n >= 8 )
			{
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				n -= 8;
			}

			switch( n )
			{
				case 8 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				case 7 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				case 6 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				case 5 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				case 4 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				case 3 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				case 2 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
				case 1 : Chunk._pixels[context.lastSetPixelIndex++]	= rgb;
			}
			}catch(Throwable t){context.lastSetPixelIndex--;}
		}

}