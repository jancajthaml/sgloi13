package rabbit.gl.engine;

import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglVertex2f;
import static rabbit.gl.type.sglEElementType.SGL_LINE_STRIP;
import static rabbit.gl.type.sglEElementType.SGL_TRIANGLES;
import static rabbit.gl.type.sglEElementType.SGL_POINTS;
import static rabbit.gl.type.sglEElementType.SGL_POLYGON;
import static rabbit.gl.type.sglEMatrixMode.SGL_MODELVIEW;
import java.util.ArrayDeque;
import java.util.Arrays;

import rabbit.gl.helpers.DrawingLibraryBase;
import rabbit.gl.helpers.DrawingLibraryDepth;
import rabbit.gl.helpers.DrawingLibraryFlat;
import rabbit.gl.math.SimpleMath;
import rabbit.gl.struct.Color;
import rabbit.gl.struct.Matrix;
import rabbit.gl.struct.Vertex;
import rabbit.gl.struct.Viewport;
import rabbit.gl.type.sglEAreaMode;
import rabbit.gl.type.sglEElementType;
import rabbit.gl.type.sglEMatrixMode;

public class Context
{
	public int id							= 0;

	public Chunk storage;
	static Vertex vertex_calculation_helper	= new Vertex (0,0,0,1);

	static DrawingLibraryBase g = new DrawingLibraryBase();
	
	boolean BEGIN;
	
	boolean P_changed;
	boolean M_changed;

	public Matrix current_M;
	public Matrix current_P;
	
	public Matrix MP;
	public Matrix MVP;
	
	public Viewport viewport = new Viewport();
	sglEAreaMode drawType;	// Drawing mode
	public sglEMatrixMode matrixMode;

	public ArrayDeque <sglEElementType> types	= new ArrayDeque<sglEElementType>();		
	public ArrayDeque <Matrix> P_stack			= new ArrayDeque<Matrix>();

	public Context(int width, int height)
	{
		storage        = new Chunk();
		storage.w      = width;
		storage.h      = height;
		storage.w_h    = width * height;
		storage.color  = new Color(0,255,0);
		storage.size   = 1;

		id             = 0;
		BEGIN		   = false;

		current_M      = Matrix.identity();
		current_P      = Matrix.identity();

		MP             = Matrix.identity();
		MVP            = Matrix.identity();
		
		P_changed      = true;
		M_changed      = true;

		drawType       = sglEAreaMode.SGL_FILL;
		
		this.disableDepthTest();
	}
	
	public void setVertex2f(float x, float y)
	{ storage.vertices.push(create(x,y,0.0f,1.0f)); }
	
	public void setVertex3f(float x, float y, float z)
	{ storage.vertices.push(create(x,y,z,1.0f)); }

	public void setVertex4f(float x, float y, float z, float w)
	{ storage.vertices.push(create(x,y,z,w)); }

	public void rasterize()
	{
		try
		{
			if (types.size() == 0);

			sglEElementType type = types.peekFirst();
			
			if(type==null) return;
			
			switch( drawType )
			{
            	case SGL_POINT          : g.drawPoints ( storage )    ; break;

            	default                 : switch( type )	//LINES of FILLING
            	{
                	case SGL_POINTS     : g.drawPoints    ( storage ) ; break;
                	case SGL_LINES      : g.drawLines     ( storage ) ; break;
                	case SGL_LINE_STRIP : g.drawLineStrip ( storage ) ; break;
                	case SGL_LINE_LOOP  : g.drawLineLoop  ( storage ) ; break;
                	case SGL_TRIANGLES  : switch( drawType )  //TRIANGLE LINE/FILL
                	{
                		case SGL_LINE   : g.drawPolygon      ( storage ) ; break;	//FIXME TO DRAW TRIANGLE FAN in future
                		default         : g.fillTrianglesFan ( storage ) ; break;
                	}
                	break;
                	case SGL_POLYGON    : switch( drawType )  //POLYGON LINE/FILL
                	{
                    	case SGL_LINE   : g.drawPolygon   ( storage ) ; break;
                    	default         : g.fillPolygon   ( storage ) ; break;
                	}
                	break;

                	case SGL_AREA_LIGHT        : 					              ; break;
                	case SGL_LAST_ELEMENT_TYPE : 					              ; break;
                	default                    :                                    break;
            	}
            	break;
			}
		}
		catch(Throwable t){ t.printStackTrace(); }
        storage.vertices.clear();
	}

	private Vertex create(float x, float y, float z, float w)
	{
		vertex_calculation_helper.x = x;
		vertex_calculation_helper.y = y;
		vertex_calculation_helper.z = z;
		vertex_calculation_helper.w = w;
		
		checkPMVMatrix();

		Vertex v = MVP.multiply(vertex_calculation_helper);
		
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
		
		return  v;
	}

	public float calculateRadiusScaleFactor()
	{
		checkPMVMatrix();
		return SimpleMath.sqrt(((MVP.matrix[0] * MVP.matrix[5]) - (MVP.matrix[1] * MVP.matrix[4])));
	}

	public void drawEllipse(float center_x, float center_y, float center_z, float axis_x, float axis_y)
	{
		sglBegin(SGL_POLYGON);

		sglVertex2f( center_x										, center_y + axis_y									 );
		sglVertex2f( center_x + (axis_x * 0.1564344693575539f	)	, center_y + ( axis_y * 0.987688339911341f			));
		sglVertex2f( center_x + (axis_x * 0.3090170026893479f	)	, center_y + ( axis_y * 0.9510565135936411f			));
				sglVertex2f( center_x + (axis_x * 0.45399051142368685f	)	, center_y + ( axis_y * 0.8910065182350011f			));
				sglVertex2f( center_x + (axis_x * 0.587785266437776f		)	, center_y + ( axis_y * 0.8090169840977831f		));
				sglVertex2f( center_x + (axis_x * 0.7071067966408575f	)	, center_y + ( axis_y * 0.7071067657322372f			));
				sglVertex2f( center_x + (axis_x * 0.8090170097906934f	)	, center_y + ( axis_y * 0.5877852310745185f			));
				sglVertex2f( center_x + (axis_x * 0.891006511019614f		)	, center_y + ( axis_y * 0.4539905255846804f		));
				sglVertex2f( center_x + (axis_x * 0.9510565271012029f	)	, center_y + ( axis_y * 0.3090169611173454f			));
				sglVertex2f( center_x + (axis_x * 0.9876883560735247f	)	, center_y + ( axis_y * 0.15643436731351018f		));
				sglVertex2f( center_x + (axis_x * 0.999999999999999f		)	, center_y - ( axis_y * 4.371139000186241E-8f	));
				sglVertex2f( center_x + (axis_x * 0.9876883423975937f	)	, center_y - ( axis_y * 0.15643445365997144f		));
				sglVertex2f( center_x + (axis_x * 0.9510565000860774f	)	, center_y - ( axis_y * 0.30901704426134974f		));
				sglVertex2f( center_x + (axis_x * 0.8910064713304968f	)	, center_y - ( axis_y * 0.4539906034789449f			));
				sglVertex2f( center_x + (axis_x * 0.8090170284743339f	)	, center_y - ( axis_y * 0.5877852053586913f			));
				sglVertex2f( center_x + (axis_x * 0.7071067769704655f	)	, center_y - ( axis_y * 0.7071067854026294f			));
				sglVertex2f( center_x + (axis_x * 0.5877851957112599f	)	, center_y - ( axis_y * 0.809017035483602f			));
				sglVertex2f( center_x + (axis_x * 0.4539903804212881f	)	, center_y - ( axis_y * 0.8910065849840472f			));
				sglVertex2f( center_x + (axis_x * 0.3090168061705656f	)	, center_y - ( axis_y * 0.9510565774464436f			));
				sglVertex2f( center_x + (axis_x * 0.15643444188190603f	)	, center_y - ( axis_y * 0.9876883442630557f			));
				sglVertex2f( center_x - (axis_x * 8.742278000372475E-8f	)	, center_y - ( axis_y * 0.9999999999999962f			));
				sglVertex2f( center_x - (axis_x * 0.1564346145748253f	)	, center_y - ( axis_y * 0.9876883169111731f			));
				sglVertex2f( center_x - (axis_x * 0.3090169724585808f	)	, center_y - ( axis_y * 0.9510565234162125f			));
				sglVertex2f( center_x - (axis_x * 0.4539905362098265f	)	, center_y - ( axis_y * 0.8910065056058313f			));
				sglVertex2f( center_x - (axis_x * 0.5877853371642876f	)	, center_y - ( axis_y * 0.8090169327119581f			));
				sglVertex2f( center_x - (axis_x * 0.7071069006049366f	)	, center_y - ( axis_y * 0.7071066617681382f			));
				sglVertex2f( center_x - (axis_x * 0.8090171312459549f	)	, center_y - ( axis_y * 0.5877850639056469f			));
				sglVertex2f( center_x - (axis_x * 0.8910066589484567f	)	, center_y - ( axis_y * 0.4539902352578838f			));
				sglVertex2f( center_x - (axis_x * 0.951056480440929f	)	, center_y - ( axis_y * 0.3090171047228822f			));
				sglVertex2f( center_x - (axis_x * 0.9876883324525811f	)	, center_y - ( axis_y * 0.156434516450301f			));
				sglVertex2f( center_x - (axis_x * 0.9999999999999999f	)	, center_y + ( axis_y * 1.1924880454806035E-8f		));
				sglVertex2f( center_x - (axis_x * 0.9876883287216551f	)	, center_y + ( axis_y * 0.15643454000643153f		));
				sglVertex2f( center_x - (axis_x * 0.9510564730709448f	)	, center_y + ( axis_y * 0.30901712740535175f		));
				sglVertex2f( center_x - (axis_x * 0.8910064316413727f	)	, center_y + ( axis_y * 0.45399068137320586f		));
				sglVertex2f( center_x - (axis_x * 0.8090168369495607f	)	, center_y + ( axis_y * 0.5877854689698682f			));
				sglVertex2f( center_x - (axis_x * 0.7071065465657921f	)	, center_y + ( axis_y * 0.7071070158072251f			));
				sglVertex2f( center_x - (axis_x * 0.5877849321000184f	)	, center_y + ( axis_y * 0.8090172270082862f			));
				sglVertex2f( center_x - (axis_x * 0.45399051495953424f	)	, center_y + ( axis_y * 0.8910065164333968f			));
				sglVertex2f( center_x - (axis_x * 0.30901694977611f		)	, center_y + ( axis_y * 0.9510565307861931f			));
				sglVertex2f( center_x - (axis_x * 0.1564343555354446f	)	, center_y + ( axis_y * 0.9876883579389858f			));

				sglEnd();
	}
	public void drawArc2D(float x, float y, float z, float r, float from, float to)
	{
		try
		{
			float x2			= 0.0f					;
			float y2			= 0.0f					;
			float f				= to - from				;
			float N				= f * 12.732395447351626861510701069801148962756771659236515f;
			float alpha			= f / N					;
			float offset = from / alpha			;
			float from_offset	= (offset - 1)*alpha	;
			float x1			= r * SimpleMath.cos(from_offset)	;
			float y1			= r * SimpleMath.sin(from_offset)	;
			float sa			= SimpleMath.sin(alpha)			;
			float ca			= SimpleMath.cos(alpha)			;

			int RR     = (int) (SimpleMath.round(N)+offset);

			switch( drawType )
			{
				case SGL_POINT  :
				{
					sglBegin(SGL_POINTS);

				}
				break;

				case SGL_LINE   :
				{
					sglBegin(SGL_LINE_STRIP);
				}
				break;

				default         :
				{
					//Fill Arct bi triangle fan (can we do it better ? )
					//Artefacts appear in polygon fill too
					//SGL_POLYGON is SLOWER!
					sglBegin(SGL_TRIANGLES);

					//Center
					setVertex2f(x, y);

				}break;
			}

			for( int i = (int)offset; i <= RR; i++ )
			{
				x2 = ca * x1 - sa * y1;
				y2 = sa * x1 + ca * y1;

				setVertex2f(x2 + x, y2 + y);

				x1 = x2;
				y1 = y2;
			}

			sglEnd();
		}
		catch(Throwable t){ System.err.println(t.getMessage()); }
	}


	int stackSize()
	{ return types.size(); }

	public void multiplyCurrentMatrix(Matrix m)
	{
		if (matrixMode == SGL_MODELVIEW)
		{
			M_changed	= true;
			current_M	= current_M.multiply(m);
		}
		else
		{
			P_changed	= true;
			current_P	= current_P.multiply(m);
		}
	}

	public void setViewport(int width, int height, int x, int y)
	{ viewport.changeViewport(width, height, x, y); }

	public Matrix getCurrentMatrix()
	{ return (matrixMode == SGL_MODELVIEW) ? current_M : current_P; }

	public void pushMatrix()
	{
		if(matrixMode==null) return;
		//switch is faster than if 
		switch(matrixMode)
		{
			case SGL_MODELVIEW : P_stack.push(current_M); break;
			default :            P_stack.push(current_P); break;
		}
		//if (matrixMode == SGL_MODELVIEW)	projectionStack.push(currentModelviewMatrix);//push_back
		//else 								projectionStack.push(currentProjectionMatrix);//push_back
	}

	public void setCurrentMatrix(Matrix matrix)
	{
		if(matrixMode==null) return;
		switch(matrixMode)
		{
			case SGL_MODELVIEW :
			{
				M_changed = true;
				current_M = matrix;
			}
			break;
			
			default :
			{
				P_changed = true;
				current_P = matrix;
			}
			break;
		}
	}

	public void setMatrixMode(sglEMatrixMode mode)
	{ matrixMode = mode; }

	public boolean invalidTypeStack()
	{ return (types.size() > 0); }

	public boolean stackEmpty()
	{ return (P_stack.size() == 0); }

	public void popMatrix()
	{
		try
		{
			if (P_stack.size() == 0)
			{
			//*err = SGL_STACK_UNDERFLOW;
		       	return;
			}
			if (matrixMode == SGL_MODELVIEW)
			{
				M_changed	= true;
				current_M	= P_stack.pop();
			}
			else
			{
				P_changed	= true;
				current_P	= P_stack.pop();
			}
		}
		catch(Throwable t)
		{
			System.err.println(t.getMessage());
		}
	}

	public void pushTypeState(sglEElementType type)
	{ types.push(type); }

	public void clearColorBuffer()
	{
		System.arraycopy(Chunk.clearbuff, 0, Chunk._pixels, 0, Chunk.max_w*Chunk.max_h);
		return;
	}
	
	public void clearDepthBuffer()
	{
		Arrays.fill(Chunk.depth,Float.MAX_VALUE);
	}

	boolean checkPMMatrix()
	{
		if (M_changed || P_changed)
		{
			M_changed	= false;
			P_changed	= false;
			MP			= current_P.multiply(current_M);
			return true;
		}
		return false;
	}

	void checkPMVMatrix()
	{
		if (checkPMMatrix() || viewport.viewportMatrixChanged)
		{
			viewport.viewportMatrixChanged	=  false;
			MVP								=  viewport.getViewportMatrix().multiply(MP);
		}
	}

	public void drawCricle(float x, float y, float z, float r)
	{
		
		switch( drawType )
		{
			case SGL_POINT  : g.drawPoints ( storage                                                            ) ; break;
			case SGL_LINE   : g.drawCircle ( create(x , y ,0.0f, 1.0f), r*calculateRadiusScaleFactor(), storage ) ; break;
			default         : g.fillCircle ( create(x , y ,0.0f, 1.0f), r*calculateRadiusScaleFactor(), storage ) ; break;
		}	
	}
	
	public void enableDepthTest()
	{ g.set(DrawingLibraryDepth.instance); }

	public void disableDepthTest()
	{ g.set(DrawingLibraryFlat.instance); }

}