/*
 * DrawingLibraryBase.h
 *
 *  Created on: 21.10.2013
 *      Author: jancajthaml
 */

#ifndef DRAWINGLIBRARYBASE_H_
#define DRAWINGLIBRARYBASE_H_

#include <stdint.h>
#include "./Helpers.h"
#include "./../struct/Color.h"
#include "./../struct/Vertex.h"
#include "./../struct/Edge.h"
#include "./../struct/VertexStack.h"
#include "./../core/ContextChunk.h"

class DrawingLibraryInterface
{
	public:

		virtual void drawLine2D		( Vertex a, Vertex b, Chunk &context) = 0;
		virtual void drawPoints		( Chunk &context ) = 0;
		virtual void drawLines		( Chunk &context ) = 0;
		virtual void drawLineStrip	( Chunk &context ) = 0;
		virtual void drawLineLoop	( Chunk &context ) = 0;
		virtual void drawPolygon	( Chunk &context ) = 0;
		virtual void drawCircle		( Vertex v,float r, Chunk &context) = 0;
		virtual void fillCircle		( Vertex v,float r,Chunk &context ) = 0;
		virtual void drawTrianglesFan	( Chunk &context ) = 0;
		virtual void drawTrianglesStrip	( Chunk &context ) = 0;

		//virtual void drawArc2D		( float x, float y, float z, float r, float from, float to, Chunk &context) = 0;
		//virtual void fillArc2D		( float x, float y, float z, float r, float from, float to, Chunk &context) = 0;
		virtual void fillPolygon	( Chunk &context ) = 0;

	protected:
		DrawingLibraryInterface()	{};
    	~DrawingLibraryInterface()	{};

};

class DrawingLibraryBase
{
	public:
		DrawingLibraryBase () : state(){};

		void drawLine2D		( Vertex a, Vertex b, Chunk &context);
		void drawPoints		( Chunk &context );
		void drawLines		( Chunk &context );
		void drawLineStrip	( Chunk &context );
		void drawLineLoop	( Chunk &context );
		void drawPolygon	( Chunk &context );
		void drawCircle		( Vertex v,float r, Chunk &context);
		void drawTrianglesFan	( Chunk &context );
		void drawTrianglesStrip	( Chunk &context );

	//	void drawArc2D		( float x, float y, float z, float r, float from, float to, Chunk &context);
//		void fillArc2D		( float x, float y, float z, float r, float from, float to, Chunk &context);

		void fillCircle		( Vertex v,float r, Chunk &context );
		void fillPolygon	( Chunk &context );

		void set (DrawingLibraryInterface* newState){ state = newState; }

  	  private:
    	friend class DrawingLibraryInterface;
    	DrawingLibraryInterface* state;
};

void DrawingLibraryBase::drawLine2D		( Vertex a, Vertex b, Chunk &context)										{ state->drawLine2D(a, b, context);				}
void DrawingLibraryBase::drawPoints		( Chunk &context )															{ state->drawPoints( context );			}
void DrawingLibraryBase::drawLines		( Chunk &context )															{ state->drawLines( context );			}
void DrawingLibraryBase::drawLineStrip	( Chunk &context )															{ state->drawLineStrip(context);		}
void DrawingLibraryBase::drawLineLoop	( Chunk &context )															{ state->drawLineLoop(context);			}
void DrawingLibraryBase::drawPolygon	( Chunk &context )															{ state->drawPolygon(context);			}
void DrawingLibraryBase::drawCircle		( Vertex v,float r, Chunk &context)											{ state->drawCircle( v, r, context);	}
void DrawingLibraryBase::drawTrianglesFan	( Chunk &context)															{ state->drawTrianglesFan( context);	}
void DrawingLibraryBase::drawTrianglesStrip	( Chunk &context)															{ state->drawTrianglesStrip( context);	}

//void DrawingLibraryBase::drawArc2D		( float x, float y, float z, float r, float from, float to, Chunk &context)	{ state->drawArc2D( x, y, z, r, from, to, context);	}
//void DrawingLibraryBase::fillArc2D		( float x, float y, float z, float r, float from, float to, Chunk &context)	{ state->fillArc2D( x, y, z, r, from, to, context);	}
void DrawingLibraryBase::fillCircle		( Vertex v,float r,Chunk &context )											{ state->fillCircle( v, r, context );	}
void DrawingLibraryBase::fillPolygon	( Chunk &context )															{ state->fillPolygon( context );		}

#endif /* DRAWINGLIBRARYBASE_H_ */
