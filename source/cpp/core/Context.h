/*
 * data.h
 *
 *  Created on: 23.9.2013
 *      Author: jancajthaml
 */

#ifndef DATA_H_
#define DATA_H_

#include "./CrossReferenceDispatcher.h"
#include "./../struct/Color.h"
#include "./../struct/Vertex.h"
#include "./../struct/Edge.h"
#include "./../struct/Matrix.h"
#include "./../struct/VertexStack.h"
#include "./../struct/MatrixCache.h"
#include "./Viewport.h"
#include "./../helpers/DrawingLibraryBase.h"
#include "./../helpers/DrawingLibrary_FLAT.h"
#include "./../helpers/DrawingLibrary_DEPTH.h"
#include "./ContextChunk.h"

/*
 * Side-notes:
 * Jan Cajthaml -using struct instead of class because of no need of encapsulation
 *
 * */

/*
 * ChangeLog:
 * 23.9.2013, Jan Cajthaml - added struct Vertex
 * 23.9.2013, Jan Cajthaml - added struct Edge
 * 24.9.2013, Jan Cajthaml - added struct Color
 * 24.9.2013, Jan Cajthaml - added struct Matrix
 * 24.9.2013, Jan Cajthaml - added struct Context
 * 25.9.2013, Jan Cajthaml - added ViewPort & depth flag to Context
 * 25.9.2013, Jan Cajthaml - added struct ContextManager
 * 30.9.2013, Jan Cajthaml - added operator overload for aritmetic operations on Matrix
 * 30.9.2013, Jan Cajthaml - added operator overload for aritmetic operations on Color
 * 30.9.2013, Jan Cajthaml - added Model,Projection,Current and MPV (Model-Projection-View pre-calculated) matricies to Context
 *
 * -----
 *
 * 15.10.2013, Jan Cajthaml - added "begin" flag to determine is sglBegin and sglEnd were called
 * 17.10.2013, Jan Cajthaml - added ContextChunk for data hold bridge
 *                          - added CrossReferenceDispatcher (temporary)
 *                          - added DrawingLibrary helper
 *                          - added Edge
 *                          - added EdgeStack
 *                          - added Helpers for helper functions (like sort)
 * */


//TODO COMMENT !!!!!!!
//Context
struct Context
{

	int_fast8_t id            ;  //Maximum 256 contexts

	Chunk storage             ;  // Graphic dependent data storage
	Viewport viewport         ;

	sglEAreaMode drawType     ;  // Drawing mode
	sglEMatrixMode matrixMode ;

	static DrawingLibraryBase  g;
	static DrawingLibraryFlat  flat;
	static DrawingLibraryDepth depth;

	//State
	bool BEGIN  ;
	bool Z_TEST ;

	//Transformation matrices
	Matrix current_MV ;
	Matrix current_P  ;

	Matrix MP  ;
	Matrix MVP ;

	bool P_changed ;
	bool M_changed ;

	//FIXME don't use a std::vector stack for this, implement custom
	std::vector<sglEElementType> types ;

	//FIXME don't use a std::vector stack for this, implement custom
	std::vector<Matrix> P_stack        ;


	Context(uint_fast16_t width, uint_fast16_t height)
	{
		storage        = Chunk();
		storage.w      = width;
		storage.h      = height;
		storage.w_h    = width * height;
		storage.color  = Color(0,255,0);
		storage.size   = 1;

		//----------------------//

		//Initialise Drawing
		id		 	  = 0;

		//----------------------//

		//Initialise Flags
		//storage.depth  = false;
		BEGIN			= false;
		Z_TEST			= false;
		//----------------------//

		storage.size   = 1                                            ;
		storage.buffer = (Color*) malloc(sizeof(Color) * storage.w_h) ;
		storage.depth  = (float*) malloc(sizeof(float) * storage.w_h) ;
		storage.clear  = (Color*) malloc(sizeof(Color) * storage.w_h) ;

		current_MV	   = MatrixCache::identity()                      ;
		current_P      = MatrixCache::identity()                      ;

		P_changed      = true                                         ;
		M_changed      = true                                         ;
		drawType       = SGL_FILL                                     ;
		matrixMode     = SGL_MODELVIEW                                ;

		this->disableDepthTest();
	}

	inline void setVertex2f(float x, float y)
	{ storage.vertices.push_back(create(x, y, 0.0f, 1.0f)); }

	inline void setVertex3f(float x, float y, float z)
	{ storage.vertices.push_back(create(x, y, z, 1.0f)); }

	inline void setVertex4f(float x, float y, float z, float w)
	{ storage.vertices.push_back(create(x, y, z, w)); }

	inline void rasterize()
	{
		sglEElementType type = types.back();
		types.pop_back();

        switch( drawType )
        {
            case SGL_POINT          : g.drawPoints ( storage )       ; break;

            default                 : switch( type )	//LINES of FILLING
            {
                case SGL_POINTS     : g.drawPoints       ( storage ) ; break;
                case SGL_LINES      : g.drawLines        ( storage ) ; break;
                case SGL_LINE_STRIP : g.drawLineStrip    ( storage ) ; break;
                case SGL_LINE_LOOP  : g.drawLineLoop     ( storage ) ; break;
                case SGL_TRIANGLES  : g.fillTrianglesFan ( storage ) ; break;

                case SGL_POLYGON    : switch( drawType )  //POLYGON LINE/FILL
                {
                    case SGL_LINE   : g.drawPolygon      ( storage ) ; break;
                    default         : g.fillPolygon      ( storage ) ; break;
                }
                break;

                case SGL_AREA_LIGHT        : 					     ; break;
                case SGL_LAST_ELEMENT_TYPE : 					     ; break;
                default                    :                         ; break;
            }
            break;
        }

        storage.vertices.index = 0;
	}

	inline Vertex& create(float x, float y, float z, float w)
	{
		Vertex v(x, y, z, w);
		transform(v);
		return v;
	}

	inline Vertex transform(Vertex & v)
	{
		check_MVP();

		//This is a uqly fix to differ the multiplication with Z-TEST and without

		if(Z_TEST)	v = MVP << v ;  //WITH
		else		v = MVP <  v ;  //WITHOUT

		printf("     after transform -> vertex: x=%f, y=%f, z=%f, w=%f\n",v.x,v.y,v.z,v.w);

		return v;
	}

	inline float calculateRadiusScaleFactor()
	{
		check_MVP();
		return sqrt((MVP.matrix[0] * MVP.matrix[5]) - (MVP.matrix[1] * MVP.matrix[4]));
	}

	inline void drawCricle( float x, float y, float z, float r )
	{
		switch( drawType )
		{
			case SGL_POINT  : g.drawPoints ( storage                                                            ) ; break;
			case SGL_LINE   : g.drawCircle ( create(x , y ,0.0f, 1.0f), r*calculateRadiusScaleFactor(), storage ) ; break;
			default         : g.fillCircle ( create(x , y ,0.0f, 1.0f), r*calculateRadiusScaleFactor(), storage ) ; break;
		}

	}

	inline void drawEllipse(float center_x, float center_y, float center_z, float axis_x, float axis_y)
	{
		bool ellipse_adaptive = false;

		if(ellipse_adaptive)
		{

		}
		else
		{
			sglBegin(SGL_POLYGON);

				//Translate center_x,center_y

				//float diff = 0.15707963267f;

				//for(int_fast32_t i = 0; i < 40; ++i)
				//{
				//sglVertex2f(center_x+(axis_x * sin(i*diff)), center_y+(axis_y * cos(i*diff)));
				//}


				// //Translate -center_x,-center_y
				//TRANSLATE is slower than calculation

				sglVertex2f( center_x										, center_y + axis_y								 );
				sglVertex2f( center_x + (axis_x * 0.1564344693575539	)	, center_y + ( axis_y * 0.987688339911341		));
				sglVertex2f( center_x + (axis_x * 0.3090170026893479	)	, center_y + ( axis_y * 0.9510565135936411		));
				sglVertex2f( center_x + (axis_x * 0.45399051142368685	)	, center_y + ( axis_y * 0.8910065182350011		));
				sglVertex2f( center_x + (axis_x * 0.587785266437776		)	, center_y + ( axis_y * 0.8090169840977831		));
				sglVertex2f( center_x + (axis_x * 0.7071067966408575	)	, center_y + ( axis_y * 0.7071067657322372		));
				sglVertex2f( center_x + (axis_x * 0.8090170097906934	)	, center_y + ( axis_y * 0.5877852310745185		));
				sglVertex2f( center_x + (axis_x * 0.891006511019614		)	, center_y + ( axis_y * 0.4539905255846804		));
				sglVertex2f( center_x + (axis_x * 0.9510565271012029	)	, center_y + ( axis_y * 0.3090169611173454		));
				sglVertex2f( center_x + (axis_x * 0.9876883560735247	)	, center_y + ( axis_y * 0.15643436731351018		));
				sglVertex2f( center_x + (axis_x * 0.999999999999999		)	, center_y - ( axis_y * 4.371139000186241E-8	));
				sglVertex2f( center_x + (axis_x * 0.9876883423975937	)	, center_y - ( axis_y * 0.15643445365997144		));
				sglVertex2f( center_x + (axis_x * 0.9510565000860774	)	, center_y - ( axis_y * 0.30901704426134974		));
				sglVertex2f( center_x + (axis_x * 0.8910064713304968	)	, center_y - ( axis_y * 0.4539906034789449		));
				sglVertex2f( center_x + (axis_x * 0.8090170284743339	)	, center_y - ( axis_y * 0.5877852053586913		));
				sglVertex2f( center_x + (axis_x * 0.7071067769704655	)	, center_y - ( axis_y * 0.7071067854026294		));
				sglVertex2f( center_x + (axis_x * 0.5877851957112599	)	, center_y - ( axis_y * 0.809017035483602		));
				sglVertex2f( center_x + (axis_x * 0.4539903804212881	)	, center_y - ( axis_y * 0.8910065849840472		));
				sglVertex2f( center_x + (axis_x * 0.3090168061705656	)	, center_y - ( axis_y * 0.9510565774464436		));
				sglVertex2f( center_x + (axis_x * 0.15643444188190603	)	, center_y - ( axis_y * 0.9876883442630557		));
				sglVertex2f( center_x - (axis_x * 8.742278000372475E-8	)	, center_y - ( axis_y * 0.9999999999999962		));
				sglVertex2f( center_x - (axis_x * 0.1564346145748253	)	, center_y - ( axis_y * 0.9876883169111731		));
				sglVertex2f( center_x - (axis_x * 0.3090169724585808	)	, center_y - ( axis_y * 0.9510565234162125		));
				sglVertex2f( center_x - (axis_x * 0.4539905362098265	)	, center_y - ( axis_y * 0.8910065056058313		));
				sglVertex2f( center_x - (axis_x * 0.5877853371642876	)	, center_y - ( axis_y * 0.8090169327119581		));
				sglVertex2f( center_x - (axis_x * 0.7071069006049366	)	, center_y - ( axis_y * 0.7071066617681382		));
				sglVertex2f( center_x - (axis_x * 0.8090171312459549	)	, center_y - ( axis_y * 0.5877850639056469		));
				sglVertex2f( center_x - (axis_x * 0.8910066589484567	)	, center_y - ( axis_y * 0.4539902352578838		));
				sglVertex2f( center_x - (axis_x * 0.951056480440929		)	, center_y - ( axis_y * 0.3090171047228822		));
				sglVertex2f( center_x - (axis_x * 0.9876883324525811	)	, center_y - ( axis_y * 0.156434516450301		));
				sglVertex2f( center_x - (axis_x * 0.9999999999999999	)	, center_y + ( axis_y * 1.1924880454806035E-8	));
				sglVertex2f( center_x - (axis_x * 0.9876883287216551	)	, center_y + ( axis_y * 0.15643454000643153		));
				sglVertex2f( center_x - (axis_x * 0.9510564730709448	)	, center_y + ( axis_y * 0.30901712740535175		));
				sglVertex2f( center_x - (axis_x * 0.8910064316413727	)	, center_y + ( axis_y * 0.45399068137320586		));
				sglVertex2f( center_x - (axis_x * 0.8090168369495607	)	, center_y + ( axis_y * 0.5877854689698682		));
				sglVertex2f( center_x - (axis_x * 0.7071065465657921	)	, center_y + ( axis_y * 0.7071070158072251		));
				sglVertex2f( center_x - (axis_x * 0.5877849321000184	)	, center_y + ( axis_y * 0.8090172270082862		));
				sglVertex2f( center_x - (axis_x * 0.45399051495953424	)	, center_y + ( axis_y * 0.8910065164333968		));
				sglVertex2f( center_x - (axis_x * 0.30901694977611		)	, center_y + ( axis_y * 0.9510565307861931		));
				sglVertex2f( center_x - (axis_x * 0.1564343555354446	)	, center_y + ( axis_y * 0.9876883579389858		));

			//	}

				sglEnd();
			}
		}


	inline void drawArc2D(float x, float y, float z, float r, float from, float to)
	{
		float x2			= 0.0f					;
		float y2			= 0.0f					;
		float f				= to - from				;
		float N				= f * 12.732395447351626861510701069801148962756771659236515f;
		float alpha			= f / N					;
		float offset = from / alpha			;
		float from_offset	= (offset - 1)*alpha	;
		float x1			= r * cosf(from_offset)	;
		float y1			= r * sinf(from_offset)	;
		float sa			= sinf(alpha)			;
		float ca			= cosf(alpha)			;

		float RR = Helper::round(N)+offset;

		switch( drawType )
		{
			case SGL_POINT  :
			{
				sglBegin(SGL_POINTS);

				for (int_fast32_t i = offset; i <= RR; i++)
				{
					x2 = ca * x1 - sa * y1;
					y2 = sa * x1 + ca * y1;
					setVertex2f(x2 + x, y2 + y);
					x1 = x2;
					y1 = y2;
				}
			}
			break;

			case SGL_LINE   :
			{

				sglBegin(SGL_LINE_STRIP);

				for (int_fast32_t i = offset; i <= RR; i++)
				{
					x2 = ca * x1 - sa * y1;
					y2 = sa * x1 + ca * y1;
					setVertex2f(x2 + x, y2 + y);
					x1 = x2;
					y1 = y2;
				}
			}
			break;

			default         :
			{
				//Fill Arct bi triangle fan (can we do it better ? )
				sglBegin(SGL_TRIANGLES);

				//Center
				setVertex2f(x, y);

				for (int_fast32_t i = offset; i <= RR; i++)
				{
					x2 = ca * x1 - sa * y1;
					y2 = sa * x1 + ca * y1;
					setVertex2f(x2 + x, y2 + y);

					x1 = x2;
					y1 = y2;
				}
			}break;
		}

		sglEnd();
	}

	inline void multiplyCurrentMatrix(Matrix & m)
	{
		switch(matrixMode)
		{
			case SGL_MODELVIEW :
				M_changed	= true;
				current_MV	= current_MV * m;
			break;

			default:
				P_changed	= true;
				current_P	= current_P * m;
			break;
		}
	}

	inline void setViewport(signed x, signed y, signed width, signed height)
	{ viewport.changeViewport(x, y, width, height); }

	inline Matrix & getCurrentMatrix()
	{
		switch(matrixMode)
		{
			case SGL_MODELVIEW	: return current_MV;
			default				: return current_P;
		}
	}

	inline void pushMatrix()
	{
		switch(matrixMode)
		{
			case SGL_MODELVIEW	: P_stack.push_back( current_MV ); break;
			default				: P_stack.push_back( current_P  ); break;
		}
	}

	inline void setCurrentMatrix(Matrix matrix)
	{
		switch(matrixMode)
		{
			case SGL_MODELVIEW	:
				M_changed	= true;
				current_MV	= matrix;
			break;

			default				:
				P_changed	= true;
				current_P	= matrix;
			break;
		}
	}

	inline void setMatrixMode(sglEMatrixMode mode)
	{ matrixMode = mode; }

	inline bool BeginBeforeEnd()
	{ return BEGIN; }

	inline void begin()
	{
		storage.vertices.index = 0;
		BEGIN                  = true;
	}

	inline void end()
	{ BEGIN=false; }

	inline bool stackEmpty()
	{ return (P_stack.size() == 0); }

	inline void popMatrix()
	{
		if (P_stack.size() == 0)
		{
			//*err = SGL_STACK_UNDERFLOW;
		       	return;
		}

		switch(matrixMode)
		{
			case SGL_MODELVIEW	:
				M_changed = true;
				current_MV = P_stack.back();
			break;

			default				:
				P_changed = true;
				current_P = P_stack.back();
			break;
		}

		P_stack.pop_back();
	}

	inline void pushTypeState(sglEElementType type)
	{ types.push_back(type); }

	inline void clearColorBuffer()
	{
		memcpy(storage.buffer, storage.clear, storage.w_h);
	}

	inline void clearDepthBuffer()
	{
		float clear = std::numeric_limits<float>::max();

	    for(uint_fast32_t i = 0; i < storage.w_h; ++i)
	        storage.depth[i] = clear;
	}

	inline bool check_MP()
	{
		if( M_changed || P_changed )
		{
			M_changed	= false;
			P_changed	= false;
			MP			= current_P * current_MV;
			return true;
		}
		return false;
	}

	inline void check_MVP()
	{
		if( check_MP() || viewport.V_changed )
		{
			viewport.V_changed	= false;
			MVP					= viewport.V * MP;
		}
	}

	//FIXME rethink this again and make it faster
	inline void cacheClear(float r, float g, float b, float a)
	{
		Color c = Color(r,g,b);

		//1-6 = RGB RGB
		*((__color*) (storage.clear))	= *((__color*) &c);	//1-3 RGB
		*((__color*) (storage.clear+2))	= *((__color*) &c);	//4-7 RGB

		uint_fast32_t l	= 2				;
		int_fast8_t s	= sizeof(Color)	;

		for(uint_fast32_t offset=l ; offset < storage.w_h; offset <<= 1)
		{
			memcpy(&storage.clear[offset], &storage.clear[l], s);
			l = offset;
		}
	}

	void enableDepthTest()
	{
		g.set(&DrawingLibraryDepth::instance());
		Z_TEST = true;
	}

	void disableDepthTest()
	{
		g.set(&DrawingLibraryFlat::instance());
		Z_TEST = false;
	}

};

DrawingLibraryBase Context::g = DrawingLibraryBase();

#endif /* DATA_H_ */
